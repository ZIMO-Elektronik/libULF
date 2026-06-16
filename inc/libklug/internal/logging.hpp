/**
 * Logging
 *
 * \file    inc/libklug/internal/logging.hpp
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

#define LOG_TAG "NativeKLUG"

#define LOGGINGING

#ifdef ANDROID
#  include <android/log.h>
#  include <iostream>

#  define LOGD(...)                                                            \
    __android_log_print(                                                       \
      ANDROID_LOG_DEBUG, LOG_TAG, "%s", std::format(__VA_ARGS__).c_str())
#  define LOGE(...)                                                            \
    __android_log_print(                                                       \
      ANDROID_LOG_ERROR, LOG_TAG, "%s", std::format(__VA_ARGS__).c_str())

#else
#  ifdef LOGGINGING
#    include <format>
#    include <iostream>

#    define LOGD(...)                                                          \
      std::cout << "[DEBUG] " << std::format(__VA_ARGS__) << std::endl;
#    define LOGE(...)                                                          \
      std::cout << "[ERROR] " << std::format(__VA_ARGS__) << std::endl;
#  else
#    include <iostream>
#    define LOGD(...)
#    define LOGE(...)
#  endif
#endif
