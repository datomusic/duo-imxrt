#!/bin/bash

# Configuration
TARGET_DIR="$HOME/Documents/duo-imxrt"
export DUO_FIRMWARE_VERSION="2.4.0"
FIRMWARE="releases/duo-imxrt_resistive_$DUO_FIRMWARE_VERSION.bin"

# Navigate
if [ ! -d "$TARGET_DIR" ]; then
    echo "Error: Directory $TARGET_DIR not found."
    exit 1
fi
cd "$TARGET_DIR/tools/updater" || exit

# Verify the firmware named by DUO_FIRMWARE_VERSION actually exists, so a
# version bump that misses a build fails here rather than halfway through
# a bench session.
if [ ! -f "$FIRMWARE" ]; then
    echo "Error: Firmware $FIRMWARE not found in $(pwd)."
    echo "Check DUO_FIRMWARE_VERSION at the top of this script."
    exit 1
fi

header() {
    clear
    echo "DUO Firmware Tool ($DUO_FIRMWARE_VERSION)"
}

# Main Loop
header

while true; do
    # The updater polls for boards itself and returns on Ctrl-C.
    uv run update_firmware.py -f "$FIRMWARE"

    echo ""
    read -s -p "Session ended. Press ENTER to restart, or Q to quit" -n 1 key
    echo ""

    if [[ $key == "q" || $key == "Q" ]]; then
        break
    fi

    header
done

# Close Terminal Window
osascript -e 'tell application "Terminal" to close first window' & exit
