#include <ulf/susiv2.hpp>
#include <zusi/zusi.hpp>
#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_susiv2.hpp"
#include "helper.hpp"

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
    EXPECT_CALL(conn, _write(RM(expected), _)).Times(1);
    EXPECT_CALL(conn, _read_all(_, _, _, _)).Times(1);
  }

  lib.susiv2().zppErase();
  lib.result();
}

TEST_F(TestSUSIV2, zpp_erase_result_success) {
  std::vector<uint8_t> r{ulf::susiv2::ack};
  r.push_back(zusi::crc8(r));

  ON_CALL(conn, _read_all(_, Ge(r.size()), _, _))
    .WillByDefault(helper::susiv2::receive_ack);

  lib.susiv2().zppErase();
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::Status>(result));
  ASSERT_TRUE(std::get<res::Status>(result));
}

TEST_F(TestSUSIV2, zpp_erase_write_error) {
  assertTransmitErrorCalls<true>();

  lib.susiv2().zppErase();
  lib.result();
}

TEST_F(TestSUSIV2, zpp_erase_write_error_result) {
  throwTransmitException();

  lib.susiv2().zppErase();
  auto const result{lib.result()};

  assertTransmitReceiveErrorResult(result);
}

TEST_F(TestSUSIV2, zpp_erase_receive_error) {
  assertReceiveErrorCalls<true>();

  lib.susiv2().zppErase();
  lib.result();
}

TEST_F(TestSUSIV2, zpp_erase_receive_error_result) {
  throwReceiveException();

  lib.susiv2().zppErase();
  auto const result{lib.result()};

  assertTransmitReceiveErrorResult(result);
}
