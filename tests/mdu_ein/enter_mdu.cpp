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

} // namespace

TEST_F(TestMDU_EIN, mdu_payload) {
  {
    InSequence i;
    EXPECT_CALL(conn,
                _write(RM(helper::mdu::special2frame(
                         helper::mdu::make_mdu_entry_command())),
                       _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
  }

  lib.mdu_ein().enterMDU();
  lib.result();
}

TEST_F(TestMDU_EIN, mdu_esult_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_ack);

  lib.mdu_ein().enterMDU();
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::Status>(result));
  ASSERT_TRUE(std::get<res::Status>(result));
}

TEST_F(TestMDU_EIN, mdu_esult_no_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_nak);

  lib.mdu_ein().enterMDU();
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::Status>(result));
  ASSERT_FALSE(std::get<res::Status>(result));
}

TEST_F(TestMDU_EIN, mdu_write_error) {
  assertTransmitErrorCalls();

  lib.mdu_ein().enterMDU();
  lib.result();
}

TEST_F(TestMDU_EIN, mdu_write_error_result) {
  throwTransmitException();

  lib.mdu_ein().enterMDU();
  auto const result{lib.result()};

  assertTransmitReceiveErrorResult(result);
}

TEST_F(TestMDU_EIN, mdu_eceive_error) {
  assertReceiveErrorCalls();

  lib.mdu_ein().enterMDU();
  lib.result();
}

TEST_F(TestMDU_EIN, mdu_eceive_error_result) {
  throwReceiveException();

  lib.mdu_ein().enterMDU();
  auto const result{lib.result()};

  assertTransmitReceiveErrorResult(result);
}
