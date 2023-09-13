#![no_main]
#![no_std]

mod board;
mod duopins;
mod effects;

use core::panic::PanicInfo;

mod iomuxc {
    pub use imxrt_iomuxc::imxrt1010::*;
}

use imxrt_ral as ral;
use palette::{LinSrgb, Srgb};
use ws2812_flexio::{IntoPixelStream, WS2812Driver};

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
    fn delay_mic(mics: u32);
    fn flash_led();
}

static mut PIXEL_BUFFER: [Srgb; NUM_PIXELS] = [Srgb::new(0., 0., 0.); NUM_PIXELS];

fn linearize_color(col: &Srgb) -> LinSrgb<u8> {
    col.into_linear().into_format()
}

#[no_mangle]
pub extern "C" fn rust_main() {
    let board::Resources {
        ccm, flexio, pins, ..
    } = board::duo(board::instances());

    // Set FlexIO clock to 16Mhz, as required by the driver
    ral::modify_reg!(
        ral::ccm,
        ccm,
        CS1CDR,
        FLEXIO1_CLK_PRED: FLEXIO1_CLK_PRED_4,
        FLEXIO1_CLK_PODF: DIVIDE_6,
    );

    let mut neopixel = WS2812Driver::init(flexio, (pins.led_pin,)).unwrap();
    unsafe { flash_led() };

    let framebuffer = unsafe { &mut PIXEL_BUFFER };

    let mut t = 1;
    loop {
        t += 1;

        // effects::running_dots(t, framebuffer_0);
        effects::rainbow(t, framebuffer);
        // effects::test_pattern(framebuffer_2);
        //

        neopixel.write([&mut framebuffer.iter().map(linearize_color).into_pixel_stream()]);

        unsafe {
            // flash_led();
            delay_mic(10000);
        }
    }
}
