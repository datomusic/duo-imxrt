#![no_main]
#![no_std]

mod iomuxc {
    pub use imxrt_iomuxc::imxrt1010::*;
    pub use imxrt_iomuxc::ErasedPad;
    pub use imxrt_iomuxc::Pad;
}

use core::panic::PanicInfo;
// use hal::iomuxc::flexio::*;
use imxrt_hal as hal;
use imxrt_ral as ral;
mod flexio;
mod pins;
mod pixel;
mod pixelstream;

/// Possible errors that could happen.
pub mod errors;

mod duopins;
mod board;

// pub use flexio::{PreprocessedPixels, WS2812Driver, WriteDmaResult};
pub use flexio::WS2812Driver2;
pub use pins::Pins;
// pub use pixel::Pixel;
// pub use pixelstream::IntoPixelStream;

// use imxrt_iomuxc as iomuxc;

const PIXEL_COUNT: u8 = 19;

unsafe fn clear() {
    for i in 0..PIXEL_COUNT {
        set_pixel(i, 0, 0, 0);
    }
}

#[panic_handler]
fn panic(_panic: &PanicInfo) -> ! {
    unsafe {
        for i in 0..PIXEL_COUNT {
            set_pixel(i, 255, 255, 255);
        }
    }

    loop {}
}

extern "C" {
    fn show_pixels();
    fn delay_mic(mics: u32);
    fn set_pixel(index: u8, r: u8, g: u8, b: u8);
}

struct Resources {
    /// General purpose timer 1.
    pub gpt1: hal::gpt::Gpt1,

    /// Clock control module.
    pub ccm: ral::ccm::CCM,

    /// The FlexIO2 register block.
    pub flexio: ral::flexio::FLEXIO,
}

// struct Pins {
//     p1: iomuxc::imxrt1010::gpio_ad::GPIO_AD_00,
//     p2: iomuxc::imxrt1010::gpio_ad::GPIO_AD_01,
//     p3: iomuxc::imxrt1010::gpio_ad::GPIO_AD_02,
// }
//
// const fn pins_from_pads(pads: Pads) -> Pins {
//     Pins {
//         p1: pads.gpio_ad.p00,
//         p2: pads.gpio_ad.p01,
//         p3: pads.gpio_ad.p02,
//     }
// }


#[no_mangle]
pub unsafe extern "C" fn rust_main() {
    let board::Resources {
        gpt1: mut us_timer,
        ccm,
        flexio,
        pins,
    } = board::duo(board::instances());

    us_timer.set_clock_source(hal::gpt::ClockSource::PeripheralClock);
    us_timer.set_divider(1);
    us_timer.set_mode(hal::gpt::Mode::FreeRunning);
    us_timer.enable();
    let time_us = move || us_timer.count();

    // Set FlexIO clock to 16Mhz, as required by the driver
    ral::modify_reg!(
        ral::ccm,
        ccm,
        CS1CDR,
        FLEXIO1_CLK_PRED: FLEXIO1_CLK_PRED_4,
        FLEXIO1_CLK_PODF: DIVIDE_6,
    );

    // let pins = pins_from_pads(Pads::new());
    // let pads = Pads::new();
    // let xxx = (pads.gpio_ad.p00, pads.gpio_ad.p01, pads.gpio_ad.p02);
    // let mut neopixel = WS2812Driver::init(flexio, xxx).unwrap();
    // let erased = (pins.p8.erase(), pins.p9.erase(), pins.p10.erase());
    let mut neopixel = WS2812Driver2::init(flexio, pins).unwrap();

    let mut count = 0;
    loop {
        count += 1;
        unsafe {
            clear();
            set_pixel(count % PIXEL_COUNT, 255, 0, 255);
            show_pixels();
            delay_mic(100000);
        }
    }
}
