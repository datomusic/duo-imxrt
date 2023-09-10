#![no_main]
#![no_std]

mod iomuxc {
    pub use imxrt_iomuxc::imxrt1010::*;
    pub use imxrt_iomuxc::ErasedPad;
    pub use imxrt_iomuxc::Pad;
}

use core::panic::PanicInfo;
use palette::LinSrgb;
use palette::Srgb;
// use hal::iomuxc::flexio::*;
use imxrt_hal as hal;
use imxrt_ral as ral;
mod effects;
mod flexio;
mod pins;
mod pixel;
mod pixelstream;

/// Possible errors that could happen.
pub mod errors;

mod board;
mod duopins;

// pub use flexio::{PreprocessedPixels, WS2812Driver, WriteDmaResult};
pub use flexio::WS2812Driver;
pub use pins::Pins;
use pixelstream::IntoPixelStream;
// pub use pixel::Pixel;
// pub use pixelstream::IntoPixelStream;

// use imxrt_iomuxc as iomuxc;

const PIXEL_COUNT: u8 = 19;
const NUM_PIXELS: usize = PIXEL_COUNT as usize;

#[panic_handler]
fn panic(_panic: &PanicInfo) -> ! {
    loop {
        unsafe {
            delay_mic(1000000);
        }
    }
}

extern "C" {
    //     fn show_pixels();
    fn delay_mic(mics: u32);
    //     fn set_pixel(index: u8, r: u8, g: u8, b: u8);
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

// static mut FRAMEBUFFER_0: [Srgb; NUM_PIXELS] = [Srgb::new(0., 0., 0.); NUM_PIXELS];
// static mut FRAMEBUFFER_1: [Srgb; NUM_PIXELS] = [Srgb::new(0., 0., 0.); NUM_PIXELS];
static mut FRAMEBUFFER_2: [[u8; 3]; NUM_PIXELS] = [[0; 3]; NUM_PIXELS];

fn linearize_color(col: &Srgb) -> LinSrgb<u8> {
    col.into_linear().into_format()
}

#[no_mangle]
pub extern "C" fn rust_main() {
    let board::Resources {
        // gpt1: mut gpt1,
        ccm,
        flexio,
        pins,
        ..
    } = board::duo(board::instances());

    // Set FlexIO clock to 16Mhz, as required by the driver
    // ral::modify_reg!(
    //     ral::ccm,
    //     ccm,
    //     CS1CDR,
    //     FLEXIO1_CLK_PRED: FLEXIO1_CLK_PRED_4,
    //     FLEXIO1_CLK_PODF: DIVIDE_6,
    // );

    // let mut neopixel = WS2812Driver::init(flexio, (pins.led_pin,)).unwrap();

    // let framebuffer_0 = unsafe { &mut FRAMEBUFFER_0 };
    // let framebuffer_1 = unsafe { &mut FRAMEBUFFER_1 };
    // let framebuffer_2 = unsafe { &mut FRAMEBUFFER_2 };

    loop {
        // effects::test_pattern(framebuffer_2);
        // t += 1;

        // neopixel.write([
        //     // &mut framebuffer_0
        //     //     .iter()
        //     //     .map(linearize_color)
        //     //     .into_pixel_stream(),
        //     // &mut framebuffer_1
        //     //     .iter()
        //     //     .map(linearize_color)
        //     //     .into_pixel_stream(),
        //     &mut framebuffer_2.into_pixel_stream(),
        // ]);

        unsafe {
            delay_mic(1000);
        }
    }
}
