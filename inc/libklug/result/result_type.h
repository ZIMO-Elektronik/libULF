/**
 * Result Type
 *
 * \file    inc/libklug/result/result_type.h
 * \author  Jonas Gahlert
 * \date    06.05.2026
 */

#pragma once

/**
 * Result Type
 *
 */
typedef enum {
  status, ///< Success = 1, Error = 0
  cv,     ///< Cv value
  string, ///< String
  error,  ///< General error
} result_type;
