#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_com.hpp"

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
    EXPECT_CALL(conn, _transmit(RM(expected_payload), _)).Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  lib.com().mdu_ein();
  lib.result();
}

TEST_F(TestCOM, mdu_ein_result_ok) {
  using std::operator""sv;

  auto const r{helper::string_view2span("OK\r")};
  ON_CALL(conn, _receive(_, _, _, _))
    .WillByDefault(
      [&](uint8_t* buf, uint32_t len, int* rx_ed, uint32_t timeout) {
        assert(len >= r.size());
        std::ranges::copy(r, buf);
        *rx_ed = r.size();
        return 0;
      });

  lib.com().mdu_ein();
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::Status>(result));
  ASSERT_TRUE(std::get<res::Status>(result));
}

TEST_F(TestCOM, mdu_ein_result_not_ok) {
  using std::operator""sv;

  auto const r{helper::string_view2span("NOT_OK\r")};
  ON_CALL(conn, _receive(_, _, _, _))
    .WillByDefault(
      [&](uint8_t* buf, uint32_t len, int* rx_ed, uint32_t timeout) {
        assert(len >= r.size());
        std::ranges::copy(r, buf);
        *rx_ed = r.size();
        return 0;
      });

  lib.com().mdu_ein();
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::Status>(result));
  ASSERT_FALSE(std::get<res::Status>(result));
}

// TEST_F(TestCOM, mdu_ein_invalid_format) {
//   using std::operator""sv;
//
//   auto const r{helper::string_view2span("OK"sv)};
//   ON_CALL(conn, _receive(_, _, _, _))
//     .WillByDefault(
//       [&](uint8_t* buf, uint32_t len, int* rx_ed, uint32_t timeout) {
//         assert(len >= r.size());
//         std::ranges::copy(r, buf);
//         *rx_ed = r.size();
//         return 0;
//       });
//
//   lib.com().mdu_ein();
//   auto const result{lib.result()};
//
//   ASSERT_TRUE(std::holds_alternative<res::Error>(result));
//   ASSERT_EQ(std::get<res::Error>(result), err::Error::format);
// }

TEST_F(TestCOM, mdu_ein_transmit_error) {
  ON_CALL(conn, _transmit(_, _)).WillByDefault(Return(LIBUSB_ERROR_IO));

  EXPECT_CALL(conn, _transmit(_, _)).Times(1);
  EXPECT_CALL(conn, _receive(_, _, _, _)).Times(0);

  lib.com().mdu_ein();
  lib.result();
}

TEST_F(TestCOM, mdu_ein_transmit_error_result) {
  ON_CALL(conn, _transmit(_, _)).WillByDefault(Return(LIBUSB_ERROR_IO));

  EXPECT_CALL(conn, _transmit(_, _)).Times(1);
  EXPECT_CALL(conn, _receive(_, _, _, _)).Times(0);

  lib.com().mdu_ein();
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::LibusbError>(result));
  ASSERT_EQ(std::get<res::LibusbError>(result), LIBUSB_ERROR_IO);
}

TEST_F(TestCOM, mdu_ein_receive_error) {
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(Return(LIBUSB_ERROR_IO));

  {
    InSequence i;
    EXPECT_CALL(conn, _transmit(_, _)).Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  lib.com().mdu_ein();
  lib.result();
}

TEST_F(TestCOM, mdu_ein_receive_error_result) {
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(Return(LIBUSB_ERROR_IO));

  {
    InSequence i;
    EXPECT_CALL(conn, _transmit(_, _)).Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  lib.com().mdu_ein();
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::LibusbError>(result));
  ASSERT_EQ(std::get<res::LibusbError>(result), LIBUSB_ERROR_IO);
}
