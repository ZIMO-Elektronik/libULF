#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <libklug/internal/connection.hpp>

struct MockConnection : public Connection {
  MOCK_METHOD(int, open, (uint16_t, uint16_t), (override));
  MOCK_METHOD(int, openFd, (int Fd), (override));

  MOCK_METHOD(int, config, (), (override));
  MOCK_METHOD(int, claim, (), (override));

  MOCK_METHOD(int, release, (), (override));
  MOCK_METHOD(void, close, (), (override));

  MOCK_METHOD(int, _transmit, (std::span<uint8_t const>, uint32_t), (override));
  MOCK_METHOD(int, _receive, (uint8_t*, uint32_t, int*, uint32_t), (override));
};
