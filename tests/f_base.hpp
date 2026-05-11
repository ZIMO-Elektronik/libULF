#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <libklug/libklug.hpp>
#include <memory>
#include "mock_connection.hpp"

using testing::NiceMock;

struct TestBase : public testing::Test {
  TestBase()
    : p_conn{std::make_shared<NiceMock<MockConnection>>()}, lib{p_conn},
      conn{*p_conn} {}

  std::shared_ptr<NiceMock<MockConnection>> p_conn;
  NiceMock<MockConnection>& conn;
  libklug::LibKLUG lib;
};