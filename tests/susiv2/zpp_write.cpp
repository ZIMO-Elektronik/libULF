#include <ulf/susiv2.hpp>
#include <zusi/zusi.hpp>
#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_susiv2.hpp"

using testing::_;
using testing::Ge;
using testing::InSequence;
using testing::Return;

constexpr auto flash{helper::sequence<256u>};
constexpr auto address{0uz};

TEST_F(TestSUSIV2, zpp_write_payload) {
  auto const expected{helper::range2span(
    ulf::susiv2::packet2frame<
      ztl::inplace_vector<uint8_t, ZUSI_MAX_PACKET_SIZE + 5uz>>(
      zusi::make_zpp_write_packet(flash.size() - 1u, 0uz, flash)))};

  {
    InSequence i;
    EXPECT_CALL(conn, _transmit(RM(expected), _)).Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  lib.susiv2().zppWrite(address, flash);
  lib.result();
}

TEST_F(TestSUSIV2, zpp_write_result_success) {
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

  lib.susiv2().zppWrite(address, flash);
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::Status>(result));
  ASSERT_TRUE(std::get<res::Status>(result));
}

TEST_F(TestSUSIV2, zpp_write_transmit_error) {
  ON_CALL(conn, _transmit(_, _)).WillByDefault(Return(LIBUSB_ERROR_IO));

  EXPECT_CALL(conn, _transmit(_, _)).Times(1);
  EXPECT_CALL(conn, _receive(_, _, _, _)).Times(0);

  lib.susiv2().zppWrite(address, flash);
  lib.result();
}

TEST_F(TestSUSIV2, zpp_write_transmit_error_result) {
  ON_CALL(conn, _transmit(_, _)).WillByDefault(Return(LIBUSB_ERROR_IO));

  EXPECT_CALL(conn, _transmit(_, _)).Times(1);
  EXPECT_CALL(conn, _receive(_, _, _, _)).Times(0);

  lib.susiv2().zppWrite(address, flash);
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::LibusbError>(result));
  ASSERT_EQ(std::get<res::LibusbError>(result), LIBUSB_ERROR_IO);
}

TEST_F(TestSUSIV2, zpp_write_receive_error) {
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(Return(LIBUSB_ERROR_IO));

  {
    InSequence i;
    EXPECT_CALL(conn, _transmit(_, _)).Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  lib.susiv2().zppWrite(address, flash);
  lib.result();
}

TEST_F(TestSUSIV2, zpp_write_receive_error_result) {
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(Return(LIBUSB_ERROR_IO));

  {
    InSequence i;
    EXPECT_CALL(conn, _transmit(_, _)).Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  lib.susiv2().zppWrite(address, flash);
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::LibusbError>(result));
  ASSERT_EQ(std::get<res::LibusbError>(result), LIBUSB_ERROR_IO);
}
