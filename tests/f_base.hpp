#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <libklug/libklug.h>
#include <libklug/libklug.hpp>
#include <memory>
#include "mock_connection.hpp"

using testing::_;
using testing::InSequence;
using testing::NiceMock;
using testing::Return;

struct TestBase : public testing::Test {
  TestBase()
    : p_conn{std::make_shared<NiceMock<MockConnection>>()}, lib{p_conn},
      conn{*p_conn} {}

  std::shared_ptr<NiceMock<MockConnection>> p_conn;
  NiceMock<MockConnection>& conn;
  libklug::LibKLUG lib;
  libklug_handle libHandle{reinterpret_cast<libklug_handle>(&lib)};

  void assertTransmitErrorCalls(int error = LIBUSB_ERROR_IO) {
    ON_CALL(conn, _transmit(_, _)).WillByDefault(Return(error));
    EXPECT_CALL(conn, _transmit(_, _)).Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(0);
  }
  void assertReceiveErrorCalls(int error = LIBUSB_ERROR_IO) {
    ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(Return(error));
    {
      InSequence i;
      EXPECT_CALL(conn, _transmit(_, _)).Times(1);
      EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
    }
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
