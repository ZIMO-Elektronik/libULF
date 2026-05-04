# libklug

C / C++ library to communicate with ZIMO KLUG

Based on [Libusb](https://libusb.info/):

- Should cover Windows, Linux, Android & MacOS seamlessly
- For use in Java we should add JNI bindings
- And for React Native even bundle it in a node.js-module

### Setup

#### Linux 
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

#### Android
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