#!/bin/bash

# Configuration
export DUO_FIRMWARE_VERSION="2.4.0"
FIRMWARE="releases/duo-imxrt_resistive_$DUO_FIRMWARE_VERSION.bin"

# Any failure before the updater starts must hold the window open, otherwise
# a double-click just flashes a Terminal window and closes it.
die() {
    echo "Error: $1" >&2
    shift
    for line in "$@"; do echo "$line" >&2; done
    echo ""
    read -s -n 1 -p "Press any key to close this window."
    echo ""
    exit 1
}

# This script lives in the repo it needs, so locate it from its own path
# rather than a hardcoded checkout location.
cd "$(dirname "${BASH_SOURCE[0]}")" || die "Could not enter $(dirname "${BASH_SOURCE[0]}")."

command -v uv >/dev/null || die "uv is not installed or not on PATH." \
    "Install it from https://docs.astral.sh/uv/"

# Verify the firmware named by DUO_FIRMWARE_VERSION actually exists, so a
# version bump that misses a build fails here rather than halfway through
# a bench session.
[ -f "$FIRMWARE" ] || die "Firmware $FIRMWARE not found in $(pwd)." \
    "Check DUO_FIRMWARE_VERSION at the top of this script."

clear
echo "DUO Firmware Tool ($DUO_FIRMWARE_VERSION)"

# The updater polls for boards itself and returns on Ctrl-C. When it exits,
# the session is over -- relaunch by double-clicking the icon again.
uv run update_firmware.py --factory "$FIRMWARE"

# Hold the window open so the final tally (or a traceback) stays readable.
# The Ctrl-C that ends the updater reaches this script too; a no-op handler
# keeps it from tripping the read below. Must stay after the uv run line --
# children inherit it as a default disposition, but never as an ignore.
trap ':' INT
echo ""
read -s -n 1 -p "Session ended. Press any key to close this window."
echo ""

# Close Terminal Window
osascript -e 'tell application "Terminal" to close first window' & exit
