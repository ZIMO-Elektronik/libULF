#include "../range_matcher.hpp"
#include "f_com.hpp"

using testing::_;

TEST_F(TestCOM, ping) {
  using std::operator""sv;

  auto const p{"PING\r"sv};
  std::span<uint8_t const> const expected_payload{
    std::bit_cast<uint8_t*>(p.data()), p.size()};

  {
    testing::InSequence i;
    EXPECT_CALL(conn, _transmit(RM(expected_payload), _)).Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  lib.com().ping();
}
