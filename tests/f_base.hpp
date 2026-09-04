#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <klug/c/libklug.h>
#include <bridge/bridge.hpp>
#include <klug/cpp/klug_error.hpp>
#include <memory>
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
  libklug_handle libHandle{reinterpret_cast<libklug_handle>(&lib)};

  template<bool read_all = false>
  void assertTransmitErrorCalls() {
    throwTransmitException(libklug::Error::usb);
    EXPECT_CALL(conn, _write(_, _)).Times(1);
    if constexpr (read_all) EXPECT_CALL(conn, _read_all(_, _, _, _)).Times(0);
    else EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(0);
  }

  template<bool read_all = false>
  void assertReceiveErrorCalls() {
    throwReceiveException(libklug::Error::usb);
    {
      InSequence i;
      EXPECT_CALL(conn, _write(_, _)).Times(1);
      if constexpr (read_all) EXPECT_CALL(conn, _read_all(_, _, _, _)).Times(1);
      else EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
    }
  }

  void throwTransmitException(libklug::Error error = libklug::Error::usb) {
    ON_CALL(conn, _write(_, _))
      .WillByDefault(
        Throw(libklug::klug_error{error, "A very important error message"}));
  }

  void throwReceiveException(libklug::Error error = libklug::Error::usb) {
    ON_CALL(conn, _read_all(_, _, _, _))
      .WillByDefault(
        Throw(libklug::klug_error{error, "A very important error message"}));
    ON_CALL(conn, _read_until(_, _, _, _, _))
      .WillByDefault(
        Throw(libklug::klug_error{error, "A very important error message"}));
  }
};
