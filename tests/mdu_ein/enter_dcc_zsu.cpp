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

constexpr uint32_t sn{0x00FF00FFuz};
constexpr uint32_t id{0xFF00FF00uz};

} // namespace

TEST_F(TestMDU_EIN, dcc_zsu_payload) {
  {
    InSequence i;
    EXPECT_CALL(
      conn,
      _transmit(RM(helper::mdu::special2frame(
                  helper::mdu::make_dcc_zsu_entry_command(id, sn, false))),
                _))
      .Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  lib.mdu_ein().enterDCCZSU(id, sn);
  lib.result();
}

TEST_F(TestMDU_EIN, dcc_zsu_esult_success) {
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(helper::mdu::receive_ack);

  lib.mdu_ein().enterDCCZSU(id, sn);
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::Status>(result));
  ASSERT_TRUE(std::get<res::Status>(result));
}

TEST_F(TestMDU_EIN, dcc_zsu_esult_no_success) {
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(helper::mdu::receive_nak);

  lib.mdu_ein().enterDCCZSU(id, sn);
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::Status>(result));
  ASSERT_FALSE(std::get<res::Status>(result));
}

TEST_F(TestMDU_EIN, dcc_zsu_transmit_error) {
  ON_CALL(conn, _transmit(_, _)).WillByDefault(Return(LIBUSB_ERROR_IO));

  EXPECT_CALL(conn, _transmit(_, _)).Times(1);
  EXPECT_CALL(conn, _receive(_, _, _, _)).Times(0);

  lib.mdu_ein().enterDCCZSU(id, sn);
  lib.result();
}

TEST_F(TestMDU_EIN, dcc_zsu_transmit_error_result) {
  ON_CALL(conn, _transmit(_, _)).WillByDefault(Return(LIBUSB_ERROR_IO));

  EXPECT_CALL(conn, _transmit(_, _)).Times(1);
  EXPECT_CALL(conn, _receive(_, _, _, _)).Times(0);

  lib.mdu_ein().enterDCCZSU(id, sn);
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::LibusbError>(result));
  ASSERT_EQ(std::get<res::LibusbError>(result), LIBUSB_ERROR_IO);
}

TEST_F(TestMDU_EIN, dcc_zsu_eceive_error) {
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(Return(LIBUSB_ERROR_IO));

  {
    InSequence i;
    EXPECT_CALL(conn, _transmit(_, _)).Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  lib.mdu_ein().enterDCCZSU(id, sn);
  lib.result();
}

TEST_F(TestMDU_EIN, dcc_zsu_eceive_error_result) {
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(Return(LIBUSB_ERROR_IO));

  {
    InSequence i;
    EXPECT_CALL(conn, _transmit(_, _)).Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  lib.mdu_ein().enterDCCZSU(id, sn);
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::LibusbError>(result));
  ASSERT_EQ(std::get<res::LibusbError>(result), LIBUSB_ERROR_IO);
}
