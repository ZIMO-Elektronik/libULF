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

  libklug_susiv2_zpp_erase(libHandle);
  libklug_job_await(libHandle);
}

TEST_F(TestSUSIV2, zpp_erase_result_success) {
  std::vector<uint8_t> r{ulf::susiv2::ack};
  r.push_back(zusi::crc8(r));

  ON_CALL(conn, _read_all(_, Ge(r.size()), _, _))
    .WillByDefault(helper::susiv2::receive_ack);

  libklug_susiv2_zpp_erase(libHandle);
  auto const result{libklug_job_await(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_TRUE(result.data.success);
}

TEST_F(TestSUSIV2, zpp_erase_write_error) {
  assertTransmitErrorCalls<true>();

  libklug_susiv2_zpp_erase(libHandle);
  libklug_job_await(libHandle);
}

TEST_F(TestSUSIV2, zpp_erase_write_error_result) {
  throwTransmitException();

  libklug_susiv2_zpp_erase(libHandle);
  auto const result{libklug_job_await(libHandle)};

  assertTransmitReceiveErrorResult(result);
}

TEST_F(TestSUSIV2, zpp_erase_receive_error) {
  assertReceiveErrorCalls<true>();

  libklug_susiv2_zpp_erase(libHandle);
  libklug_job_await(libHandle);
}

TEST_F(TestSUSIV2, zpp_erase_receive_error_result) {
  throwReceiveException();

  libklug_susiv2_zpp_erase(libHandle);
  auto const result{libklug_job_await(libHandle)};

  assertTransmitReceiveErrorResult(result);
}
