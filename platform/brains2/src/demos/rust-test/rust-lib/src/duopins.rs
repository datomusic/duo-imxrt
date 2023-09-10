// pub use crate::common::*;
// use crate::iomuxc::{gpio_sd::*, ErasedPad};
// use crate::iomuxc::{gpio_ad_b0::*, gpio_ad_b1::*, gpio_b0::*, gpio_b1::*, gpio_emc::*};
// use crate::iomuxc::ErasedPad;
// use crate::iomuxc::gpio_ad::*;
use crate::iomuxc::gpio::*;
// use crate::iomuxc::gpio_ad::*;
// use crate::iomuxc::gpio_sd::*;

// /// Pin 0 (common)
// pub type P0 = GPIO_AD_00;
// /// Pin 1 (common)
// pub type P1 = GPIO_AD_B0_02;
// /// Pin 2 (common)
// pub type P2 = GPIO_EMC_04;
// /// Pin 3 (common)
// pub type P3 = GPIO_EMC_05;
// /// Pin 4 (common)
// pub type P4 = GPIO_EMC_06;
// /// Pin 5 (common)
// pub type P5 = GPIO_EMC_08;
// /// Pin 6 (common)
// pub type P6 = GPIO_10;
// /// Pin 7 (common)
// pub type P7 = GPIO_01;
/// Pin 8 (common)
pub type P8 = GPIO_08;
/// Pin 9 (common)
pub type P9 = GPIO_09;
/// Pin 10 (common)
pub type P10 = GPIO_10;
// /// Pin 11 (common)
// pub type P11 = GPIO_B0_02;
// /// Pin 12 (common)
// pub type P12 = GPIO_B0_01;
// /// Pin 13 (common)
// pub type P13 = GPIO_B0_03;
// /// Pin 14 (common)
// pub type P14 = GPIO_AD_B1_02;
// /// Pin 15 (common)
// pub type P15 = GPIO_AD_B1_03;
// /// Pin 16 (common)
// pub type P16 = GPIO_AD_B1_07;
// /// Pin 17 (common)
// pub type P17 = GPIO_AD_B1_06;
// /// Pin 18 (common)
// pub type P18 = GPIO_AD_B1_01;
// /// Pin 19 (common)
// pub type P19 = GPIO_AD_B1_00;
// /// Pin 20 (common)
// pub type P20 = GPIO_AD_B1_10;
// /// Pin 21 (common)
// pub type P21 = GPIO_AD_B1_11;
// /// Pin 22 (common)
// pub type P22 = GPIO_AD_B1_08;
// /// Pin 23 (common)
// pub type P23 = GPIO_AD_B1_09;
// /// Pin 24 (common)
// pub type P24 = GPIO_AD_B0_12;
// /// Pin 25 (common)
// pub type P25 = GPIO_AD_B0_13;
// /// Pin 26 (common)
// pub type P26 = GPIO_AD_B1_14;
// /// Pin 27 (common)
// pub type P27 = GPIO_AD_B1_15;
// /// Pin 28 (common)
// pub type P28 = GPIO_EMC_32;
// /// Pin 29 (common)
// pub type P29 = GPIO_EMC_31;
// /// Pin 30 (common)
// pub type P30 = GPIO_EMC_37;
// /// Pin 31 (common)
// pub type P31 = GPIO_EMC_36;
// /// Pin 32 (common)
// pub type P32 = GPIO_B0_12;
// /// Pin 33 (common)
// pub type P33 = GPIO_EMC_07;
//

// /// Pin 34 (4.0)
// pub type P34 = GPIO_SD_B0_03;
// /// Pin 35 (4.0)
// pub type P35 = GPIO_SD_B0_02;
// /// Pin 36 (4.0)
// pub type P36 = GPIO_SD_B0_01;
// /// Pin 37 (4.0)
// pub type P37 = GPIO_SD_B0_00;
// /// Pin 38 (4.0)
// pub type P38 = GPIO_SD_B0_05;
// /// Pin 39 (4.0)
// pub type P39 = GPIO_SD_B0_04;

// pub type ErasedPins = [ErasedPad; 3];

pub struct DuoPins {
    // /// Pin 0
    // pub p0: P0,
    // /// Pin 1
    // pub p1: P1,
    // /// Pin 2
    // pub p2: P2,
    // /// Pin 3
    // pub p3: P3,
    // /// Pin 4
    // pub p4: P4,
    // /// Pin 5
    // pub p5: P5,
    // /// Pin 6
    // pub p6: P6,
    // /// Pin 7
    // pub p7: P7,
    /// Pin 8
    pub p8: P8,
    /// Pin 9
    pub p9: P9,
    /// Pin 10
    pub p10: P10,
    // /// Pin 11
    // pub p11: P11,
    // /// Pin 12
    // pub p12: P12,
    // /// Pin 13
    // pub p13: P13,
    // /// Pin 14
    // pub p14: P14,
    // /// Pin 15
    // pub p15: P15,
    // /// Pin 16
    // pub p16: P16,
    // /// Pin 17
    // pub p17: P17,
    // /// Pin 18
    // pub p18: P18,
    // /// Pin 19
    // pub p19: P19,
    // /// Pin 20
    // pub p20: P20,
    // /// Pin 21
    // pub p21: P21,
    // /// Pin 22
    // pub p22: P22,
    // /// Pin 23
    // pub p23: P23,
    // /// Pin 24
    // pub p24: P24,
    // /// Pin 25
    // pub p25: P25,
    // /// Pin 26
    // pub p26: P26,
    // /// Pin 27
    // pub p27: P27,
    // /// Pin 28
    // pub p28: P28,
    // /// Pin 29
    // pub p29: P29,
    // /// Pin 30
    // pub p30: P30,
    // /// Pin 31
    // pub p31: P31,
    // /// Pin 32
    // pub p32: P32,
    // /// Pin 33
    // pub p33: P33,
    // // END OF COMMON PINS
    // /// Pin 34
    // pub p34: P34,
    // /// Pin 35
    // pub p35: P35,
    // /// Pin 36
    // pub p36: P36,
    // /// Pin 37
    // pub p37: P37,
    // /// Pin 38
    // pub p38: P38,
    // /// Pin 39
    // pub p39: P39,
}

/// Constrain the processor pads to the Teensy 4.0 pins
#[inline]
pub const fn from_pads(iomuxc: crate::iomuxc::Pads) -> DuoPins {
    DuoPins {
        // p0: iomuxc.gpio_ad.p00,
        // p1: iomuxc.gpio_ad_b0.p02,
        // p2: iomuxc.gpio_emc.p04,
        // p3: iomuxc.gpio_emc.p05,
        // p4: iomuxc.gpio_emc.p06,
        // p5: iomuxc.gpio_emc.p08,
        // p6: iomuxc.gpio.p10,
        // p7: iomuxc.gpio.p01,
        p8: iomuxc.gpio.p08,
        p9: iomuxc.gpio.p09,
        p10: iomuxc.gpio.p10,
        // p11: iomuxc.gpio_b0.p02,
        // p12: iomuxc.gpio_b0.p01,
        // p13: iomuxc.gpio_b0.p03,
        // p14: iomuxc.gpio_ad_b1.p02,
        // p15: iomuxc.gpio_ad_b1.p03,
        // p16: iomuxc.gpio_ad_b1.p07,
        // p17: iomuxc.gpio_ad_b1.p06,
        // p18: iomuxc.gpio_ad_b1.p01,
        // p19: iomuxc.gpio_ad_b1.p00,
        // p20: iomuxc.gpio_ad_b1.p10,
        // p21: iomuxc.gpio_ad_b1.p11,
        // p22: iomuxc.gpio_ad_b1.p08,
        // p23: iomuxc.gpio_ad_b1.p09,
        // p24: iomuxc.gpio_ad_b0.p12,
        // p25: iomuxc.gpio_ad_b0.p13,
        // p26: iomuxc.gpio_ad_b1.p14,
        // p27: iomuxc.gpio_ad_b1.p15,
        // p28: iomuxc.gpio_emc.p32,
        // p29: iomuxc.gpio_emc.p31,
        // p30: iomuxc.gpio_emc.p37,
        // p31: iomuxc.gpio_emc.p36,
        // p32: iomuxc.gpio_b0.p12,
        // p33: iomuxc.gpio_emc.p07,
        // // END OF COMMON PINS
        // p34: iomuxc.gpio_sd_b0.p03,
        // p35: iomuxc.gpio_sd_b0.p02,
        // p36: iomuxc.gpio_sd_b0.p01,
        // p37: iomuxc.gpio_sd_b0.p00,
        // p38: iomuxc.gpio_sd_b0.p05,
        // p39: iomuxc.gpio_sd_b0.p04,
    }
}

impl DuoPins {
    /// Create an instance of `Pins` when you do not have a handle
    /// to the processor pads
    ///
    /// # Safety
    ///
    /// Caller must ensure that the pins are not aliased elsewhere in
    /// the program. This could include
    ///
    /// - an existing handle to the `imxrt-iomuxc` pads,
    /// - another instance of `Pins` that was safely acquired
    ///   using [`from_pads`](from_pads()).
    #[inline]
    pub const unsafe fn new() -> Self {
        from_pads(crate::iomuxc::Pads::new())
    }

    // /// Erase the types of all pins
    // #[inline]
    // pub fn erase(self) -> ErasedPins {
    //     [
    //         // self.p0.erase(),
    //         // self.p1.erase(),
    //         // self.p2.erase(),
    //         // self.p3.erase(),
    //         // self.p4.erase(),
    //         // self.p5.erase(),
    //         // self.p6.erase(),
    //         // self.p7.erase(),
    //         self.p8.erase(),
    //         self.p9.erase(),
    //         self.p10.erase(),
    //         // self.p11.erase(),
    //         // self.p12.erase(),
    //         // self.p13.erase(),
    //         // self.p14.erase(),
    //         // self.p15.erase(),
    //         // self.p16.erase(),
    //         // self.p17.erase(),
    //         // self.p18.erase(),
    //         // self.p19.erase(),
    //         // self.p20.erase(),
    //         // self.p21.erase(),
    //         // self.p22.erase(),
    //         // self.p23.erase(),
    //         // self.p24.erase(),
    //         // self.p25.erase(),
    //         // self.p26.erase(),
    //         // self.p27.erase(),
    //         // self.p28.erase(),
    //         // self.p29.erase(),
    //         // self.p30.erase(),
    //         // self.p31.erase(),
    //         // self.p32.erase(),
    //         // self.p33.erase(),
    //         // // END OF COMMON PINS
    //         // self.p34.erase(),
    //         // self.p35.erase(),
    //         // self.p36.erase(),
    //         // self.p37.erase(),
    //         // self.p38.erase(),
    //         // self.p39.erase(),
    //     ]
    // }
}
