# CHANGELOG

## 0.3.2

- ***Breaking*** Change: Default backend for OSX to libserialport
- Fix: CMake wrapper for libserialport correctly links agains OSX frameworks

## 0.3.1

- ***Breaking*** Change: Rename cmake project to `ULF`
- Fix: Constexpr usage causing problems when compiling for Apple

## 0.3.0

- ***Breaking*** Change: Default backend (excluding Windows) to libusb

## 0.2.1

- Build: Switch from inaccessible cpm dependencies to public equivalent

## 0.2.0

- Add: JNI Java binding plus wrapper
- Add: Gradle project to build lib with jni adapter
- ***Breaking*** Change: Restructure API
- ***Breaking*** Change: Rename to LibULF

## 0.1.0

- ***Breaking*** Change: ZSU firmware iterator api no longer allocates
- ***Breaking*** Change: API no longer is async
- ***Breaking*** Change: Simplify API for opening and closing a connection

## 0.0.2

- Bugfix: Destruction order of LibKLUG leads to SEGFAULT
