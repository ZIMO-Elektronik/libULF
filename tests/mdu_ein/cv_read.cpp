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
                _write(RM(helper::mdu::packet2frame(
                         mdu::make_cv_read_packet(cv_address, 0u))),
                       _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);

    // Bit1
    EXPECT_CALL(conn,
                _write(RM(helper::mdu::packet2frame(
                         mdu::make_cv_read_packet(cv_address, 1u))),
                       _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);

    // Bit2
    EXPECT_CALL(conn,
                _write(RM(helper::mdu::packet2frame(
                         mdu::make_cv_read_packet(cv_address, 2u))),
                       _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);

    // Bit3
    EXPECT_CALL(conn,
                _write(RM(helper::mdu::packet2frame(
                         mdu::make_cv_read_packet(cv_address, 3u))),
                       _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);

    // Bit4
    EXPECT_CALL(conn,
                _write(RM(helper::mdu::packet2frame(
                         mdu::make_cv_read_packet(cv_address, 4u))),
                       _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);

    // Bit5
    EXPECT_CALL(conn,
                _write(RM(helper::mdu::packet2frame(
                         mdu::make_cv_read_packet(cv_address, 5u))),
                       _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);

    // Bit6
    EXPECT_CALL(conn,
                _write(RM(helper::mdu::packet2frame(
                         mdu::make_cv_read_packet(cv_address, 6u))),
                       _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);

    // Bit7
    EXPECT_CALL(conn,
                _write(RM(helper::mdu::packet2frame(
                         mdu::make_cv_read_packet(cv_address, 7u))),
                       _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
  }

  int r{};
  libulf_mdu_ein_cv_read(libHandle, cv_address, &r);
}

TEST_F(TestMDU_EIN, cv_read_result_success) {
  uint8_t const val{145u};

  EXPECT_CALL(conn, _read_until(_, _, _, _, _))
    .Times(8u)
    .WillOnce(helper::mdu::receive_nak)  // = 145
    .WillOnce(helper::mdu::receive_ack)  //
    .WillOnce(helper::mdu::receive_ack)  //
    .WillOnce(helper::mdu::receive_ack)  //
    .WillOnce(helper::mdu::receive_nak)  //
    .WillOnce(helper::mdu::receive_ack)  //
    .WillOnce(helper::mdu::receive_ack)  //
    .WillOnce(helper::mdu::receive_nak); //

  int r{};
  ASSERT_EQ(libulf_mdu_ein_cv_read(libHandle, cv_address, &r), LIBULF_OK);
  ASSERT_EQ(r, cv_value);
}

/// \note Technically, we should test this against every transfer
TEST_F(TestMDU_EIN, cv_read_write_error) {
  assertTransmitErrorCalls();

  int r{};
  libulf_mdu_ein_cv_read(libHandle, cv_address, &r);
}

TEST_F(TestMDU_EIN, cv_read_write_error_result) {
  throwTransmitException();

  int r{};
  ASSERT_NE(libulf_mdu_ein_cv_read(libHandle, cv_address, &r), LIBULF_OK);
}

TEST_F(TestMDU_EIN, cv_read_receive_error) {
  assertReceiveErrorCalls();

  int r{};
  libulf_mdu_ein_cv_read(libHandle, cv_address, &r);
}

TEST_F(TestMDU_EIN, cv_read_receive_error_result) {
  throwReceiveException();

  int r{};
  ASSERT_NE(libulf_mdu_ein_cv_read(libHandle, cv_address, &r), LIBULF_OK);
}
