# libklug

C / C++ library to communicate with ZIMO KLUG

Based on [Libusb](https://libusb.info/) or [Libserialport](https://sigrok.org/wiki/Libserialport):

- Should cover Windows, Linux, Android & MacOS seamlessly
- For use in Java we should add JNI bindings
- And for React Native even bundle it in a node.js-module

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

Since we rely on libusb as USB backend, the raw traffic of the device needs to be accessible, else we cant open it. To ensure this, we need to add the user to the `plugdev` group.

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

### Android

> [!NOTE]
> After the API change, the JNI is hopelessly broken and in need of fixing

The Native side of the JNI bridge is provided by the library. However, a matching java / kotlin class (e.g. NativeLib and NativeResult) need to be provided by the user. Additionally, some options need to be set for the cmake build.

It is recommended to set these in gradle, so that they are simply added during the app build process.

```gradle
android{
    ... // Your options
    externalNativeBuild {
        cmake {
            path = file("/home/jonas/Development/libklug/CMakeLists.txt")
            version = "3.28.3"
        }
    }
    defaultConfig {
        externalNativeBuild {
            cmake {
              val jni_class_path = "Java_${namespace.replace("_", "_1").replace(".", "_")}_<NativeLib>_"
              val jni_result_path = "${namespace.replace(".", "/")}/<NativeResult>"
              arguments.add("-DJNI_CLASS_PATH=${jni_class_path}")
              arguments.add("-DJNI_RESULT_PATH=\"$jni_result_path\"")
            }
        }
    }
    ... // More options
}
```

This will configure the JNI code to connect to a `NativeLib` class for functionality and a `NativeResult` class for result values. The names can be (almost) freely selected, however as of now, they cannot contain special characters (notably `_`).
