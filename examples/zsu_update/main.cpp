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

int main() {
  auto lib{setup::connect()};
  if (!lib) return -1;

  auto zsu{libklug_zsu_read(paths::zsu_path.data(), paths::zsu_path.size())};
  if (!zsu) return -1;

  size_t fwIndex{};
  size_t const maxFwIndex{libklug_zsu_get_firmware_count(zsu) - 1uz};

  libklug_bool result{}; // Result container (implicit bool)

  gsl::final_action a([&]() {
    if (libklug_com_reset(lib, &result) != LIBKLUG_OK || !result) {
      std::cout << "Unable to RESET device" << std::endl;
    }
    libklug_zsu_release(zsu);
    setup::disconnect(lib);
  });

  std::cout << "Enter MDU_EIN" << std::endl;
  if (libklug_com_mdu_ein(lib, &result) != LIBKLUG_OK || !result) {
    std::cout << "Unable to enter MDU_EIN" << std::endl;
    return -1;
  };

  std::cout << "Entering Bootloader" << std::endl;
  if (libklug_mdu_ein_enter_mdu(lib, &result) != LIBKLUG_OK || !result) {
    std::cout << "Unable to enter Decoder BL" << std::endl;
    return -1;
  }

  std::cout << "Set transfer rate to slow" << std::endl;
  if (libklug_mdu_ein_config_transfer_rate(lib, 3, &result) != LIBKLUG_OK ||
      !result) {
    std::cout << "Unable to set Transfer Rate" << std::endl;
    return -1;
  }

  std::cout << "Searching decoder" << std::endl;
  bool found{};
  do { // Caution, this assumes at least one firmware in file
    if (libklug_mdu_ein_ping(
          lib, 0uz, libklug_zsu_get_firmware_id(zsu, fwIndex), &result) !=
        LIBKLUG_OK) {
      std::cout << "Error during pinging" << std::endl;
      return -1;
    }

    if (result) {
      found = true;
      break;
    }

  } while (fwIndex++ < maxFwIndex);
  if (found)
    std::cout << "Found decoder " << libklug_zsu_get_firmware_name(zsu, fwIndex)
              << " with ID " << std::hex
              << libklug_zsu_get_firmware_id(zsu, fwIndex) << std::dec
              << std::endl;
  else {
    std::cout << "Unable to find decoder " << std::endl;
    return -1;
  }

  std::cout << "Initialize Salsa20" << std::endl;
  if (libklug_mdu_ein_zsu_salsa20_iv(lib, zsu, fwIndex, &result) || !result) {
    std::cout << "Unable to init Salsa20" << std::endl;
    return -1;
  }

  std::cout << "Erasing Flash" << std::endl;
  if (libklug_mdu_ein_zsu_erase(lib, zsu, fwIndex, &result) != LIBKLUG_OK ||
      !result) {
    std::cout << "Unable to erase flash" << std::endl;
    return -1;
  }

  for (int i{0}; i < 10; i++) { // Loop for 10s
    libklug_mdu_ein_busy(lib, &result);
    std::this_thread::sleep_for(1000ms);
  }
  std::cout << "Flash erased" << std::endl;

  std::cout << "Progress" << std::endl;
  long const blocks{libklug_zsu_get_firmware_block_count(zsu, fwIndex)};
  double progress{0.0};
  unsigned int retry{0uz};
  for (long i{0}; i < blocks; i++) {
    int barWidth = 70;
    std::cout << "[";
    int pos = static_cast<int>(barWidth * progress);
    for (int j{0}; j < barWidth; j++) {
      if (j < pos) std::cout << "=";
      else if (j == pos) std::cout << ">";
      else std::cout << " ";
    }

    if (libklug_mdu_ein_zsu_update(lib, zsu, fwIndex, i, &result) !=
          LIBKLUG_OK ||
        !result) {
      if (retry >= max_retries) {
        std::cout << "Error at block " << i << std::endl;
        return -1;
      }
      retry++;
      i--;
    } else retry = 0;

    progress = static_cast<double>(i + 1) / static_cast<double>(blocks);
    std::cout << "] Progress " << static_cast<int>(progress * 100) << "%";
    std::cout << "\r";
    std::cout.flush();
  }
  std::cout << std::endl;

  if (libklug_mdu_ein_zsu_crc32_start(lib, zsu, fwIndex, &result) !=
        LIBKLUG_OK ||
      !result) {
    std::cout << "Unable to init CRC32 verification" << std::endl;
    return -1;
  }
  std::cout << "CRC32 check started" << std::endl;

  if (libklug_mdu_ein_zsu_crc32_result_exit(lib, &result) != LIBKLUG_OK ||
      !result) {
    std::cout << "Bad CRC32" << std::endl;
    return -1;
  }
  std::cout << "CRC32 check successful" << std::endl;

  return 0;
}
