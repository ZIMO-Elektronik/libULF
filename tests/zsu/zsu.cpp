#include "f_zsu.hpp"

TEST_F(TestZsu, zsu_firmware_id) {
  auto const id{libulf_zsu_get_firmware_id(fileHandle, fwIndex)};

  ASSERT_EQ(id, file->firmwares.at(fwIndex).id);
}

TEST_F(TestZsu, zsu_firmware_name) {
  auto const name{
    std::string_view{libulf_zsu_get_firmware_name(fileHandle, fwIndex)}};

  ASSERT_EQ(name, file->firmwares.at(fwIndex).name);
}

TEST_F(TestZsu, zsu_firmware_version_major) {
  auto const version_major{std::string_view{
    libulf_zsu_get_firmware_major_version(fileHandle, fwIndex)}};

  ASSERT_EQ(version_major, file->firmwares.at(fwIndex).major_version);
}

TEST_F(TestZsu, zsu_firmware_version_minor) {
  auto const version_minor{std::string_view{
    libulf_zsu_get_firmware_minor_version(fileHandle, fwIndex)}};

  ASSERT_EQ(version_minor, file->firmwares.at(fwIndex).minor_version);
}

TEST_F(TestZsu, zsu_firmware_type) {
  auto const type{libulf_zsu_get_firmware_type(fileHandle, fwIndex)};

  ASSERT_EQ(type, file->firmwares.at(fwIndex).type);
}

TEST_F(TestZsu, zsu_block_count) {
  auto const blocks{libulf_zsu_get_firmware_block_count(fileHandle, fwIndex)};

  ASSERT_EQ(blocks, (file->firmwares.at(fwIndex).bin.size() + 64u - 1u) / 64u);
}
