/*
 * Copyright (C) 2015 The CyanogenMod Project
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
#define LOG_TAG "PowerHAL"

#include <hardware/power.h>

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <log/log.h>

#include "power.h"

#define STATE_ON "state=1"

#define CPUFREQ_LIMIT_PATH "/sys/kernel/cpufreq_limit/"
#define INTERACTIVE_PATH "/sys/devices/system/cpu/cpufreq/interactive/"

static int boostpulse_fd = -1;

static int current_power_profile = -1;

static int sysfs_write_str(char* path, char* s) {
    char buf[80];
    int len;
    int ret = 0;
    int fd;

    fd = open(path, O_WRONLY);
    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);
        return -1;
    }

    len = write(fd, s, strlen(s));
    if (len < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error writing to %s: %s\n", path, buf);
        ret = -1;
    }

    close(fd);

    return ret;
}

static int sysfs_write_int(char* path, int value) {
    char buf[80];
    snprintf(buf, 80, "%d", value);
    return sysfs_write_str(path, buf);
}

static bool check_governor(void) {
    struct stat s;
    int err = stat(INTERACTIVE_PATH, &s);
    if (err != 0) return false;
    if (S_ISDIR(s.st_mode)) return true;
    return false;
}

static int is_profile_valid(int profile) {
    return profile >= 0 && profile < PROFILE_MAX;
}

void power_init(void) {
    ALOGI("%s", __func__);
}

static int boostpulse_open() {
    if (boostpulse_fd < 0) {
        boostpulse_fd = open(INTERACTIVE_PATH "boostpulse", O_WRONLY);
    }

    return boostpulse_fd;
}

void power_set_interactive(int on) {
    if (!is_profile_valid(current_power_profile)) {
        ALOGD("%s: no power profile selected yet", __func__);
        return;
    }

    // break out early if governor is not interactive
    if (!check_governor()) return;

    if (on) {
        sysfs_write_int(INTERACTIVE_PATH "hispeed_freq",
                        profiles[current_power_profile].hispeed_freq);
        sysfs_write_int(INTERACTIVE_PATH "go_hispeed_load",
                        profiles[current_power_profile].go_hispeed_load);
        sysfs_write_int(INTERACTIVE_PATH "timer_rate", profiles[current_power_profile].timer_rate);
        sysfs_write_str(INTERACTIVE_PATH "target_loads",
                        profiles[current_power_profile].target_loads);
    } else {
        sysfs_write_int(INTERACTIVE_PATH "hispeed_freq",
                        profiles[current_power_profile].hispeed_freq_off);
        sysfs_write_int(INTERACTIVE_PATH "timer_rate",
                        profiles[current_power_profile].timer_rate_off);
        sysfs_write_int(INTERACTIVE_PATH "go_hispeed_load",
                        profiles[current_power_profile].go_hispeed_load_off);
        sysfs_write_str(INTERACTIVE_PATH "target_loads",
                        profiles[current_power_profile].target_loads_off);
    }
}

static void set_power_profile(int profile) {
    if (!is_profile_valid(profile)) {
        ALOGE("%s: unknown profile: %d", __func__, profile);
        return;
    }

    // break out early if governor is not interactive
    if (!check_governor()) return;

    if (profile == current_power_profile) return;

    ALOGD("%s: setting profile %d", __func__, profile);

    sysfs_write_int(INTERACTIVE_PATH "boost", profiles[profile].boost);
    sysfs_write_int(INTERACTIVE_PATH "boostpulse_duration", profiles[profile].boostpulse_duration);
    sysfs_write_int(INTERACTIVE_PATH "go_hispeed_load", profiles[profile].go_hispeed_load);
    sysfs_write_int(INTERACTIVE_PATH "hispeed_freq", profiles[profile].hispeed_freq);
    sysfs_write_str(INTERACTIVE_PATH "above_hispeed_delay", profiles[profile].above_hispeed_delay);
    sysfs_write_int(INTERACTIVE_PATH "timer_rate", profiles[profile].timer_rate);
    sysfs_write_int(INTERACTIVE_PATH "io_is_busy", profiles[profile].io_is_busy);
    sysfs_write_int(INTERACTIVE_PATH "min_sample_time", profiles[profile].min_sample_time);
    sysfs_write_int(INTERACTIVE_PATH "max_freq_hysteresis", profiles[profile].max_freq_hysteresis);
    sysfs_write_str(INTERACTIVE_PATH "target_loads", profiles[profile].target_loads);
    sysfs_write_int(CPUFREQ_LIMIT_PATH "limited_min_freq", profiles[profile].limited_min_freq);
    sysfs_write_int(CPUFREQ_LIMIT_PATH "limited_max_freq", profiles[profile].limited_max_freq);

    current_power_profile = profile;
}

static void process_video_encode_hint(void* metadata) {
    int on;

    if (!metadata) return;

    /* Break out early if governor is not interactive */
    if (!check_governor()) return;

    on = !strncmp(metadata, STATE_ON, sizeof(STATE_ON));

    sysfs_write_int(INTERACTIVE_PATH "timer_rate",
                    on ? VID_ENC_TIMER_RATE : profiles[current_power_profile].timer_rate);

    sysfs_write_int(INTERACTIVE_PATH "io_is_busy",
                    on ? VID_ENC_IO_IS_BUSY : profiles[current_power_profile].io_is_busy);
}

void power_hint(power_hint_t hint, void* data) {
    char buf[80];
    int len;

    if (hint == POWER_HINT_SET_PROFILE) {
        set_power_profile(*(int32_t *)data);
        return;
    }

    // Skip other hints in powersave mode
    if (current_power_profile == PROFILE_POWER_SAVE)
        return;

    switch (hint) {
        case POWER_HINT_INTERACTION:
        case POWER_HINT_LAUNCH:
            if (!is_profile_valid(current_power_profile)) {
                ALOGD("%s: no power profile selected yet", __func__);
                return;
            }

            if (!profiles[current_power_profile].boostpulse_duration) return;

            // break out early if governor is not interactive
            if (!check_governor()) return;

            if (boostpulse_open() >= 0) {
                snprintf(buf, sizeof(buf), "%d", 1);
                len = write(boostpulse_fd, &buf, sizeof(buf));
                if (len < 0) {
                    strerror_r(errno, buf, sizeof(buf));
                    ALOGE("Error writing to boostpulse: %s\n", buf);

                    close(boostpulse_fd);
                    boostpulse_fd = -1;
                }
            }
            break;
        case POWER_HINT_VIDEO_ENCODE:
            process_video_encode_hint(data);
            break;
        default:
            break;
    }
}

int get_number_of_profiles()
{
    return PROFILE_MAX;
}
