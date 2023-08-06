use embedded_hal::{digital::v2::OutputPin, PwmPin};

pub struct Fan<OP, PWM> {
    pub pwm_percent: u8,
    pub rpm: u16,
    pub is_4pin: bool,
    pub fan_detected: bool,

    pub power_switch_pin: OP,
    pub pwm_output: PWM,
    //pub ic_handle: Timer<IcTimer>,

    //TIM_HandleTypeDef* ic_handle;
    //pub ic_channel: u32,
    //uint32_t ic_channel;
    //uint32_t ic_channel_active;

    //// for input capture
    pub ic_overflow: u32,
    pub last_val: u32,
}

pub enum FanType {
    NoFan,
    Fan,
    Fan4pin,
}

//static FANS: Mutex<RefCell<[Option<Fan<gpio::Output, PwmChannelTypeF103>>; NUM_FANS]>> =
//    Mutex::new(RefCell::new([None; NUM_FANS]));

impl<OP, PWM> Fan<OP, PWM>
where
    OP: OutputPin,
    PWM: PwmPin<Duty = u16>,
{
    pub fn new(
        power_pin: OP,
        pwm_output: PWM,
        is_4pin: bool,
        //pwm_pin: OP,
        //ic_timer: Timer<IcTimer>,
        //ic_channel: u32,
    ) -> Self {
        Self {
            pwm_percent: 0,
            rpm: 0,
            is_4pin,
            fan_detected: false,
            power_switch_pin: power_pin,
            pwm_output,
            //ic_handle: ic_timer,
            //ic_channel,
            ic_overflow: 0,
            last_val: 0,
        }
    }
}

pub trait PwmFan {
    fn get_fan_type(&self) -> FanType;
    fn get_fan_rpm(&self) -> u16;
    fn get_fan_pwm(&self) -> u8;
    fn set_fan_pwm_percentage(&mut self, pwm_percent: u8);
}

impl<OP, PWM> PwmFan for Fan<OP, PWM>
where
    OP: OutputPin,
    PWM: PwmPin<Duty = u16>,
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

    fn set_fan_pwm_percentage(&mut self, pwm_percent: u8) {
        if pwm_percent > 0 {
            let _ = self.power_switch_pin.set_high();
            let pwm = self.pwm_output.get_max_duty() as f32 * (pwm_percent as f32 / 100.0) + 0.5;
            self.pwm_output.set_duty(pwm as u16);
        } else {
            // Shut down fan
            let _ = self.power_switch_pin.set_low();
        }
        self.pwm_percent = pwm_percent;
    }
}
