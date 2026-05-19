#include <mdu/mdu.hpp>
#include <ulf/mdu_ein.hpp>
#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_mdu_ein.hpp"
#include "helper.hpp"

using testing::_;
using testing::Ge;
using testing::InSequence;
using testing::Return;

namespace {

constexpr uint16_t cv_address{8u};
constexpr uint8_t cv_value{145u};

} // namespace

TEST_F(TestMDU_EIN, cv_read_payload) {
  {
    InSequence i;
    // Bit0
    EXPECT_CALL(conn,
                _transmit(RM(helper::mdu::packet2frame(
                            mdu::make_cv_read_packet(cv_address, 0u))),
                          _))
      .Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);

    // Bit1
    EXPECT_CALL(conn,
                _transmit(RM(helper::mdu::packet2frame(
                            mdu::make_cv_read_packet(cv_address, 1u))),
                          _))
      .Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);

    // Bit2
    EXPECT_CALL(conn,
                _transmit(RM(helper::mdu::packet2frame(
                            mdu::make_cv_read_packet(cv_address, 2u))),
                          _))
      .Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);

    // Bit3
    EXPECT_CALL(conn,
                _transmit(RM(helper::mdu::packet2frame(
                            mdu::make_cv_read_packet(cv_address, 3u))),
                          _))
      .Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);

    // Bit4
    EXPECT_CALL(conn,
                _transmit(RM(helper::mdu::packet2frame(
                            mdu::make_cv_read_packet(cv_address, 4u))),
                          _))
      .Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);

    // Bit5
    EXPECT_CALL(conn,
                _transmit(RM(helper::mdu::packet2frame(
                            mdu::make_cv_read_packet(cv_address, 5u))),
                          _))
      .Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);

    // Bit6
    EXPECT_CALL(conn,
                _transmit(RM(helper::mdu::packet2frame(
                            mdu::make_cv_read_packet(cv_address, 6u))),
                          _))
      .Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);

    // Bit7
    EXPECT_CALL(conn,
                _transmit(RM(helper::mdu::packet2frame(
                            mdu::make_cv_read_packet(cv_address, 7u))),
                          _))
      .Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  lib.mdu_ein().cvRead(cv_address);
  lib.result();
}

TEST_F(TestMDU_EIN, cv_read_result_success) {
  uint8_t const val{145u};

  EXPECT_CALL(conn, _receive(_, _, _, _))
    .Times(8u)
    .WillOnce(helper::mdu::receive_nak)  // = 145
    .WillOnce(helper::mdu::receive_ack)  //
    .WillOnce(helper::mdu::receive_ack)  //
    .WillOnce(helper::mdu::receive_ack)  //
    .WillOnce(helper::mdu::receive_nak)  //
    .WillOnce(helper::mdu::receive_ack)  //
    .WillOnce(helper::mdu::receive_ack)  //
    .WillOnce(helper::mdu::receive_nak); //

  lib.mdu_ein().cvRead(cv_address);
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::Cv>(result));
  ASSERT_EQ(std::get<res::Cv>(result), cv_value);
}

/// \note Technically, we should test this against every transfer
TEST_F(TestMDU_EIN, cv_read_transmit_error) {
  assertTransmitErrorCalls();

  lib.susiv2().cvRead(cv_value);
  lib.result();
}

TEST_F(TestMDU_EIN, cv_read_transmit_error_result) {
  throwTransmitException();

  lib.mdu_ein().cvRead(cv_address);
  auto const result{lib.result()};

  assertTransmitReceiveErrorResult(result);
}

TEST_F(TestMDU_EIN, cv_read_receive_error) {
  assertReceiveErrorCalls();

  lib.mdu_ein().cvRead(cv_address);
  lib.result();
}

TEST_F(TestMDU_EIN, cv_read_receive_error_result) {
  throwReceiveException();

  lib.mdu_ein().cvRead(cv_address);
  auto const result{lib.result()};

  assertTransmitReceiveErrorResult(result);
}
