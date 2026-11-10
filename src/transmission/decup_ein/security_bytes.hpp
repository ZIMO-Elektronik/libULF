/**
 * Copyright (C) 2026 ZIMO Elektronik
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://gnu.org>.
 *
 *
 *
 *
 *
 * DECUP_EIN security bytes transmission
 *
 * \file    src/transmission/decup_ein/security_bytes.hpp
 * \author  Jonas Gahlert
 * \date    10.11.2026
 */

#pragma once

#include "base.hpp"

namespace transmission::decup_ein {

struct SecurityBytes : ITransmission {
  SecurityBytes(std::shared_ptr<internal::IConnection> conn,
                std::size_t timeout);
  virtual ~SecurityBytes() = default;

  void execute() final;

  std::string evaluateString() final;
  bool evaluateBool() final;
  int evaluateValue() final;

private:
  std::shared_ptr<internal::IConnection> _conn;
  std::size_t _timeout;

  bool _first_success{};
  bool _second_success{};
};

} // namespace transmission::decup_ein
