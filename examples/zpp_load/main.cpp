#include <libklug/libklug.h>
#include <chrono>
#include <codecvt>
#include <cstdlib>
#include <iostream>
#include <locale>
#include <paths.hpp>
#include <string_view>
#include "setup.hpp"

int main() {
  auto handle{setup::connect()};
  if (!handle) abort();

  auto z_handle{
    libklug_zpp_read(paths::zpp_path.data(), paths::zpp_path.size())};
  if (!z_handle) abort();

  long const count{libklug_zpp_blocks(z_handle)};

  libklug_bool boolean{}; // Result container (implicit bool)

  std::cout << "SUSIV2 mode" << std::endl;
  if (libklug_com_susiv2(handle, &boolean) != LIBKLUG_OK || !boolean) abort();

  std::cout << "SUSIV2 Features" << std::endl;
  if (libklug_susiv2_features(handle, &boolean) != LIBKLUG_OK || !boolean)
    abort();

  std::cout << "Erase Flash" << std::endl;
  if (libklug_susiv2_zpp_erase(handle, &boolean) != LIBKLUG_OK || !boolean)
    abort();

  std::cout << "Progress" << std::endl;
  double progress{0.0};

  for (uint32_t i{0}; i < count; i++) {
    int barWidth = 70;
    std::cout << "[";
    int pos = static_cast<int>(barWidth * progress);
    for (int j{0}; j < barWidth; j++) {
      if (j < pos) std::cout << "=";
      else if (j == pos) std::cout << ">";
      else std::cout << " ";
    }

    std::cout << int(progress * 100.0) << " %";

    if (libklug_susiv2_zpp_write(handle, z_handle, i, &boolean) != LIBKLUG_OK ||
        !boolean) {
      std::cout << std::endl << "Error at block " << i << std::endl;
      abort();
    }
    progress = static_cast<double>(i) / static_cast<double>(count);
    std::cout << "\r";
    std::cout.flush();
  }

  std::cout << std::endl;

  if (libklug_susiv2_exit(handle, LIBKLUG_TRUE, LIBKLUG_TRUE, &boolean) !=
        LIBKLUG_OK ||
      !boolean)
    abort();

  if (libklug_com_reset(handle, &boolean) != LIBKLUG_OK || !boolean) abort();

  libklug_zpp_release(z_handle);
  setup::disconnect(handle);
}
