use crate::iomuxc::gpio_sd::*;

pub struct DuoPins {
    pub led_pin: GPIO_SD_05,
}

#[inline]
pub const fn from_pads(iomuxc: crate::iomuxc::Pads) -> DuoPins {
    DuoPins {
        led_pin: iomuxc.gpio_sd.p05,
    }
}
