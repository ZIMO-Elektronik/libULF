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

constexpr uint16_t cv_index{7u};
constexpr uint8_t cv_value{145u};

TEST_F(TestSUSIV2, cv_read_payload) {
  auto const payload{
    ulf::susiv2::packet2frame(zusi::make_cv_read_packet(0, cv_index))};
  auto const expected{helper::range2span(payload)};

  {
    InSequence i;
    EXPECT_CALL(conn, _write(RM(expected), _)).Times(1);
    EXPECT_CALL(conn, _read_all(_, _, _, _)).Times(1);
  }

  int r{};
  libulf_susiv2_cv_read(libHandle, cv_index, &r);
}

TEST_F(TestSUSIV2, cv_read_result_success) {
  std::vector<uint8_t> expected{ulf::susiv2::ack, cv_value};
  expected.push_back(zusi::crc8(expected));

  ON_CALL(conn, _read_all(_, Ge(expected.size()), _, _))
    .WillByDefault(
      [&](uint8_t* buf, uint32_t len, int* rx_ed, uint32_t timeout) {
        assert(len >= expected.size());
        std::ranges::copy(expected, buf);
        *rx_ed = expected.size();
        return 0;
      });

  int r{};
  ASSERT_EQ(libulf_susiv2_cv_read(libHandle, cv_index, &r), LIBULF_OK);
  ASSERT_EQ(r, cv_value);
}

TEST_F(TestSUSIV2, cv_read_write_error) {
  assertTransmitErrorCalls<true>();

  int r{};
  libulf_susiv2_cv_read(libHandle, cv_index, &r);
}

TEST_F(TestSUSIV2, cv_read_write_error_result) {
  throwTransmitException();

  int r{};
  ASSERT_NE(libulf_susiv2_cv_read(libHandle, cv_index, &r), LIBULF_OK);
}

TEST_F(TestSUSIV2, cv_read_receive_error) {
  assertReceiveErrorCalls<true>();

  int r{};
  libulf_susiv2_cv_read(libHandle, cv_index, &r);
}

TEST_F(TestSUSIV2, cv_read_receive_error_result) {
  throwReceiveException();

  int r{};
  ASSERT_NE(libulf_susiv2_cv_read(libHandle, cv_index, &r), LIBULF_OK);
}
