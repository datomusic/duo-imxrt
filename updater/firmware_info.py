import usb
from typing import NamedTuple

class FirmwareInfo(NamedTuple):
    tag: str
    branch: str
    commit: str

def get_firmware_info():
    dev = usb.core.find(idVendor=0x16d0, idProduct=0x10a7)
    if dev is None:
        print('No DUO connected.')
        return None
    else:
        return FirmwareInfo(
                tag = usb.util.get_string(dev, 4),
                branch = usb.util.get_string(dev, 5),
                commit = usb.util.get_string(dev, 6))

if __name__ == "__main__":
    info = get_firmware_info()
    for k, v in info._asdict().items():
        print(f"{k}: {v}")
