#include <ulf/c/libulf.h>
#include <chrono>
#include <codecvt>
#include <cstdlib>
#include <iostream>
#include <locale>
#include <paths.hpp>
#include <string_view>
#include "progress_bar.hpp"
#include "setup.hpp"

int main() {
  std::cout << "1. Create Resources and initialize device" << std::endl;
  auto handle{setup::connect()};
  if (!handle) return -1;

  auto z_handle{
    libulf_zpp_read(paths::zpp_path.data(), paths::zpp_path.size())};
  if (!z_handle) return -1;

  long const count{libulf_zpp_blocks(z_handle)};
  bool boolean{}; // Result container (implicit bool)

  std::cout << "2. Change Mode to SUSIV2" << std::endl;
  if (libulf_com_susiv2(handle, &boolean) != LIBULF_OK || !boolean) return -1;

  std::cout << "3. Request features to set transfer rate to fastest"
            << std::endl;
  if (libulf_susiv2_features(handle, &boolean) != LIBULF_OK || !boolean)
    return -1;

  std::cout << "4. Erase decoder flash" << std::endl;
  if (libulf_susiv2_zpp_erase(handle, &boolean) != LIBULF_OK || !boolean)
    return -1;

  std::cout << "5. Write zpp to decoder flash" << std::endl;
  { // Update
    ProgressBar<70uz> bar{};
    for (uint32_t i{0}; i < count; i++) {
      if (libulf_susiv2_zpp_write(handle, z_handle, i, &boolean) != LIBULF_OK ||
          !boolean) {
        std::cout << std::endl << "Error at block " << i << std::endl;
        return -1;
      }
      bar(static_cast<double>(i) / static_cast<double>(count));
    }
  }

  std::cout << "6. Tell decoder to exit ZUSI" << std::endl;
  if (libulf_susiv2_exit(handle, true, true, &boolean) != LIBULF_OK || !boolean)
    return -1;

  std::cout << "7. Reset Update device" << std::endl;
  if (libulf_com_reset(handle, &boolean) != LIBULF_OK || !boolean) return -1;

  std::cout << "8. Release created resources" << std::endl;
  libulf_zpp_release(z_handle);
  setup::disconnect(handle);
  return 0;
}
