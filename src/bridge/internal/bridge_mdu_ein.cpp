#include "bridge/internal/bridge_mdu_ein.hpp"

namespace bridge {

MDU_EIN::MDU_EIN(Context& ctx, Worker& worker) : _ctx{ctx}, _worker{worker} {}

int MDU_EIN::enterMDU() { return -1; }

int MDU_EIN::enterDCCZSU(uint32_t sn, bool done) { return -1; }

int MDU_EIN::enterDCCZPP(uint32_t id, uint32_t sn, bool done) { return -1; }

int MDU_EIN::cvRead(uint16_t cv) { return -1; }

int MDU_EIN::cvWrite(uint16_t cv, uint8_t value) { return -1; }

int MDU_EIN::ping(uint32_t it, uint32_t sn) { return -1; }

}  // namespace bridge