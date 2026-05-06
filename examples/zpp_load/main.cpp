#include <libklug/libklug.h>
#include <chrono>
#include <codecvt>
#include <cstdlib>
#include <iostream>
#include <locale>
#include <string_view>
#include "setup.hpp"

int main() {
  auto handle{setup::connect()};
  if (!handle) abort();

  std::string path{"/home/jonas/Downloads/Da_Di_Collection_ZIMO-16Bit_S02.zpp"};
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
  std::u16string u16_path{convert.from_bytes(path)};

  auto z_handle{libklug_zpp_read(handle, u16_path.data(), u16_path.size())};
  if (!z_handle) abort();

  long const count{libklug_zpp_blocks(handle, z_handle)};

  std::cout << "SUSIV2 mode" << std::endl;
  libklug_com_susiv2(handle);
  auto r{libklug_result(handle)};
  if (r.type != result_type::status) abort();

  std::cout << "SUSIV2 Features" << std::endl;
  libklug_susiv2_features(handle);
  r = libklug_result(handle);
  if (r.type != result_type::status) abort();

  std::cout << "Erase Flash" << std::endl;
  libklug_susiv2_zpp_erase(handle);
  r = libklug_result(handle);
  if (r.type != result_type::status) abort();

  std::cout << "Progress" << std::endl;
  double progress{0.0};

  int i_progress{progress * 100.0};

  for (long i{0}; i < count; i++) {
    // int barWidth = 70;
    // std::cout << "[";
    // int pos = static_cast<int>(barWidth * progress);
    // for (int j{0}; j < barWidth; j++) {
    //   if (j < pos) std::cout << "=";
    //   else if (j == pos) std::cout << ">";
    //   else std::cout << " ";
    // }
    int ti_progress{progress * 100.0};

    std::cout << int(progress * 100.0) << " %";

    auto start{std::chrono::high_resolution_clock::now()};
    libklug_susiv2_zpp_write(handle, z_handle, i);
    auto end{std::chrono::high_resolution_clock::now()};
    auto duration{
      std::chrono::duration_cast<std::chrono::microseconds>(end - start)};
    if (ti_progress > i_progress) {
      std::cout << "Enqueue " << duration.count() << " us;  ";
    }

    start = std::chrono::high_resolution_clock::now();
    r = libklug_result(handle);
    end = std::chrono::high_resolution_clock::now();
    duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    if (ti_progress > i_progress) {
      std::cout << "Receive " << duration.count() << " us";
    }

    if (r.type != result_type::status) {
      std::cout << std::endl << "Error at block " << i << std::endl;
      abort();
    }
    progress = static_cast<double>(i) / static_cast<double>(count);
    i_progress = ti_progress;
    std::cout << "\r";
    std::cout.flush();
  }

  std::cout << std::endl;

  libklug_susiv2_exit(handle, 1, 1);
  r = libklug_result(handle);
  if (r.type != result_type::status) abort();

  libklug_zpp_release(handle, z_handle);
  setup::disconnect(handle);
}
