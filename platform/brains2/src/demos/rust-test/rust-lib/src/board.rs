use crate::{duopins, iomuxc, ral};
use core::sync::atomic::{AtomicBool, Ordering};

use ral::Instances;

#[non_exhaustive]
pub struct Resources<Pins> {
    /// Clock control module.
    pub ccm: ral::ccm::CCM,
    /// All available pins.
    pub pins: Pins,
    /// The FlexIO register block.
    pub flexio: ral::flexio::FLEXIO,
}

pub type DuoResources = Resources<duopins::DuoPins>;

pub fn duo(instances: impl Into<Instances>) -> DuoResources {
    prepare_resources(instances.into(), duopins::from_pads)
}

fn prepare_resources<Pins>(
    instances: Instances,
    from_pads: impl FnOnce(iomuxc::Pads) -> Pins,
) -> Resources<Pins> {
    unsafe {
        let iomuxc = iomuxc::Pads::new();
        let pins = from_pads(iomuxc);

        Resources {
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
