# Copyright (C) 2014 The CyanogenMod Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#
# This file sets variables that control the way modules are built
# throughout the system. It should not be used to conditionally
# disable makefiles (the proper mechanism to control what gets
# included in a build is to use PRODUCT_PACKAGES in a product
# definition file).
#

COMMON_PATH := device/samsung/expressltexx

# Inherit from common msm8930

-include device/samsung/msm8930-common/BoardConfigCommon.mk

# Includes
TARGET_SPECIFIC_HEADER_PATH += $(COMMON_PATH)/include

# The first api level the device has commercially launched on
PRODUCT_SHIPPING_API_LEVEL := 19

# Manifests
DEVICE_FRAMEWORK_MANIFEST_FILE := system/libhidl/vintfdata/manifest_healthd_exclude.xml
DEVICE_MANIFEST_FILE += $(COMMON_PATH)/manifest.xml

# Set default locale
PRODUCT_LOCALES := en-GB

# Kernel
BOARD_KERNEL_CMDLINE := androidboot.hardware=qcom user_debug=22 zcache msm_rtb.filter=0x3F ehci-hcd.park=3 androidboot.bootdevice=msm_sdcc.1
BOARD_KERNEL_BASE := 0x80200000
BOARD_KERNEL_IMAGE_NAME := zImage
BOARD_MKBOOTIMG_ARGS := --ramdisk_offset 0x02000000
BOARD_KERNEL_PAGESIZE := 2048
TARGET_KERNEL_SOURCE := kernel/samsung/msm8930-common
TARGET_KERNEL_CONFIG := samsung_express_defconfig
TARGET_KERNEL_VARIANT_CONFIG := msm8930_express_eur_lte_defconfig

# Bootloader
TARGET_BOOTLOADER_BOARD_NAME := MSM8960

# Assert
TARGET_OTA_ASSERT_DEVICE := expressltexx,expresslte,GT-I8730,GT-I8730T

# Recovery
LZMA_RAMDISK_TARGETS := recovery
TARGET_RECOVERY_FSTAB := $(COMMON_PATH)/rootdir/fstab.qcom

# Lights
TARGET_PROVIDES_LIBLIGHT := true

# Filesystem
TARGET_FS_CONFIG_GEN := $(COMMON_PATH)/config.fs
TARGET_USERIMAGES_USE_EXT4 := true
TARGET_USERIMAGES_USE_F2FS := true
BOARD_BOOTIMAGE_PARTITION_SIZE := 10485760
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 10485760
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 1415577600
BOARD_SYSTEMIMAGE_JOURNAL_SIZE := 0
BOARD_USERDATAIMAGE_PARTITION_SIZE := 5079285760
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_CACHEIMAGE_PARTITION_SIZE := 880803840
BOARD_FLASH_BLOCK_SIZE := 131072
BOARD_ROOT_EXTRA_FOLDERS := efs firmware persist
BOARD_ROOT_EXTRA_SYMLINKS := /data/tombstones:/tombstones

# Bluetooth
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := $(COMMON_PATH)/bluetooth

# Charger
BOARD_NO_CHARGER_LED := true

# Needed for LPA
BOARD_GLOBAL_CFLAGS += -DLPA_DEFAULT_BUFFER_SIZE=32

# Audio
USE_XML_AUDIO_POLICY_CONF := 1

# FM
AUDIO_FEATURE_ENABLED_FM_POWER_OPT := true
BOARD_HAVE_QCOM_FM := true
TARGET_FM_LEGACY_PATCHLOADER := true

# Charger
BOARD_CHARGER_ENABLE_SUSPEND := true

# Camera
TARGET_NEED_DISABLE_AUTOFOCUS := true
TARGET_NEED_DISABLE_FACE_DETECTION := true
TARGET_NEED_DISABLE_FACE_DETECTION_BOTH_CAMERAS := true

# RIL
BOARD_PROVIDES_LIBRIL := true
TARGET_RIL_VARIANT := caf

# Legacy blobs
TARGET_PROCESS_SDK_VERSION_OVERRIDE += \
    /system/vendor/bin/hw/rild=27

# Network Routing
TARGET_NEEDS_NETD_DIRECT_CONNECT_RULE := true

# Vendor init
TARGET_INIT_VENDOR_LIB := libinit_serrano
TARGET_RECOVERY_DEVICE_MODULES := libinit_serrano

ifeq ($(WITH_TWRP),true)
-include $(COMMON_PATH)/twrp.mk
endif