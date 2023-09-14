#![no_std]
#![no_main]

mod board;
mod duopins;
mod effects;
mod panic_handler;

mod iomuxc {
    pub use imxrt_iomuxc::imxrt1010::*;
}

use imxrt_ral as ral;
use palette::{LinSrgb, Srgb};
use ws2812_flexio::{IntoPixelStream, PreprocessedPixels, WS2812Driver};

const NUM_PIXELS: usize = 19 as usize;
static mut PIXEL_BUFFER: [Srgb; NUM_PIXELS] = [Srgb::new(0., 0., 0.); NUM_PIXELS];
static mut PIXS: [Srgb; NUM_PIXELS] = [Srgb::new(0., 0., 0.); NUM_PIXELS];

static mut BUFFERS: (
    PreprocessedPixels<NUM_PIXELS, 1>,
    PreprocessedPixels<NUM_PIXELS, 1>,
) = (PreprocessedPixels::new(), PreprocessedPixels::new());

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
pub extern "C" fn show_pixels_bytes(size: u32, array_pointer: *const u8) {
    unsafe {
        let rgb_bytes = core::slice::from_raw_parts(array_pointer as *const u8, size as usize);
        PIXS = build_pix_buffer(rgb_bytes);
    };
}

extern "C" {
    fn c_update_callback();
    fn yes_interrupts();
    fn no_interrupts();
    fn start_dac();

}

fn linearize_color(col: &Srgb) -> LinSrgb<u8> {
    col.into_linear().into_format()
}

#[no_mangle]
pub extern "C" fn rust_main() {
    let board::Resources {
        ccm,
        flexio,
        pins,
        mut dma,
        ..
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
    let mut neopixel_dma = dma[1].take().unwrap();

    let framebuffer = unsafe { &mut PIXEL_BUFFER };
    let pixs_buffer = unsafe { &mut PIXS };

    let mut t = 0;
    let buffers = unsafe { &mut BUFFERS };
    let mut flip_buffers = false;
    let mut x = 0;

    unsafe {
        start_dac();
    }

    loop {
        if x > 10 {
            x = 0;
            t += 1;
        } else {
            x += 1;
        }

        let (render_buffer, display_buffer) = if flip_buffers {
            (&mut buffers.0, &buffers.1)
        } else {
            (&mut buffers.1, &buffers.0)
        };

        flip_buffers = !flip_buffers;

        // unsafe {
        //     no_interrupts();
        // }
        let lagged = neopixel
            .write_dma(display_buffer, &mut neopixel_dma, 0, || {
                t += 1;

                effects::rainbow(t, framebuffer);

                render_buffer.prepare_pixels([&mut framebuffer
                    .iter()
                    .map(linearize_color)
                    .into_pixel_stream()]);

                // render_buffer.prepare_pixels([&mut pixs_buffer.iter().map(linearize_color).into_pixel_stream()]);
            })
            .unwrap()
            .lagged;

        // unsafe {
        //     yes_interrupts();
        // }

        // unsafe {
        //     no_interrupts();
        // }
        // // neopixel.write([&mut framebuffer.iter().map(linearize_color).into_pixel_stream()]);
        // neopixel.write([&mut pixs_buffer.iter().map(linearize_color).into_pixel_stream()]);
        // unsafe {
        //     yes_interrupts();
        // }

        unsafe {
            c_update_callback();
        }
    }
}
