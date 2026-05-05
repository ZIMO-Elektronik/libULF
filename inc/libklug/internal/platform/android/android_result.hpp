#pragma once

#include <jni.h>
#include "libklug/result.hpp"

jobject dispatch(JNIEnv* env, result_t const& r);
