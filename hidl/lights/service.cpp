/*
 * Copyright (C) 2017 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "android.hardware.light@2.0-service.express"

#include <hidl/HidlTransportSupport.h>
#include <hwbinder/ProcessState.h>


#include "Light.h"

// libhwbinder:
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

// Generated HIDL files
using android::hardware::light::V2_0::ILight;
using android::hardware::light::V2_0::implementation::Light;

const static std::string kBacklightPath = "/sys/class/leds/lcd-backlight/brightness";
const static std::string kButtonlightPath = "/sys/class/leds/button-backlight/brightness";
const static std::string kIndicatorPath = "/sys/class/sec/led/led_blink";

int main() {
    android::hardware::ProcessState::initWithMmapSize((size_t)(32768));
    std::ofstream backlight(kBacklightPath);
    if (!backlight) {
        int error = errno;
        ALOGE("Failed to open %s (%d): %s", kBacklightPath.c_str(), error, strerror(error));
        return -error;
    }

    std::ofstream buttonlight(kButtonlightPath);
    if (!buttonlight) {
        ALOGI("Button lights not supported on this device");
    }

    std::ofstream indicator(kIndicatorPath);
    if (!indicator) {
        ALOGI("Indicator light not supported on this device");
    }

    android::sp<ILight> service = new Light(std::move(backlight), std::move(buttonlight), std::move(indicator));

    configureRpcThreadpool(1, true);

    android::status_t status = service->registerAsService();

    if (status != android::OK) {
        ALOGE("Cannot register Light HAL service");
        return 1;
    }

    ALOGI("Light HAL Ready.");
    joinRpcThreadpool();
    // Under normal cases, execution will not reach this line.
    ALOGE("Light HAL failed to join thread pool.");
    return 1;
}
