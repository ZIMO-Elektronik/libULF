# libklug

C library to communicate with ZIMO KLUG

Based on Libserialport by sigrok:
https://sigrok.org/wiki/Libserialport

- Should cover Windows, Linux, Android & MacOS seamlessly
- For use in Java we should add JNI bindings
- And for React Native even bundle it in a node.js-module

### Setup

#### Linux 
Since we rely on libUSB as USB backend, the raw traffic of the device needs to be accessible, else we cant open it. To ensure this, we need to add the user to the `plugdev` group. 

```sh
useradd -a -G plugdev $USER
```

Additionally, a udev-rule is needed to add any ZIMO interface to the plugdev group. 

```sh
## Add udev-rule
sudo echo 'SUBSYSTEM=="usb", ATTR{idVendor}=="1fc9", ATTR{idProduct}=="81c1", MODE="0660", GROUP="plugdev"' | sudo tee /etc udev/rules.d/99-myusb.rules

## Trigger reload of rules
sudo udevadm control --reload-rules
sudo udevadm trigger
```