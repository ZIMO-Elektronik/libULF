#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ulf/c/libulf.h>
#include <bridge/bridge.hpp>
#include <memory>
#include <ulf/cpp/ulf_error.hpp>
#include "mock_connection.hpp"

using testing::_;
using testing::InSequence;
using testing::NiceMock;
using testing::Return;
using testing::Throw;

struct TestBase : public testing::Test {
  TestBase()
    : p_conn{std::make_shared<NiceMock<MockConnection>>()}, lib{p_conn},
      conn{*p_conn} {}

  std::shared_ptr<NiceMock<MockConnection>> p_conn;
  NiceMock<MockConnection>& conn;
  bridge::Bridge lib;
  libulf_handle libHandle{reinterpret_cast<libulf_handle>(&lib)};

  template<bool read_all = false>
  void assertTransmitErrorCalls() {
    throwTransmitException(libulf::Error::usb);
    EXPECT_CALL(conn, _write(_, _)).Times(1);
    if constexpr (read_all) EXPECT_CALL(conn, _read_all(_, _, _, _)).Times(0);
    else EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(0);
  }

  template<bool read_all = false>
  void assertReceiveErrorCalls() {
    throwReceiveException(libulf::Error::usb);
    {
      InSequence i;
      EXPECT_CALL(conn, _write(_, _)).Times(1);
      if constexpr (read_all) EXPECT_CALL(conn, _read_all(_, _, _, _)).Times(1);
      else EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
    }
  }

  void throwTransmitException(libulf::Error error = libulf::Error::usb) {
    ON_CALL(conn, _write(_, _))
      .WillByDefault(
        Throw(libulf::ulf_error{error, "A very important error message"}));
  }

  void throwReceiveException(libulf::Error error = libulf::Error::usb) {
    ON_CALL(conn, _read_all(_, _, _, _))
      .WillByDefault(
        Throw(libulf::ulf_error{error, "A very important error message"}));
    ON_CALL(conn, _read_until(_, _, _, _, _))
      .WillByDefault(
        Throw(libulf::ulf_error{error, "A very important error message"}));
  }
};
