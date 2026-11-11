#include <ulf/c/libulf.h>
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

int main() {
  std::cout << "1. Create Resources and initialize device" << std::endl;
  auto lib{setup::connect()};
  if (!lib) return -1;

  auto zsu{
    libulf_zsu_read(paths::mx_zsu_path.data(), paths::mx_zsu_path.size())};
  if (!zsu) return -1;

  size_t fwIndex{};
  size_t const maxFwIndex{libulf_zsu_get_firmware_count(zsu) - 1uz};

  bool result{}; // Result container (implicit bool)

  gsl::final_action a([&]() {
    libulf_set_rts(lib, false);
    libulf_set_dtr(lib, false);

    if (libulf_com_reset(lib, &result) != LIBULF_OK || !result) {
      std::cout << "Unable to RESET device" << std::endl;
    }
    libulf_zsu_release(zsu);
    setup::disconnect(lib);
  });

  std::cout << "2. Change Mode to DECUP_EIN" << std::endl;
  if (libulf_com_decup_ein(lib, &result) != LIBULF_OK || !result) {
    std::cout << "Unable to enter DECUP_EIN" << std::endl;
    return -1;
  };

  std::cout << "3. Enable RTS and DTR lines" << std::endl;
  if (libulf_set_rts(lib, true) != LIBULF_OK ||
      libulf_set_dtr(lib, true) != LIBULF_OK) {
    std::cout << "Unable to set RTS state" << std::endl;
    return -1;
  }

  std::cout << "4. Reset decoders to bootloader" << std::endl;
  if (libulf_decup_ein_enter_zsu(lib, &result) != LIBULF_OK || !result) {
    std::cout << "Unable to enter Decoder BL" << std::endl;
    return -1;
  }

  std::cout << "5. Search for decoder" << std::endl;
  bool found{};
  do { // Caution, this assumes at least one firmware in file
    if (libulf_decup_ein_zsu_decoder_id(
          lib, libulf_zsu_get_firmware_id(zsu, fwIndex), &result) !=
        LIBULF_OK) {
      std::cout << "Error during search" << std::endl;
      return -1;
    }

    if (result) {
      found = true;
      break;
    }

  } while (fwIndex++ < maxFwIndex);
  if (found)
    std::cout << " - Found decoder "
              << libulf_zsu_get_firmware_name(zsu, fwIndex) << " with ID "
              << std::hex << libulf_zsu_get_firmware_id(zsu, fwIndex)
              << std::dec << std::endl;
  else {
    std::cout << "Unable to find decoder " << std::endl;
    return -1;
  }

  std::cout << "6. Send page count" << std::endl;
  if (libulf_decup_ein_zsu_page_count(lib, zsu, fwIndex, &result) !=
      LIBULF_OK) {
    std::cout << "Unable to transmit page count" << std::endl;
    return -1;
  }

  std::cout << "7. Send security bytes" << std::endl;
  if (libulf_decup_ein_zsu_security_bytes(lib, &result) != LIBULF_OK) {
    std::cout << "Unable to transmit security bytes" << std::endl;
    return -1;
  }

  std::cout << "8. Write firmware" << std::endl;
  auto const blocks{libulf_zsu_get_firmware_block_count(zsu, fwIndex)};
  {
    ProgressBar<70uz> bar{};
    for (long i{0}; i < blocks; i++) {
      if (libulf_decup_ein_zsu_blocks(lib, zsu, fwIndex, i, &result) !=
            LIBULF_OK ||
          !result)
        return -1;
      bar(static_cast<double>(i + 1) / static_cast<double>(blocks));
    }
  }

  std::cout << "9. Reset" << std::endl;
  return 0;
}
