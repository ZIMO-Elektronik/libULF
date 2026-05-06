#pragma once

#include <jni.h>
#include "libklug/result/result.hpp"

namespace res {

jobject jni_dispatch(JNIEnv* env, Result const& r);

} // namespace res
