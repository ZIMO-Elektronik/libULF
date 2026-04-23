#pragma once

#define LOG_TAG "NativeKLUG"

#ifdef ANDROID
#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#else
#include <iostream>

#define LOGD(...)                                                              \
  std::cout << "[DEBUG] " << std::format(__VA_ARGS__) << std::endl;
#define LOGE(...)                                                              \
  std::cout << "[ERROR] " << std::format(__VA_ARGS__) << std::endl;

#endif