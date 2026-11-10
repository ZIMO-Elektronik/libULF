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

  auto zpp{
    libulf_zpp_read(paths::zpp_8bit_path.data(), paths::zpp_8bit_path.size())};
  if (!zpp) return -1;

  bool result{}; // Result container

  gsl::final_action a([&]() {
    libulf_set_rts(lib, false);
    libulf_set_dtr(lib, false);

    if (libulf_com_reset(lib, &result) != LIBULF_OK || !result) {
      std::cout << "Unable to RESET device" << std::endl;
    }
    libulf_zpp_release(zpp);
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
  if (libulf_decup_ein_enter_zpp(lib, &result) != LIBULF_OK || !result) {
    std::cout << "Unable to enter Decoder BL" << std::endl;
    return -1;
  }

  std::cout << "5. Read CV 8 (2x)" << std::endl;
  for (int i{0}; i < 2; i++) {
    int value{};
    if (libulf_decup_ein_cv_read(lib, 7u, &value) != LIBULF_OK) return -1;
    std::cout << " - " << (i == 0 ? "First" : "Second") << " value: " << value
              << std::endl;
  }

  std::cout << "6. Read decoder ID (2x)" << std::endl;
  for (int i{0}; i < 2; i++) {
    int value{};
    if (libulf_decup_ein_zpp_decoder_id(lib, &value) != LIBULF_OK) return -1;
    std::cout << " - " << (i == 0 ? "First" : "Second") << " value: " << value
              << std::endl;
  }

  std::cout << "7. Read CV 7 (2x)" << std::endl;
  for (int i{0}; i < 2; i++) {
    int value{};
    if (libulf_decup_ein_cv_read(lib, 6u, &value) != LIBULF_OK) return -1;
    std::cout << " - " << (i == 0 ? "First" : "Second") << " value: " << value
              << std::endl;
  }

  std::cout << "8. Erase flash" << std::endl;
  if (libulf_decup_ein_zpp_erase_flash(lib, &result) || !result) {
    std::cout << "Unable to erase flash." << std::endl;
    return -1;
  }

  std::cout << "9. Write project" << std::endl;
  auto const blocks{libulf_zpp_block_count(zpp)};
  {
    ProgressBar<70uz> bar{};
    for (long i{0}; i < blocks; i++) {
      if (libulf_decup_ein_zpp_write_flash(
            lib, zpp, static_cast<size_t>(i), &result) != LIBULF_OK ||
          !result)
        return -1;
      bar(static_cast<double>(i + 1) / static_cast<double>(blocks));
    }
    bar.erase();
  }

  std::cout << "10. Write project CV's" << std::endl;
  auto const cv_count{libulf_zpp_cv_count(zpp)};
  {
    ProgressBar<70uz> bar{};
    for (long i{0}; i < cv_count; i++) {
      if (libulf_zpp_cv_address(zpp, i) == 143u)
        continue; // Skip to avoid setting update lock

      if (libulf_decup_ein_cv_write(lib,
                                    libulf_zpp_cv_address(zpp, i),
                                    libulf_zpp_cv_value(zpp, i),
                                    &result) != LIBULF_OK ||
          !result) {
        std::cout << "\tUnable to write CVs to decoder" << std::endl;
        return -1;
      }
      bar(static_cast<double>(i + 1) / static_cast<double>(cv_count));
    }
    bar.erase();
  }

  std::cout << "11. Reset" << std::endl;
  return 0;
}
