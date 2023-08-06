pub mod prelude {
    use cortex_m::asm::delay;
    use stm32f1xx_hal::device::{TIM2, TIM3};
    use stm32f1xx_hal::gpio::{ErasedPin, Output};
    use stm32f1xx_hal::pac;
    use stm32f1xx_hal::prelude::*;
    use stm32f1xx_hal::timer::{PwmChannel, Tim2NoRemap, Tim3NoRemap, Timer};
    use stm32f1xx_hal::usb::Peripheral;

    use crate::fan::Fan;
    use crate::fanmanager::FanManager;

    pub type FanType0 = Fan<ErasedPin<Output>, PwmChannel<TIM2, 0>>;
    pub type FanType1 = Fan<ErasedPin<Output>, PwmChannel<TIM2, 1>>;
    pub type FanType2 = Fan<ErasedPin<Output>, PwmChannel<TIM2, 2>>;
    pub type FanType3 = Fan<ErasedPin<Output>, PwmChannel<TIM3, 0>>;
    pub type FanType4 = Fan<ErasedPin<Output>, PwmChannel<TIM3, 1>>;
    pub type FanType5 = Fan<ErasedPin<Output>, PwmChannel<TIM3, 2>>;

    pub type FanManagerType =
        FanManager<FanType0, FanType1, FanType2, FanType3, FanType4, FanType5>;

    pub fn init() -> (FanManagerType, Peripheral) {
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

        let mut gpioa = p.GPIOA.split();
        let mut gpiob = p.GPIOB.split();

        // TIM2
        let mut pwm_pin0 = gpioa.pa0.into_alternate_push_pull(&mut gpioa.crl);
        let mut pwm_pin1 = gpioa.pa1.into_alternate_push_pull(&mut gpioa.crl);
        let mut pwm_pin2 = gpioa.pa2.into_alternate_push_pull(&mut gpioa.crl);
        // TIM3
        let mut pwm_pin3 = gpioa.pa6.into_alternate_push_pull(&mut gpioa.crl);
        let mut pwm_pin4 = gpioa.pa7.into_alternate_push_pull(&mut gpioa.crl);
        let mut pwm_pin5 = gpiob.pb0.into_alternate_push_pull(&mut gpiob.crl);
        let power_pin0 = gpiob.pb5.into_push_pull_output(&mut gpiob.crl).erase();
        let power_pin1 = gpiob.pb6.into_push_pull_output(&mut gpiob.crl).erase();
        let power_pin2 = gpiob.pb7.into_push_pull_output(&mut gpiob.crl).erase();
        let power_pin3 = gpioa.pa3.into_push_pull_output(&mut gpioa.crl).erase();
        let power_pin4 = gpioa.pa4.into_push_pull_output(&mut gpioa.crl).erase();
        let power_pin5 = gpioa.pa5.into_push_pull_output(&mut gpioa.crl).erase();
        let mut is_4pin: [bool; 6] = [true; 6];

        pwm_pin0.as_pull_down_input(&mut gpioa.crl, |f| is_4pin[0] = f.is_high());
        pwm_pin1.as_pull_down_input(&mut gpioa.crl, |f| is_4pin[1] = f.is_high());
        pwm_pin2.as_pull_down_input(&mut gpioa.crl, |f| is_4pin[2] = f.is_high());
        pwm_pin3.as_pull_down_input(&mut gpioa.crl, |f| is_4pin[3] = f.is_high());
        pwm_pin4.as_pull_down_input(&mut gpioa.crl, |f| is_4pin[4] = f.is_high());
        pwm_pin5.as_pull_down_input(&mut gpiob.crl, |f| is_4pin[5] = f.is_high());

        let tim2 = &p.TIM2;
        // TODO: set correct registers
        tim2.ccer.modify(|_r, w| w.cc1e().set_bit());
        //    let pin: Pin<'B', 0> = hal::gpio::Pin {
        //        mode: Default::default(),
        //    };
        //    let pin2 = get_gpio!('B', 0);

        let timer2 = Timer::new(p.TIM2, &clocks);
        let pwm = timer2.pwm_hz::<Tim2NoRemap, _, _>(
            (pwm_pin0, pwm_pin1, pwm_pin2),
            &mut afio.mapr,
            1.kHz(),
        );
        let timer3 = Timer::new(p.TIM3, &clocks);
        let pwm_channels3 = timer3
            .pwm_hz::<Tim3NoRemap, _, _>((pwm_pin3, pwm_pin4, pwm_pin5), &mut afio.mapr, 1.kHz())
            .split();

        let pwm_channels2 = pwm.split();

        let fan0 = Fan::<ErasedPin<Output>, _>::new(power_pin0, pwm_channels2.0, is_4pin[0]);
        let fan1 = Fan::<ErasedPin<Output>, _>::new(power_pin1, pwm_channels2.1, is_4pin[1]);
        let fan2 = Fan::<ErasedPin<Output>, _>::new(power_pin2, pwm_channels2.2, is_4pin[2]);
        let fan3 = Fan::<ErasedPin<Output>, _>::new(power_pin3, pwm_channels3.0, is_4pin[3]);
        let fan4 = Fan::<ErasedPin<Output>, _>::new(power_pin4, pwm_channels3.1, is_4pin[4]);
        let fan5 = Fan::<ErasedPin<Output>, _>::new(power_pin5, pwm_channels3.2, is_4pin[5]);

        let manager: FanManagerType = FanManager::new(fan0, fan1, fan2, fan3, fan4, fan5);

        // USB
        let usb_dm = gpioa.pa11;
        let mut usb_dp = gpioa.pa12.into_push_pull_output(&mut gpioa.crh);
        usb_dp.set_low();
        delay(clocks.sysclk().raw() / 100);
        let usb = Peripheral {
            usb: p.USB,
            pin_dm: usb_dm,
            pin_dp: usb_dp.into_floating_input(&mut gpioa.crh),
        };
        (manager, usb)
    }
}
