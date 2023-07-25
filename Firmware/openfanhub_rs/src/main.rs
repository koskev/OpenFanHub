#![no_std]
#![no_main]

use core::cell::RefCell;

use cortex_m::asm::wfi;
use cortex_m::interrupt::{free, Mutex};
use cortex_m::peripheral::NVIC;
use hal::device::TIM2;
use hal::timer::pwm::Pins;
use hal::timer::Configuration;
use hal::usb::Peripheral;
use panic_halt as _;

use cortex_m_rt::entry;
use embedded_hal::digital::v2::{InputPin, OutputPin};
use embedded_hal::Pwm;
use hal::pac::interrupt;
use hal::{pac, prelude::*, timer::Timer};
use stm32_usbd::UsbBus;
use stm32f1xx_hal as hal;
use usb_device::class_prelude::UsbBusAllocator;
use usb_device::prelude::*;
use usbd_hid::hid_class::HIDClass;

mod timer;

const USB_CLASS_HID: u8 = 0x03;

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

//struct FanHubReport {
//    bytes: [u8; 16],
//}
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

struct Fan<OP, PWM, IC> {
    pwm_percent: u8,
    rpm: u16,
    is_4pin: bool,
    fan_detected: bool,

    power_switch_pin: OP,
    pwm_output: PWM,
    ic_handle: Timer<IC>,

    //TIM_HandleTypeDef* ic_handle;
    ic_channel: u32,
    //uint32_t ic_channel;
    //uint32_t ic_channel_active;

    //// for input capture
    ic_overflow: u32,
    last_val: u32,
}

enum FanType {
    NoFan,
    Fan,
    Fan4pin,
}

impl<OP, PWM, IC> Fan<OP, PWM, IC>
where
    OP: OutputPin,
    PWM: Pwm,
{
    fn get_fan_type(&self) -> FanType {
        if self.rpm == 0 {
            FanType::NoFan
        } else if self.is_4pin {
            FanType::Fan4pin
        } else {
            FanType::Fan
        }
    }

    fn get_fan_rpm(&self) -> u16 {
        self.rpm
    }

    fn get_fan_pwm(&self) -> u8 {
        self.pwm_percent
    }

    fn new(power_pin: OP, pwm_pin: PWM, ic_pin: Timer<IC>, ic_channel: u32) -> Self {
        Self {
            pwm_percent: 0,
            rpm: 0,
            is_4pin: false,
            fan_detected: false,
            power_switch_pin: power_pin,
            pwm_output: pwm_pin,
            ic_handle: ic_pin,
            ic_channel,
            ic_overflow: 0,
            last_val: 0,
        }
    }

    fn set_fan_pwm_percentage(&mut self, pwm_percent: u8) {
        if pwm_percent > 0 {
            //let pwm = self.pwm_output.get_period().unwrap();
            //* (pwm_percent/100) + 0.5;
            //self.pwm_output.set_duty(channel, duty);
            //self.power_switch_pin.set_high();
            //pwm = handle->Init.Period * (pwm_percent/100.0f) + 0.5;
            //__HAL_TIM_SET_COMPARE(handle, fan->pwm_channel, pwm);
            //HAL_GPIO_WritePin(fan->power_switch_port, fan->power_switch_pin, GPIO_PIN_SET);
        } else {
            // Shut down fan
            self.power_switch_pin.set_low();
        }
        self.pwm_percent = pwm_percent;
    }
}

struct FanHubUsb {
    device: UsbDevice<'static, UsbBus<Peripheral>>,
    hid: Hid<'static, FanHubReport, UsbBus<Peripheral>>,
}

static USB: Mutex<RefCell<Option<FanHubUsb>>> = Mutex::new(RefCell::new(None));

#[entry]
fn main() -> ! {
    let p = pac::Peripherals::take().unwrap();

    let mut flash = p.FLASH.constrain();
    let rcc = p.RCC.constrain();

    let clocks = rcc
        .cfgr
        .use_hse(8.MHz())
        .sysclk(72.MHz())
        .pclk1(36.MHz())
        .freeze(&mut flash.acr);

    assert!(clocks.usbclk_valid());

    let mut afio = p.AFIO.constrain();
    let mut dbg = p.DBGMCU;

    let gpioa = p.GPIOA.split();
    let mut gpiob = p.GPIOB.split();

    // TIM3
    let pwm_pin = gpiob.pb5.into_alternate_push_pull(&mut gpiob.crl);
    //let power_pin = gpiob.pb5.into_push_pull_output(&mut gpiob.crl);
    //let input_pin = gpioa.pa1;

    let tim2 = &p.TIM2;
    // TODO: set correct registers
    tim2.ccer.modify(|_r, w| w.cc1e().set_bit());

    let pwm = Timer::new(p.TIM3, &clocks).pwm_hz(pwm_pin, &mut afio.mapr, 1.kHz());

    let max = pwm.get_max_duty();

    let mut pwm_channels = pwm.split();

    //    let fan = Fan::new(power_pin, pwm, pwm_input);

    // USB
    let usb_dm = gpioa.pa11;
    let usb_dp = gpioa.pa12;
    let usb = Peripheral {
        usb: p.USB,
        pin_dm: usb_dm,
        pin_dp: usb_dp,
    };

    static mut USB_BUS: Option<UsbBusAllocator<UsbBus<Peripheral>>> = None;
    free(move |cs| {
        let usb_bus = UsbBus::new(usb);
        let usb_bus = unsafe {
            USB_BUS = Some(usb_bus);
            USB_BUS.as_ref().unwrap()
        };
        let usb_dev = UsbDeviceBuilder::new(&usb_bus, UsbVidPid(0x1b1c, 0x0c10))
            .manufacturer("OpenFanHub")
            .product("OpenFanHub")
            .device_class(USB_CLASS_HID)
            .build();

        let hid = HIDClass::new(&usb_bus, USB_DESCRIPTOR, 10);

        let usb = FanHubUsb {
            device: usb_dev,
            hid,
        };
        USB.borrow(&cs).replace(Some(usb));
    });

    // unmask nvic
    unsafe {
        NVIC::unmask(interrupt::TIM2);
        NVIC::unmask(interrupt::USB_HP_CAN_TX);
        NVIC::unmask(interrupt::USB_LP_CAN_RX0);
    }

    loop {
        wfi();
    }
}

fn usb_interrupt() {
    free(move |cs| {
        let mut borrow = USB.borrow(&cs).borrow_mut();
        let usb = &mut borrow.as_mut().unwrap();
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
