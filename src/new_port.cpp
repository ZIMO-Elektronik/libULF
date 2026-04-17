#include "new_port.hpp"

#include <cassert>

#include <iostream>

libusb_device_handle* _handle{};

void init() {
  libusb_init(NULL);
}

bool open_klug() {
  assert(!_handle);
  auto handle{libusb_open_device_with_vid_pid(NULL, 0u, 0u)}; 
  if (!handle) return false;
  _handle = handle;

  // Populate endpoints
  libusb_config_descriptor* config{};
  if (libusb_get_active_config_descriptor(_handle->dev, &config)) {
    std::cout << "Could not get config descriptor, exiting...";
    abort();
  }

  libusb_free_config_descriptor(config);

   
  return true; 
}

bool close_klug() {
  if (!_handle) return false;
  libusb_close(_handle);
  return true;
}

const char* ping_klug() {
  return "";
}


