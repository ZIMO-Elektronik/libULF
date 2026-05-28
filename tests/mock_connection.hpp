#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <libklug/internal/connection/i_connection.hpp>

struct MockConnection : public internal::IConnection {
  MOCK_METHOD(int, init, (), (override));

  MOCK_METHOD(int, open, (uint16_t, uint16_t), (override));
  MOCK_METHOD(int, openFd, (int Fd), (override));

  MOCK_METHOD(int, config, (), (override));
  MOCK_METHOD(int, claim, (), (override));

  MOCK_METHOD(int, release, (), (override));
  MOCK_METHOD(void, close, (), (override));

  MOCK_METHOD(void, flush, (), (override));

  MOCK_METHOD(void,
              _transmit,
              (std::span<uint8_t const>, uint32_t),
              (override));
  MOCK_METHOD(void, _receive, (uint8_t*, uint32_t, int*, uint32_t), (override));
};
