#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <libklug/libklug.h>
#include <libklug/internal/bridge/bridge.hpp>
#include <libklug/internal/exception/e_generic.hpp>
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
    throwTransmitException(err::Error::usb);
    EXPECT_CALL(conn, _write(_, _)).Times(1);
    if constexpr (read_all) EXPECT_CALL(conn, _read_all(_, _, _, _)).Times(0);
    else EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(0);
  }

  template<bool read_all = false>
  void assertReceiveErrorCalls() {
    throwReceiveException(err::Error::usb);
    {
      InSequence i;
      EXPECT_CALL(conn, _write(_, _)).Times(1);
      if constexpr (read_all) EXPECT_CALL(conn, _read_all(_, _, _, _)).Times(1);
      else EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
    }
  }

  void throwTransmitException(err::Error error = err::Error::usb) {
    using std::operator""sv;
    ON_CALL(conn, _write(_, _))
      .WillByDefault(Throw(
        except::generic_error{error, "A very important error message"sv}));
  }

  void throwReceiveException(err::Error error = err::Error::usb) {
    using std::operator""sv;
    ON_CALL(conn, _read_all(_, _, _, _))
      .WillByDefault(Throw(
        except::generic_error{error, "A very important error message"sv}));
    ON_CALL(conn, _read_until(_, _, _, _, _))
      .WillByDefault(Throw(
        except::generic_error{error, "A very important error message"sv}));
  }
};
