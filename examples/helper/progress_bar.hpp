#pragma once

#include <iostream>

template<int width>
struct ProgressBar {
  ProgressBar() = default;
  ~ProgressBar() = default;

  void erase() {
    std::cout << "\r";
    for (int i{0}; i < width * 2; i++) std::cout << " ";
    std::cout.flush();
    std::cout << "\r";
    std::cout.flush();
  }

  void operator()(double progress) {
    std::cout << "\rProgress: ";
    std::cout << "[";
    int pos = static_cast<int>(width * progress);
    for (int j{0}; j < width; j++) {
      if (j < pos) std::cout << "=";
      else if (j == pos) std::cout << ">";
      else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %";
    std::cout.flush();
  }
};
