# libklug

C / C++ library to communicate with ZIMO KLUG

Based on [Libusb](https://libusb.info/) or [Libserialport](https://sigrok.org/wiki/Libserialport):

- Should cover Windows, Linux, Android & MacOS seamlessly

## Third-Party Licenses

This project incorporates several open-source libraries. Their licenses and compliance terms are detailed below:

### LGPL Dependencies

- **[libusb](https://github.com/libusb/libusb)**: Licensed under the GNU Lesser General Public License v2.1 or later (LGPL-2.1-or-later).
- **[libserialport](https://sigrok.org/wiki/Libserialport)**: Licensed under the GNU Lesser General Public License v3 or later (LGPL-3.0-or-later).

Compliance Note: These libraries are dynamically linked to satisfy the terms of the LGPL.

### MPL Dependencies

The following libraries are licensed under the **Mozilla Public License 2.0 (MPL-2.0)**:

- **[ULF_COM](https://github.com/ZIMO-Elektronik/ULF_COM)**
- **[ULF_SUSIV2](https://github.com/ZIMO-Elektronik/ULF_SUSIV2)**
- **[ULF_MDU_EIN](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN)**

**Compliance Note for MPL Libraries:**
According to Section 2.4 of the MPL 2.0, these files are compatible with the GNU Lesser General Public License v3 (LGPL-3.0-or-later) and are combined into this project under the terms of the LGPLv3. The original MPL-2.0 licensing headers within these specific files remain untouched.

## Setup

TBD.

### Linux

If you rely on libusb as USB backend, the raw traffic of the device needs to be accessible, else we can't open it. To ensure this, the user needs to be in the `plugdev` group.

```sh
useradd -a -G plugdev $USER
```

Additionally, a udev-rule is needed to add any ZIMO interface to the plugdev group.

```sh
## Add udev-rule
sudo echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="1fc9", ATTR{idProduct}=="81c1", MODE="0660", GROUP="plugdev"' | sudo tee/etc udev/rules.d/99-myusb.rules

## Trigger reload of rules
sudo udevadm control --reload-rules
sudo udevadm trigger
```

### Android / JAVA

This library can be used on android. For more info, please read the [Documentation](platform/android/README.md)

## Typical processes

1. Create libklug `libklug_create`
2. Initialize libklug `libklug_init`
3. Open USB device `libklug_open` or `libklug_openFd`
4. ***Process(-es)***
5. Close USB device `libklug_close`
6. Destroy libklug `libklug_destroy`

The procedure for the protocol / action still apply. More information can be found in the corresponding repositories

- [MDU](https://github.com/ZIMO-Elektronik/MDU) -> Typical process for update and soundload via track. (MDU_EIN)
- [ZUSI](https://github.com/ZIMO-Elektronik/ZUSI) -> Typical process for soundload via the ZUSI protocol. (SUSIV2)