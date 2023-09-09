#![no_main]
#![no_std]

use core::panic::PanicInfo;

#[panic_handler]
fn panic(_panic: &PanicInfo) -> ! {
    loop {}
}

#[no_mangle]
pub extern "C" fn rtest(counter: i32) -> i32 {
    const PIXEL_COUNT: i32 = 19;
    if counter >= PIXEL_COUNT {
        1
    } else {
        0
    }
}
