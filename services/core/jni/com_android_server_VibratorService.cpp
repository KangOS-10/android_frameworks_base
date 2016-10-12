/*
 * Copyright (C) 2009 The Android Open Source Project
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

#define LOG_TAG "VibratorService"

#include <android/hardware/vibrator/1.0/IVibrator.h>
#include <android/hardware/vibrator/1.0/types.h>

#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"

#include <utils/misc.h>
#include <utils/Log.h>
#include <hardware/vibrator.h>

#include <stdio.h>

using android::hardware::getService;
using android::hardware::vibrator::V1_0::IVibrator;
using android::hardware::vibrator::V1_0::Status;

namespace android
{

static sp<IVibrator> mHal;

static void vibratorInit(JNIEnv /* env */, jobject /* clazz */)
{
    /* TODO(b/31632518) */
    if (mHal != nullptr) {
        return;
    }
    mHal = IVibrator::getService("vibrator");
}

static jboolean vibratorExists(JNIEnv* /* env */, jobject /* clazz */)
{
    if (mHal != nullptr) {
        return JNI_TRUE;
    } else {
        return JNI_FALSE;
    }
}

static void vibratorOn(JNIEnv* /* env */, jobject /* clazz */, jlong timeout_ms)
{
    if (mHal != nullptr) {
        Status retStatus = mHal->on(timeout_ms);
        if (retStatus == Status::ERR) {
            ALOGE("vibratorOn command failed.");
        }
    } else {
        ALOGW("Tried to vibrate but there is no vibrator device.");
    }
}

static void vibratorOff(JNIEnv* /* env */, jobject /* clazz */)
{
    if (mHal != nullptr) {
        Status retStatus = mHal->off();
        if (retStatus == Status::ERR) {
            ALOGE("vibratorOff command failed.");
        }
    } else {
        ALOGW("Tried to stop vibrating but there is no vibrator device.");
    }
}

static const JNINativeMethod method_table[] = {
    { "vibratorExists", "()Z", (void*)vibratorExists },
    { "vibratorInit", "()V", (void*)vibratorInit },
    { "vibratorOn", "(J)V", (void*)vibratorOn },
    { "vibratorOff", "()V", (void*)vibratorOff }
};

int register_android_server_VibratorService(JNIEnv *env)
{
    return jniRegisterNativeMethods(env, "com/android/server/VibratorService",
            method_table, NELEM(method_table));
}

};
