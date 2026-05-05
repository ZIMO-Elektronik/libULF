/**
 * Callback
 *
 * \file    callback.h
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include "libklug/result.hpp"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Callback
 *
 * \todo This should be replaced by a functor, so we can handle platform
 * sepcific stuff (like jni)
 *
 */
typedef void (*bridge_callback)(result_t r);

// Callback registration
/// \todo Write registration

#ifdef __cplusplus
}
#endif
