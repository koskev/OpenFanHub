use crate::fan::PwmFan;

pub struct FanManager<F0, F1, F2, F3, F4, F5> {
    fan0: F0,
    fan1: F1,
    fan2: F2,
    fan3: F3,
    fan4: F4,
    fan5: F5,
}

impl<F0, F1, F2, F3, F4, F5> FanManager<F0, F1, F2, F3, F4, F5> {
    pub fn new(fan0: F0, fan1: F1, fan2: F2, fan3: F3, fan4: F4, fan5: F5) -> Self {
        Self {
            fan0,
            fan1,
            fan2,
            fan3,
            fan4,
            fan5,
        }
    }
}

impl<F0, F1, F2, F3, F4, F5> FanManager<F0, F1, F2, F3, F4, F5>
where
    F0: PwmFan,
    F1: PwmFan,
    F2: PwmFan,
    F3: PwmFan,
    F4: PwmFan,
    F5: PwmFan,
{
    pub fn get_fan_mut(&mut self, num: usize) -> &mut dyn PwmFan {
        match num {
            0 => &mut self.fan0,
            1 => &mut self.fan1,
            2 => &mut self.fan2,
            3 => &mut self.fan3,
            4 => &mut self.fan4,
            5 => &mut self.fan5,
            _ => unimplemented!(),
        }
    }

    pub fn get_fan(&self, num: usize) -> &dyn PwmFan {
        match num {
            0 => &self.fan0,
            1 => &self.fan1,
            2 => &self.fan2,
            3 => &self.fan3,
            4 => &self.fan4,
            5 => &self.fan5,
            _ => unimplemented!(),
        }
    }
}
