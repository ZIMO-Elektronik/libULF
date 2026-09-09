#include <klug/c/libklug.h>
#include <chrono>
#include <codecvt>
#include <cstdlib>
#include <gsl/gsl>
#include <iostream>
#include <locale>
#include <string_view>
#include <thread>
#include "paths.hpp"
#include "progress_bar.hpp"
#include "setup.hpp"

using namespace std::chrono_literals;

constexpr unsigned int max_retries{5uz};

int main() {
  std::cout << "1. Create Resources and initialize device" << std::endl;
  auto lib{setup::connect()};
  if (!lib) return -1;

  auto zsu{libklug_zsu_read(paths::zsu_path.data(), paths::zsu_path.size())};
  if (!zsu) return -1;

  size_t fwIndex{};
  size_t const maxFwIndex{libklug_zsu_get_firmware_count(zsu) - 1uz};

  bool result{}; // Result container (implicit bool)

  gsl::final_action a([&]() {
    if (libklug_com_reset(lib, &result) != LIBKLUG_OK || !result) {
      std::cout << "Unable to RESET device" << std::endl;
    }
    libklug_zsu_release(zsu);
    setup::disconnect(lib);
  });

  std::cout << "2. Change Mode to MDU_EIN" << std::endl;
  if (libklug_com_mdu_ein(lib, &result) != LIBKLUG_OK || !result) {
    std::cout << "Unable to enter MDU_EIN" << std::endl;
    return -1;
  };

  std::cout << "3. Reset decoders to bootloader" << std::endl;
  if (libklug_mdu_ein_enter_mdu(lib, &result) != LIBKLUG_OK || !result) {
    std::cout << "Unable to enter Decoder BL" << std::endl;
    return -1;
  }

  std::cout << "4. Configure transfer rate (maximum for update is `SLOW`)"
            << std::endl;
  if (libklug_mdu_ein_config_transfer_rate(lib, 3, &result) != LIBKLUG_OK ||
      !result) {
    std::cout << "Unable to set Transfer Rate" << std::endl;
    return -1;
  }

  std::cout << "5. Search for decoders";
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
    std::cout << " - Found decoder "
              << libklug_zsu_get_firmware_name(zsu, fwIndex) << " with ID "
              << std::hex << libklug_zsu_get_firmware_id(zsu, fwIndex)
              << std::dec << std::endl;
  else {
    std::cout << "Unable to find decoder " << std::endl;
    return -1;
  }

  std::cout << "6. Initialize encryption / decryption" << std::endl;
  if (libklug_mdu_ein_zsu_salsa20_iv(lib, zsu, fwIndex, &result) || !result) {
    std::cout << "Unable to init Salsa20" << std::endl;
    return -1;
  }

  std::cout << "8. Erase decoder flash" << std::endl;
  if (libklug_mdu_ein_zsu_erase(lib, zsu, fwIndex, &result) != LIBKLUG_OK ||
      !result) {
    std::cout << "Unable to erase flash" << std::endl;
    return -1;
  }

  for (int i{0}; i < 10; i++) { // Loop for 10s
    libklug_mdu_ein_busy(lib, &result);
    std::this_thread::sleep_for(1000ms);
  }

  std::cout << "9. Write update to decoder" << std::endl;
  long const blocks{libklug_zsu_get_firmware_block_count(zsu, fwIndex)};
  {
    ProgressBar<70uz> bar{};
    for (long i{0}; i < blocks; i++) {
      if (libklug_mdu_ein_zsu_update(lib, zsu, fwIndex, i, &result) !=
            LIBKLUG_OK ||
          !result)
        return -1;
      bar(static_cast<double>(i + 1) / static_cast<double>(blocks));
    }
  }

  std::cout << "10. Start CRC32 verification" << std::endl;
  if (libklug_mdu_ein_zsu_crc32_start(lib, zsu, fwIndex, &result) !=
        LIBKLUG_OK ||
      !result) {
    std::cout << "Unable to init CRC32 verification" << std::endl;
    return -1;
  }

  std::cout << "11. Check CRC32 verification result and reset decoders"
            << std::endl;
  if (libklug_mdu_ein_zsu_crc32_result_exit(lib, &result) != LIBKLUG_OK ||
      !result) {
    std::cout << "Bad CRC32" << std::endl;
    return -1;
  }

  std::cout << "12. Release created resources and reset device" << std::endl;
  return 0;
}
