# Installation
To install necessary depencencies:
`python -m pip install -r requirements.txt`

# Usage
`python update_firmware.py [firmware-file-path]`

If `firmware-file-path` is not supplied, the updater defaults to `duo_firmware.bin` in the current directory.


# Issues
On some Linux systems, firmware update might in way that seems like the updater cannot find the Duo USB device.\
This is most probably a permission issue, and can be solved either by running the command with `sudo` (quick fix, but not the best), or by making sure the user has permission to access relevant USB block device (more difficult and system dependent).
