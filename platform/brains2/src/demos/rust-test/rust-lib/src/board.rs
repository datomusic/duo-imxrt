use crate::{duopins, hal, iomuxc, ral};
use core::sync::atomic::{AtomicBool, Ordering};

use ral::Instances;

#[non_exhaustive]
pub struct Resources<Pins> {
    pub gpt1: hal::gpt::Gpt1,
    /// Clock control module.
    pub ccm: ral::ccm::CCM,
    /// All available pins.
    pub pins: Pins,
    /// The FlexIO1 register block.
    pub flexio: ral::flexio::FLEXIO,
}

pub type DuoResources = Resources<duopins::DuoPins>;

pub fn duo(instances: impl Into<Instances>) -> DuoResources {
    prepare_resources(instances.into(), duopins::from_pads)
}

fn prepare_resources<Pins>(
    mut instances: Instances,
    from_pads: impl FnOnce(iomuxc::Pads) -> Pins,
) -> Resources<Pins> {
    // let iomuxc = hal::iomuxc::into_pads(instances.IOMUXC);
    unsafe {
        let iomuxc = iomuxc::Pads::new();
        let pins = from_pads(iomuxc);

        // Stop timers in debug mode.
        ral::modify_reg!(ral::pit, instances.PIT, MCR, FRZ: FRZ_1);
        let pit = hal::pit::new(instances.PIT);
        //
        let mut gpt1 = hal::gpt::Gpt::new(instances.GPT1);
        gpt1.disable();

        Resources {
            gpt1,
            pins,
            ccm: instances.CCM,
            flexio: instances.FLEXIO1,
        }
    }
}

pub fn instances() -> Instances {
    static TAKEN: AtomicBool = AtomicBool::new(false);
    assert!(!TAKEN.swap(true, Ordering::SeqCst));
    unsafe { Instances::instances() }
}
