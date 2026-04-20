#include "new_port.hpp"

#include <cassert>

#include <iostream>
#include <vector>

libusb_device_handle *_handle{};

uint8_t tx_ep, rx_ep;
int interface;

std::array<unsigned char, 64> in_data;

void init()
{
  libusb_init(NULL);
}

bool open_klug()
{
  assert(!_handle);
  auto handle{libusb_open_device_with_vid_pid(NULL, 0x1FC9u, 0x81C1u)};
  if (!handle)
  {
    std::cout << "No ZIMO device found" << std::endl;
    return false;
  }
  _handle = handle;

  // Populate endpoints
  libusb_config_descriptor *config{};
  if (libusb_get_active_config_descriptor(libusb_get_device(_handle), &config))
  {
    std::cout << "Could not get config descriptor, exiting...";
    return false;
  }

  std::vector<uint8_t> tx_eps, rx_eps;

  for (int i = 0; i < config->bNumInterfaces; i++)
  {
    const struct libusb_interface_descriptor *inter_desc = &config->interface[i].altsetting[0];

    for (int k = 0; k < inter_desc->bNumEndpoints; k++)
    {
      const struct libusb_endpoint_descriptor *ep = &inter_desc->endpoint[k];

      // Nur Bulk-Endpunkte beachten
      if ((ep->bmAttributes & 0x03) == LIBUSB_TRANSFER_TYPE_BULK)
      {
        if ((ep->bEndpointAddress & 0x80) == LIBUSB_ENDPOINT_OUT)
        {
          tx_eps.push_back(ep->bEndpointAddress);
          interface = i; // Interface merken für claim_interface
        }
        else
        {
          rx_eps.push_back(ep->bEndpointAddress);
        }
      }
    }
  }

  libusb_free_config_descriptor(config);

  if (tx_eps.size() != 1 || tx_eps.size() != 1)
  {
    std::cout << "Need exactly ONE TX-EP and ONE RX-EP" << std::endl;
    close_klug();
    return false;
  }

  tx_ep = tx_eps.front();
  rx_ep = rx_eps.front();

  std::cout << "On Interface " << interface << " Found " << tx_eps.size() << " TX and " << rx_eps.size() << " RX endpoints" << std::endl;
  std::cout << "TX-EP address: x" << std::hex << static_cast<unsigned int>(tx_ep) << std::dec << std::endl;
  std::cout << "RX-EP address: x" << std::hex << static_cast<unsigned int>(rx_ep) << std::dec << std::endl;

  if (libusb_kernel_driver_active(_handle, interface) == 1)
  {
    std::cout << "Detaching Kernel driver" << std::endl;

    if (libusb_detach_kernel_driver(_handle, interface) == 0)
    {
      std::cout << "Detached Kernel driver" << std::endl;
    }
    else
    {
      std::cout << "Unable to detach driver" << std::endl;
      return false;
    }
  }

  auto r{libusb_claim_interface(_handle, interface)};
  if (r != 0)
  {
    std::cout << "Failed to claim interface: " << r << std::endl;
    return false;
  }

  return true;
}

bool close_klug()
{
  std::cout << "Attempting to close device" << std::endl;
  if (!_handle)
    return false;
  libusb_release_interface(_handle, interface);
  libusb_close(_handle);
  return true;
}

const char *ping_klug()
{
  assert(_handle);
  std::string ping("PING\r");
  int transferred{0};
  auto r{libusb_bulk_transfer(_handle, tx_ep, std::bit_cast<unsigned char *>(ping.data()), ping.size(), &transferred, 0u)};
  if (r != 0)
    std::cout << "Transfer Error " << r << std::endl;

  if (transferred != ping.size())
  {
    std::cout << "Transferred to size mismatch" << std::endl
              << "Expected " << ping.size() << " Actual " << transferred << std::endl;
    return nullptr;
  }

  libusb_bulk_transfer(_handle, rx_ep, in_data.data(), in_data.size(), &transferred, 1000u);

  std::cout << "Received " << transferred << " Bytes" << std::endl
            << "Data: ";

  for (int i{0}; i < transferred; i++)
  {
    std::cout << static_cast<char>(in_data[i]);
  }
  std::cout << std::endl;

  return "";
}
