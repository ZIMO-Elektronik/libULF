#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <libklug/libklug.h>
#include <libklug/internal/bridge/bridge.hpp>
#include <libklug/internal/exception/e_generic.hpp>
#include <libklug/internal/exception/e_libusb.hpp>
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
  void assertTransmitErrorCalls(int error = LIBUSB_ERROR_IO) {
    throwTransmitException(error);
    EXPECT_CALL(conn, _write(_, _)).Times(1);
    if constexpr (read_all) EXPECT_CALL(conn, _read_all(_, _, _, _)).Times(0);
    else EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(0);
  }

  template<bool read_all = false>
  void assertReceiveErrorCalls(int error = LIBUSB_ERROR_IO) {
    throwReceiveException(error);
    {
      InSequence i;
      EXPECT_CALL(conn, _write(_, _)).Times(1);
      if constexpr (read_all) EXPECT_CALL(conn, _read_all(_, _, _, _)).Times(1);
      else EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
    }
  }

  void throwTransmitException(int error = LIBUSB_ERROR_IO) {
    using std::operator""sv;
    ON_CALL(conn, _write(_, _))
      .WillByDefault(
        Throw(except::libusb_error{error, "A very important error message"sv}));
  }

  void throwReceiveException(int error = LIBUSB_ERROR_IO) {
    using std::operator""sv;
    ON_CALL(conn, _read_all(_, _, _, _))
      .WillByDefault(
        Throw(except::libusb_error{error, "A very important error message"sv}));
    ON_CALL(conn, _read_until(_, _, _, _, _))
      .WillByDefault(
        Throw(except::libusb_error{error, "A very important error message"sv}));
  }

  void assertTransmitReceiveErrorResult(result const& result,
                                        int error = LIBUSB_ERROR_IO) {
    ASSERT_EQ(result.type, result_type::libusb_error);
    ASSERT_EQ(result.data.libusb_error, error);
  }
  void assertTransmitReceiveErrorResult(res::Result const& result,
                                        int error = LIBUSB_ERROR_IO) {
    ASSERT_TRUE(std::holds_alternative<res::LibusbError>(result));
    ASSERT_EQ(std::get<res::LibusbError>(result), error);
  }
};
