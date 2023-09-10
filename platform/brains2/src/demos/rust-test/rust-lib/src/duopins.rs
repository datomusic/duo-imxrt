use crate::iomuxc::gpio_sd::*;

pub struct DuoPins {
    pub led_pin: GPIO_SD_02,
}

#[inline]
pub const fn from_pads(iomuxc: crate::iomuxc::Pads) -> DuoPins {
    DuoPins {
        led_pin: iomuxc.gpio_sd.p02,
    }
}

impl DuoPins {
    #[inline]
    pub const unsafe fn new() -> Self {
        from_pads(crate::iomuxc::Pads::new())
    }
}
