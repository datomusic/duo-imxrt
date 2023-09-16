#![no_std]
#![no_main]

mod panic_handler;
mod iomuxc {
    pub use imxrt_iomuxc::imxrt1010::*;
    pub use imxrt_iomuxc::ErasedPad;
    pub use imxrt_iomuxc::Pad;
}

use core::panic::PanicInfo;
use imxrt_hal as hal;
use imxrt_ral as ral;
use palette::LinSrgb;
use palette::Srgb;
mod effects;
mod flexio;
mod pins;
mod pixel;
mod pixelstream;

/// Possible errors that could happen.
pub mod errors;

mod board;
mod duopins;

pub use flexio::WS2812Driver;
pub use pins::Pins;
use pixelstream::IntoPixelStream;

const NUM_PIXELS: usize = 19 as usize;
static mut PIXEL_BUFFER: [Srgb; NUM_PIXELS] = [Srgb::new(0., 0., 0.); NUM_PIXELS];
static mut PIXS: [Srgb; NUM_PIXELS] = [Srgb::new(0., 0., 0.); NUM_PIXELS];

fn build_pix_buffer(rgb_bytes: &[u8]) -> [Srgb; NUM_PIXELS] {
    let mut buf = [Srgb::new(0., 0., 0.); NUM_PIXELS];

    for i in 0..(rgb_bytes.len() / 3) {
        if i >= NUM_PIXELS {
            break;
        } else {
            let ind = i * 3;
            let r = rgb_bytes[ind];
            let g = rgb_bytes[ind + 1];
            let b = rgb_bytes[ind + 2];
            buf[i].red = r as f32;
            buf[i].green = g as f32;
            buf[i].blue = b as f32;
        }
    }
    return buf;
}

#[no_mangle]
pub extern "C" fn show_pixels(size: u32, array_pointer: *const u8) {
    unsafe {
        let rgb_bytes = core::slice::from_raw_parts(array_pointer as *const u8, size as usize);
        PIXS = build_pix_buffer(rgb_bytes);
    };
}

extern "C" {
    fn delay_mic(mics: u32);
    fn flash_led();
    fn write_data();
}

fn linearize_color(col: &Srgb) -> LinSrgb<u8> {
    col.into_linear().into_format()
}

#[no_mangle]
pub extern "C" fn init_neopixel() {
    let board::Resources {
        ccm, flexio, pins, ..
    } = board::duo(board::instances());
    WS2812Driver::init(flexio, (pins.led_pin,)).unwrap();
}
