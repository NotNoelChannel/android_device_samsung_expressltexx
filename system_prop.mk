# RIL
PRODUCT_PROPERTY_OVERRIDES += \
    rild.libpath=/system/vendor/lib/libril-qc-qmi-1.so \
    ro.telephony.default_network=3 \
    ro.com.android.mobiledata=false \
    ro.ril.telephony.qan_resp_strings=6 \
    persist.data.qmi.adb_logmask=0 \
    persist.radio.add_power_save=1

# GPS
PRODUCT_PROPERTY_OVERRIDES += \
    ro.gps.agps_provider=1

# Media
PRODUCT_PROPERTY_OVERRIDES += \
    audio.offload.disable=1 \
    vendor.dedicated.device.for.voip=true \
    vendor.voice.path.for.pcm.voip=true \
	ro.vendor.fm.use_audio_session=true \
    media.aac_51_output_enabled=true

# Graphics
PRODUCT_PROPERTY_OVERRIDES += \
    debug.composition.type=c2d \
    debug.hwui.use_buffer_age=false \
    persist.debug.wfd.enable=1 \
    persist.sys.wfd.virtual=0 \
    ro.sf.lcd_density=240 \
    ro.opengles.version=196608 \
    ro.qualcomm.cabl=0 \
    ro.hwui.text_large_cache_height=1024 \

# Camera
PRODUCT_PROPERTY_OVERRIDES += \
    camera2.portability.force_api=1 \
    camera.disable_zsl_mode=1

# Bluetooth
PRODUCT_PROPERTY_OVERRIDES += \
    ro.bluetooth.dun=true \
    ro.bluetooth.sap=true \
    ro.bluetooth.hfp.ver=1.6 \
    ro.bt.bdaddr_path=/efs/bluetooth/bt_addr \
    ro.qualcomm.bt.hci_transport=smd

# Misc
PRODUCT_PROPERTY_OVERRIDES += \
    ro.chipname=MSM8930 \
	wifi.direct.interface=p2p0 \
    wifi.interface=wlan0

    
# QC Perf
PRODUCT_PROPERTY_OVERRIDES += \
    ro.vendor.extension_library=libqti-perfd-client.so

# Dalvik
PRODUCT_PROPERTY_OVERRIDES += \
    dalvik.vm.dex2oat-swap=false

# Vendor security patch level
PRODUCT_PROPERTY_OVERRIDES += \
    ro.lineage.build.vendor_security_patch=2016-08-01