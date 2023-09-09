#![no_main]
#![no_std]

use core::panic::PanicInfo;
// use ws2812_flexio::{IntoPixelStream, WS2812Driver};

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

#[no_mangle]
pub extern "C" fn rust_main() {
    // let mut neopixel = WS2812Driver::init(flexio2, (pins.p6, pins.p7, pins.p8)).unwrap();

    let mut count = 0;
    let mut fast = false;

    loop {
        count += 1;
        if count > PIXEL_COUNT {
            fast = !fast;
            count = 0;
        }

        unsafe {
            clear();
            set_pixel(count, 255, 0, 255);
            show_pixels();
            if fast {
                delay_mic(10000);
            } else {
                delay_mic(100000);
            }
        }
    }
}
