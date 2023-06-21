#
# Copyright (C) 2019 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

set -e

INITIAL_COPYRIGHT_YEAR=2013
USE_CAMERA_STUB := false
EOF

# Load extract_utils and do some sanity checks
MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$MY_DIR" ]]; then MY_DIR="$PWD"; fi

# Initialize the helper for common
setup_vendor "$DEVICE_COMMON" "$VENDOR" "$LINEAGE_ROOT" true

# Copyright headers and guards
write_headers "i8730 expresslte expressltexx"

# We are done!
write_footers

if [ -s "$MY_DIR"/../$DEVICE/proprietary-files.txt ]; then
    # Reinitialize the helper for device
    INITIAL_COPYRIGHT_YEAR="$DEVICE_BRINGUP_YEAR"
    setup_vendor "$DEVICE" "$VENDOR" "$LINEAGE_ROOT" false
	
	# Copyright headers and guards
    write_headers
	
	
	# The standard device blobs
    write_makefiles "$MY_DIR"/../$DEVICE/proprietary-files.txt true

	# We are done!
    write_footers
fi