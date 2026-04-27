#include "bridge/bridge.hpp"
#include "bridge/internal/bridge.hpp"

bridge_handle bridge_create(void) {
  return reinterpret_cast<bridge_instance*>(new bridge::Bridge());
}

void bridge_destroy(bridge_handle handle) {
  delete reinterpret_cast<bridge::Bridge*>(handle);
}

int bridge_init(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->init();
}

int bridge_open(bridge_handle handle, uint16_t vid, uint16_t pid) {
  return reinterpret_cast<bridge::Bridge*>(handle)->open(vid, pid);
}

int bridge_openFd(bridge_handle handle, int Fd) {
  return reinterpret_cast<bridge::Bridge*>(handle)->openFd(Fd);
}

int bridge_configure(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->config();
}

int bridge_claim(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->claim();
}

int bridge_release(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->release();
}

void bridge_close(bridge_handle handle) {
  return reinterpret_cast<bridge::Bridge*>(handle)->close();
}

int bridge_com_ping(bridge_handle handle, char* buffer, std::size_t length) {
  return reinterpret_cast<bridge::Bridge*>(handle)->com().ping(buffer, length);
}