#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace transmission
{

  struct Transmission
  {
    Transmission(std::string payload, std::size_t timeout);
    Transmission(std::span<const uint8_t> payload, std::size_t timeout);

    int execute();

    std::span<uint8_t> result();

    /// \todo Replace with pure virtual
    virtual bool evaluate() { return true; }

  protected:
    int transmit();
    int receive();

    // virtual bool evaluate() = 0;
    // virtual void notify() = 0;

    std::vector<uint8_t> _payload;
    std::vector<uint8_t> _response;
    std::size_t _timeout;

    void flush();
  };

} // namespace transmission