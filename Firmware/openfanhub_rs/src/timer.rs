use stm32f1xx_hal::{device::TIM2, pac::interrupt};

struct TimerProperties {
    timer_num: u32,
    status_register: u32,
}

#[interrupt]
fn TIM2() {
    let timer = unsafe { &*TIM2::ptr() };

    let prop = TimerProperties {
        timer_num: 2,
        status_register: timer.sr.read().bits(),
    };
}