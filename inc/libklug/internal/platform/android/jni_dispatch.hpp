/**
 * JNI Result dispatcher
 *
 * \file    inc/libklug/internal/platform/android/jni_dispatch.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#include <jni.h>
#include "libklug/result/result.hpp"

namespace res {

jobject jni_dispatch(JNIEnv* env, Result const& r);

} // namespace res
