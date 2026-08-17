# musin (vendored snapshot)

Vendored snapshot of the platform-neutral parts of musin from the
drum-firmware repository, branch `refactor-musin-filesystem-portability`
(commit efd8c4bb). Do not edit `filesystem/`, `settings/` or `hal/` here —
change them in drum-firmware and re-vendor, so the eventual switch to a
shared musin submodule stays a deletion.

`ports/imxrt/` is the DUO Brains 2 (MIMXRT1011) port and lives here for now;
it should move into musin proper once musin is extracted into its own
repository. It implements the `Filesystem` facade over littlefs
(`libraries/littlefs`) and the mflash driver
(`sdk/components/flash/mflash/mimxrt1011`), provides the `StorageRegion`
from linker-defined symbols, and wires the mounted filesystem into newlib so
POSIX stdio works, per the musin port contract.
