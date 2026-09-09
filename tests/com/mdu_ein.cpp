#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_com.hpp"
#include "helper.hpp"

using testing::_;
using testing::Ge;
using testing::InSequence;
using testing::Return;

TEST_F(TestCOM, mdu_ein_payload) {
  using std::operator""sv;

  auto const p{"MDU_EIN\r"sv};
  std::span<uint8_t const> const expected_payload{
    std::bit_cast<uint8_t*>(p.data()), p.size()};

  {
    testing::InSequence i;
    EXPECT_CALL(conn, _write(RM(expected_payload), _)).Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
  }

  bool r{};
  libklug_com_mdu_ein(libHandle, &r);
}

TEST_F(TestCOM, mdu_ein_result_ok) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::com::receive_ok);

  bool r{};
  ASSERT_EQ(libklug_com_mdu_ein(libHandle, &r), LIBKLUG_OK);
  ASSERT_TRUE(r);
}

TEST_F(TestCOM, mdu_ein_result_not_ok) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::com::receive_not_ok);

  bool r{};
  ASSERT_EQ(libklug_com_mdu_ein(libHandle, &r), LIBKLUG_OK);
  ASSERT_FALSE(r);
}

TEST_F(TestCOM, mdu_ein_transmit_error) {
  assertTransmitErrorCalls();

  bool r{};
  libklug_com_mdu_ein(libHandle, &r);
}

TEST_F(TestCOM, mdu_ein_transmit_error_result) {
  throwTransmitException();

  bool r{};
  ASSERT_NE(libklug_com_mdu_ein(libHandle, &r), LIBKLUG_OK);
}

TEST_F(TestCOM, mdu_ein_receive_error) {
  assertReceiveErrorCalls();

  bool r{};
  libklug_com_mdu_ein(libHandle, &r);
}

TEST_F(TestCOM, mdu_ein_receive_error_result) {
  throwReceiveException();

  bool r{};
  ASSERT_NE(libklug_com_mdu_ein(libHandle, &r), LIBKLUG_OK);
}
