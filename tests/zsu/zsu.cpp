#include "f_zsu.hpp"

TEST_F(TestZsu, zsu_firmware_id) {
  auto const id{libklug_zsu_firmware_id(fwItHandle)};

  ASSERT_EQ(id, fwIt.get().id);
}

TEST_F(TestZsu, zsu_firmware_name) {
  auto const name{std::string_view{libklug_zsu_firmware_name(fwItHandle)}};

  ASSERT_EQ(name, fwIt.get().name);
}

TEST_F(TestZsu, zsu_firmware_version_major) {
  auto const version_major{
    std::string_view{libklug_zsu_firmware_version_major(fwItHandle)}};

  ASSERT_EQ(version_major, fwIt.get().major_version);
}

TEST_F(TestZsu, zsu_firmware_version_minor) {
  auto const version_minor{
    std::string_view{libklug_zsu_firmware_version_minor(fwItHandle)}};

  ASSERT_EQ(version_minor, fwIt.get().minor_version);
}

TEST_F(TestZsu, zsu_firmware_type) {
  auto const type{libklug_zsu_firmware_type(fwItHandle)};

  ASSERT_EQ(type, fwIt.get().type);
}

TEST_F(TestZsu, zsu_blocks) {
  auto const blocks{libklug_zsu_firmware_blocks(libHandle, fwItHandle)};

  ASSERT_EQ(blocks, (fwIt.get().bin.size() + 64u - 1u) / 64u);
}
