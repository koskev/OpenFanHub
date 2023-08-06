#![no_std]
#![no_main]

use cortex_m_semihosting::hprintln;
use panic_halt as _;
use usbd_hid::descriptor::{AsInputReport, MouseReport, SerializedDescriptor};
use usbd_serial::{SerialPort, USB_CLASS_CDC};

use core::cell::RefCell;
use core::marker::PhantomData;
use core::usize;

use cortex_m::asm::wfi;
use cortex_m::interrupt::{free, Mutex};
use cortex_m::peripheral::NVIC;
use hal::device::{GPIOA, TIM2};
use hal::gpio::{self, gpioa, ErasedPin, Floating, Input, Output, Pin};
use hal::timer::pwm::Pins;
use hal::timer::{
    Channel, Configuration, Error, Instance, PwmChannel, Tim1NoRemap, Tim2NoRemap, Tim3NoRemap,
};
use hal::usb::Peripheral;
use num_traits::FromPrimitive;

use cortex_m_rt::entry;
use embedded_hal::digital::v2::{InputPin, IoPin, OutputPin};
use embedded_hal::PwmPin;
use hal::pac::interrupt;
use hal::{pac, prelude::*, timer::Timer};
use stm32_usbd::UsbBus;
use stm32f1xx_hal as hal;
use usb_device::class_prelude::UsbBusAllocator;
use usb_device::prelude::*;
use usbd_hid::hid_class::HIDClass;

use enum_primitive_derive::Primitive;

use serde::Serialize;

use paste::paste;

mod fan;
mod fanmanager;
mod targets;
mod timer;

use fan::Fan;
use fanmanager::FanManager;
use targets::*;

const USB_CLASS_HID: u8 = 0x03;

const NUM_TEMP_SENSORS: usize = 4;
const NUM_FANS: usize = 6; // XXX: Limited by driver

#[derive(Primitive)]
enum UsbControl {
    GetTmpCnct = 0x10,
    GetTmp = 0x11,
    GetFanCnct = 0x20,
    GetFanRpm = 0x21,
    GetFanPwm = 0x22,
    SetFanFpwm = 0x23,
    SetFanTarget = 0x24,
}

const USB_DESCRIPTOR: &[u8] = &[
    0x05, 0x01, // Usage Page (Generic Desktop)
    0x09, 0x33, // Usage (X, Y, and Z Axes)
    0xA1, 0x01, // Collection (Application)
    0x19, 0x01, // Usage Minimum (1)
    0x29, 0x10, // Usage Maximum (16)
    0x15, 0x00, // Logical Minimum (0)
    0x26, 0xFE, 0x00, // Logical Maximum (254)
    0x75, 0x08, // Report Size (8 bits)
    0x95, 0x10, // Report Count (16)
    0x81, 0x02, // Input (Data, Variable, Absolute)
    0x19, 0x01, // Usage Minimum (1)
    0x29, 0xFE, // Usage Maximum (254)
    0x15, 0x00, // Logical Minimum (0)
    0x26, 0xFE, 0x00, // Logical Maximum (254)
    0x75, 0x08, // Report Size (8 bits)
    0x95, 0x3F, // Report Count (63)
    0x91, 0x02, // Output (Data, Variable, Absolute, Buffered)
    0xC0, // End Collection
];

macro_rules! get_gpio {
    ($port: expr, $pin: expr ) => {{
        let pin: Pin<$port, $pin> = hal::gpio::Pin {
            mode: Default::default(),
        };
        pin
    }};
}

#[derive(Serialize)]
struct FanHubReport {
    bytes: [u8; 16],
}

impl FanHubReport {
    fn new() -> Self {
        Self { bytes: [0; 16] }
    }
}

impl AsInputReport for FanHubReport {}
//
//impl AsRef<[u8]> for FanHubReport {
//    fn as_ref(&self) -> &[u8] {
//        &self.bytes
//    }
//}
//
//impl HidReport for FanHubReport {
//    const DESCRIPTOR: &'static [u8] = USB_DESCRIPTOR;
//}

enum TimerTypeF103 {
    TIM2(Timer<TIM2>),
}

enum PwmChannelTypeF103 {
    TIM2(PwmChannel<TIM2, 1>),
    TIM3(PwmChannel<TIM2, 2>),
}

macro_rules! create_ic {
    ($timer: ident, $ccmr_num: expr, $capture_num: expr, $ti_num: expr) => {
        let t = unsafe { &*<$timer>::ptr() };
        // set output channel
        paste! {
            t.[<ccmr $ccmr_num _input>]()
                .modify(|_r, w| w.[<cc $capture_num s>]().[<ti $ti_num>]());
            // enable IC

            t.ccer.modify(|_r, w| w.[<cc $capture_num e>]().set_bit());

        }
    };
}

pub struct InputCapture<TIM, const ccmr_num: usize, const capture_num: usize, const ti_num: usize> {
    _marker: PhantomData<TIM>,
}

//impl InputCapture<TIM2, 1, 1, 1> {
//    fn new() -> Self {
//        //create_ic!(TIM2, RegisterNum, CaptureNume, TiNum);
//        create_ic!(TIM2, 1, 1, 1);
//        Self {
//            _marker: PhantomData,
//        }
//    }
//}

macro_rules! impl_ic_new {
    ($tim: ident, $ccmr: expr, $capture: expr, $ti: expr) => {
        impl InputCapture<$tim, $ccmr, $capture, $ti> {
            fn new() -> Self {
                //create_ic!(TIM2, RegisterNum, CaptureNume, TiNum);
                //create_ic!($tim, $ccmr, $capture, $ti);
                Self {
                    _marker: PhantomData,
                }
            }

            fn enable_ic(&self) {
                let t = unsafe { &*<$tim>::ptr() };
                // set output channel
                paste! {
                    t.[<ccmr $ccmr _input>]()
                        .modify(|_r, w| w.[<cc $capture s>]().[<ti $ti>]());
                    // enable IC

                    t.ccer.modify(|_r, w| w.[<cc $capture e>]().set_bit());

                }
            }
        }
    };
}

impl_ic_new!(TIM2, 1, 1, 2);
impl_ic_new!(TIM2, 1, 1, 1);

macro_rules! REGISTER_FAN {
    ($num: expr, $pwm_timer: ty, $pwm_channel: expr) => {
        paste! {
            mod [< fan $num >] {
                fn [< init_fan_ $num  >] () {

                }
            }
        }
    };
}

struct FanHubUsb {
    device: UsbDevice<'static, UsbBus<Peripheral>>,
    hid: HIDClass<'static, UsbBus<Peripheral>>,
}

static USB: Mutex<RefCell<Option<FanHubUsb>>> = Mutex::new(RefCell::new(None));

static FANMANAGER: Mutex<RefCell<Option<FanManagerType>>> = Mutex::new(RefCell::new(None));

#[entry]
fn main() -> ! {
    let (manager, usb) = init();
    hprintln!("Hello");
    free(|cs| {
        FANMANAGER.borrow(&cs).borrow_mut().replace(manager);
    });

    static mut USB_BUS: Option<UsbBusAllocator<UsbBus<Peripheral>>> = None;
    free(move |cs| {
        let usb_bus = UsbBus::new(usb);
        let usb_bus = unsafe {
            USB_BUS = Some(usb_bus);
            USB_BUS.as_ref().unwrap()
        };

        let mut hid = HIDClass::new(&usb_bus, USB_DESCRIPTOR, 10);

        let mut usb_dev = UsbDeviceBuilder::new(&usb_bus, UsbVidPid(0x1b1c, 0x0c10))
            .manufacturer("Koskev")
            .product("OpenFanHub")
            .serial_number("TEST")
            .device_class(USB_CLASS_HID)
            .build();

        //loop {
        //    if !usb_dev.poll(&mut [&mut hid]) {
        //        continue;
        //    }
        //}

        let usb = FanHubUsb {
            device: usb_dev,
            hid,
        };
        USB.borrow(&cs).replace(Some(usb));
    });

    // unmask nvic
    unsafe {
        NVIC::unmask(interrupt::TIM2);
        //NVIC::unmask(interrupt::USB_HP_CAN_TX);
        NVIC::unmask(interrupt::USB_LP_CAN_RX0);
    }

    loop {
        //usb_interrupt();
        wfi();
    }
}

fn usb_interrupt() {
    free(move |cs| {
        let mut borrow = USB.borrow(&cs).borrow_mut();
        let usb = &mut borrow.as_mut().unwrap();
        let mut data: [u8; 20] = [0; 20];
        let res = usb.hid.pull_raw_output(&mut data);
        let mut report = FanHubReport::new();
        match res {
            Ok(_size) => {
                hprintln!("Int");
                let cmd = data[0];
                let e = UsbControl::from_u8(cmd);
                hprintln!("{}", cmd);
                match e {
                    Some(ctrl) => match ctrl {
                        UsbControl::GetTmp => {}

                        UsbControl::GetFanRpm => {
                            let manager = FANMANAGER.borrow(&cs).borrow();
                            let fan_num = data[1];
                            let fan = manager.as_ref().unwrap().get_fan(fan_num as usize);
                            let rpm = fan.get_fan_rpm();
                            report.bytes[1] = (rpm >> 8) as u8;
                            report.bytes[2] = rpm as u8;
                        }
                        UsbControl::GetFanPwm => {
                            let manager = FANMANAGER.borrow(&cs).borrow();
                            let fan_num = data[1];
                            let fan = manager.as_ref().unwrap().get_fan(fan_num as usize);
                            let pwm = fan.get_fan_pwm();
                            report.bytes[1] = pwm;
                        }
                        UsbControl::GetTmpCnct => {
                            // fill byte 1-4 with 1 if a temp probe is present
                            for i in 0..NUM_TEMP_SENSORS {
                                report.bytes[i + 1] = 1;
                            }
                        }
                        UsbControl::GetFanCnct => {
                            for i in 0..NUM_FANS {
                                report.bytes[i + 1] = 1; // TODO: use get_fan_type
                            }
                        }
                        UsbControl::SetFanFpwm => {
                            let mut manager = FANMANAGER.borrow(&cs).borrow_mut();
                            let fan_num = data[1];
                            let percentage = data[2];
                            let fan = manager.as_mut().unwrap().get_fan_mut(fan_num as usize);
                            fan.set_fan_pwm_percentage(percentage);
                        }
                        UsbControl::SetFanTarget => (),
                    },
                    None => (),
                }
            }
            Err(_e) => (),
        }
        let _ = usb.hid.push_input(&report);
        usb.device.poll(&mut [&mut usb.hid]);
    });
}

#[interrupt]
fn USB_HP_CAN_TX() {
    usb_interrupt();
}

#[interrupt]
fn USB_LP_CAN_RX0() {
    usb_interrupt();
}
