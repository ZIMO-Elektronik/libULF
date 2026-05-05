#pragma once

#include <jni.h>
#include "result.hpp"

jobject dispatch(JNIEnv* env, result_t const& r);
