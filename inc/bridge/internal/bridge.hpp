#pragma once

#include "bridge_com.hpp"
#include "bridge_context.hpp"
#include "bridge_mdu_ein.hpp"
#include "bridge_susiv2.hpp"
#include "bridge_zpp.hpp"
#include "bridge_zsu.hpp"

namespace bridge {

struct Bridge {

  void loop(); // Loop for worker thread

  int open(uint16_t vid = 0x1FC9u, uint16_t pid = 0x81C1u); 
  int openFd(int Fd);

  int config(); 
  int claim();

  int release(); 
  void close();

  COM& com();
  SUSIV2& susiv2();
private:
  Context _context; 


  COM _com; 
  SUSIV2 _susiv2; 
};

}  // namespace bridge