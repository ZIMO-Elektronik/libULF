#include <ulf/susiv2.hpp>
#include <zusi/zusi.hpp>
#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_susiv2.hpp"

using testing::_;
using testing::Ge;
using testing::InSequence;
using testing::Return;

TEST_F(TestSUSIV2, zpp_erase_payload) {
  auto const payload{ulf::susiv2::packet2frame<
    ztl::inplace_vector<uint8_t, ZUSI_MAX_PACKET_SIZE + 5uz>>(
    zusi::make_zpp_erase_packet())};
  auto const expected{helper::range2span(payload)};

  {
    InSequence i;
    EXPECT_CALL(conn, _transmit(RM(expected), _)).Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  lib.susiv2().zppErase();
  lib.result();
}

TEST_F(TestSUSIV2, zpp_erase_result_success) {
  std::vector<uint8_t> r{ulf::susiv2::ack};
  r.push_back(zusi::crc8(r));

  ON_CALL(conn, _receive(_, Ge(r.size()), _, _))
    .WillByDefault(
      [&](uint8_t* buf, uint32_t len, int* rx_ed, uint32_t timeout) {
        assert(len >= r.size());
        std::ranges::copy(r, buf);
        *rx_ed = r.size();
        return 0;
      });

  lib.susiv2().zppErase();
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::Status>(result));
  ASSERT_TRUE(std::get<res::Status>(result));
}

TEST_F(TestSUSIV2, zpp_erase_transmit_error) {
  ON_CALL(conn, _transmit(_, _)).WillByDefault(Return(LIBUSB_ERROR_IO));

  EXPECT_CALL(conn, _transmit(_, _)).Times(1);
  EXPECT_CALL(conn, _receive(_, _, _, _)).Times(0);

  lib.susiv2().zppErase();
  lib.result();
}

TEST_F(TestSUSIV2, zpp_erase_transmit_error_result) {
  ON_CALL(conn, _transmit(_, _)).WillByDefault(Return(LIBUSB_ERROR_IO));

  EXPECT_CALL(conn, _transmit(_, _)).Times(1);
  EXPECT_CALL(conn, _receive(_, _, _, _)).Times(0);

  lib.susiv2().zppErase();
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::LibusbError>(result));
  ASSERT_EQ(std::get<res::LibusbError>(result), LIBUSB_ERROR_IO);
}

TEST_F(TestSUSIV2, zpp_erase_receive_error) {
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(Return(LIBUSB_ERROR_IO));

  {
    InSequence i;
    EXPECT_CALL(conn, _transmit(_, _)).Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  lib.susiv2().zppErase();
  lib.result();
}

TEST_F(TestSUSIV2, zpp_erase_receive_error_result) {
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(Return(LIBUSB_ERROR_IO));

  {
    InSequence i;
    EXPECT_CALL(conn, _transmit(_, _)).Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  lib.susiv2().zppErase();
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::LibusbError>(result));
  ASSERT_EQ(std::get<res::LibusbError>(result), LIBUSB_ERROR_IO);
}
