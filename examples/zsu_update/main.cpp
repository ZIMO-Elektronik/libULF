#include <libklug/libklug.h>
#include <chrono>
#include <codecvt>
#include <cstdlib>
#include <gsl/gsl>
#include <iostream>
#include <locale>
#include <string_view>
#include <thread>
#include "paths.hpp"
#include "setup.hpp"

using namespace std::chrono_literals;

constexpr unsigned int max_retries{5uz};

bool success(result r) {
  if (r.type != result_type::status) {
    std::cout << "Unexpected Result" << std::endl;
    return false;
  }

  return r.data.success == LIBKLUG_TRUE;
}

int main() {
  auto lib{setup::connect()};
  if (!lib) return -1;

  auto zsu{libklug_zsu_read(paths::zsu_path.data(), paths::zsu_path.size())};
  if (!zsu) return -1;

  auto const fw_it{libklug_zsu_firmware_iterator_create_begin(zsu)};

  gsl::final_action a([&]() {
    libklug_com_reset(lib);
    if (!success(libklug_result(lib))) {
      std::cout << "Unable to RESET device" << std::endl;
      return -1;
    }
    libklug_zsu_destroy_firmware_iterator(fw_it);
    libklug_zsu_release(zsu);
    setup::disconnect(lib);
    return 0;
  });

  result r{};

  libklug_com_mdu_ein(lib);
  r = libklug_result(lib);
  if (!success(r)) {
    std::cout << "Unable to enter MDU_EIN" << std::endl;
    return -1;
  }
  std::cout << "Entered MDU_EIN" << std::endl;

  libklug_mdu_ein_enter_mdu(lib);
  r = libklug_result(lib);
  if (!success(r)) {
    std::cout << "Unable to enter Decoder BL" << std::endl;
    return -1;
  }
  std::cout << "Entered Bootloader via MDU" << std::endl;

  libklug_mdu_ein_config_transfer_rate(lib, 3);
  r = libklug_result(lib);
  if (!success(r)) {
    std::cout << "Unable to set Transfer Rate" << std::endl;
    return -1;
  }
  std::cout << "Set transfer rate to slow" << std::endl;

  std::cout << "Searching decoder" << std::endl;
  bool found{};
  do { // Caution, this assumes at least one firmware in file
    libklug_mdu_ein_ping(lib, 0uz, libklug_zsu_firmware_iterator_get_id(fw_it));
    // Inverted success because of ping
    if (success(libklug_result(lib))) {
      found = true;
      break;
    }

  } while (libklug_zsu_firmware_iterator_next(fw_it));
  if (found)
    std::cout << "Found decoder "
              << libklug_zsu_firmware_iterator_get_name(fw_it) << " with ID "
              << std::hex << libklug_zsu_firmware_iterator_get_id(fw_it)
              << std::dec << std::endl;
  else {
    std::cout << "Unable to find decoder " << std::endl;
    return -1;
  }

  libklug_mdu_ein_zsu_salsa20_iv(lib, fw_it);
  r = libklug_result(lib);
  if (!success(r)) {
    std::cout << "Unable to init Salsa20" << std::endl;
    return -1;
  }
  std::cout << "Salsa20 initialized" << std::endl;

  libklug_mdu_ein_zsu_erase(lib, fw_it);
  r = libklug_result(lib);
  if (!success(r)) {
    std::cout << "Unable to erase flash" << std::endl;
    return -1;
  }
  std::cout << "Erasing flash" << std::endl;

  for (int i{0}; i < 10; i++) { // Loop for 10s
    libklug_mdu_ein_busy(lib);
    libklug_result(lib);
    std::this_thread::sleep_for(1000ms);
  }
  std::cout << "Flash erased" << std::endl;

  // std::cout << "Progress" << std::endl;
  long const blocks{libklug_zsu_firmware_iterator_get_blocks(fw_it)};
  double progress{0.0};
  unsigned int retry{0uz};
  for (long i{0}; i < blocks; i++) {
    // int barWidth = 70;
    // std::cout << "[";
    // int pos = static_cast<int>(barWidth * progress);
    // for (int j{0}; j < barWidth; j++) {
    //   if (j < pos) std::cout << "=";
    //   else if (j == pos) std::cout << ">";
    //   else std::cout << " ";
    // }

    libklug_mdu_ein_zsu_update(lib, fw_it, i);
    r = libklug_result(lib);
    if (!success(r)) {
      if (retry >= max_retries) {
        std::cout << "Error at block " << i << std::endl;
        return -1;
      }
      retry++;
      i--;
    } else retry = 0;

    // progress = static_cast<double>(i + 1) / static_cast<double>(blocks);
    // std::cout << "] Progress " << static_cast<int>(progress * 100) << "%";
    // std::cout << "\r";
    // std::cout.flush();
  }
  std::cout << std::endl;

  libklug_mdu_ein_zsu_crc32_start(lib, fw_it);
  r = libklug_result(lib);
  if (!success(r)) {
    std::cout << "Unable to init CRC32 verification" << std::endl;
    return -1;
  }
  std::cout << "CRC32 check started" << std::endl;

  libklug_mdu_ein_zsu_crc32_result_exit(lib);
  r = libklug_result(lib);
  if (!success(r)) {
    std::cout << "Bad CRC32" << std::endl;
    return -1;
  }
  std::cout << "CRC32 check successful" << std::endl;

  return 0;
}
