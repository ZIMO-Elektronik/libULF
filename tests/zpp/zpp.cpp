#include "../range_matcher.hpp"
#include "f_zpp.hpp"

using testing::_;

TEST_F(TestZpp, blocks) {
  auto const blocks{lib.zpp().blocks(file)};

  ASSERT_EQ(blocks, (file->flash.size() + 256u - 1u) / 256u);
}

TEST_F(TestZpp, blocks_no_file) { ASSERT_DEATH(lib.zpp().blocks(nullptr), _); }

TEST_F(TestZpp, block) {
  auto const block{0u};
  auto const expected_block{
    std::span<uint8_t>{file->flash}.subspan(block * 256u, 256u)};

  auto const addr_block{lib.zpp().block(file, block)};

  ASSERT_EQ(addr_block.first, block);
  ASSERT_THAT(addr_block.second, RM(expected_block));
}

TEST_F(TestZpp, last_block) {
  auto const blocks{lib.zpp().blocks(file)};
  auto const last_block{blocks - 1u};

  auto const expected_block{
    std::span<uint8_t>{file->flash}.subspan(last_block * 256u, 256u)};

  auto const addr_block{lib.zpp().block(file, last_block)};

  ASSERT_EQ(addr_block.first, last_block * 256u);
  ASSERT_THAT(addr_block.second, RM(expected_block));
}

TEST_F(TestZpp, block_out_of_bounds) {
  auto const blocks{lib.zpp().blocks(file)};

  ASSERT_DEATH(lib.zpp().block(file, blocks), _);
}

TEST_F(TestZpp, block_no_file) {
  ASSERT_DEATH(lib.zpp().block(nullptr, 0u), _);
}

TEST_F(TestZpp, author) {
  auto const str{lib.zpp().author(file)};

  ASSERT_EQ(file->author, str);
}

TEST_F(TestZpp, author_no_file) { ASSERT_DEATH(lib.zpp().author(nullptr), _); }

TEST_F(TestZpp, email) {
  auto const str{lib.zpp().email(file)};

  ASSERT_EQ(file->email, str);
}

TEST_F(TestZpp, email_no_file) { ASSERT_DEATH(lib.zpp().email(nullptr), _); }
