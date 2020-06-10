# Monju = Lua + Sol + JUCE

An experimental environment for building audio/visual applications with the [Lua](https://github.com/lua/lua) language, [Sol](https://github.com/ThePhD/sol2/) for C++ bindings, and [JUCE](https://github.com/WeAreROLI/JUCE).

## Install

```sh
git clone --recursive https://github.com/eliot-akira/monju.git
cd monju
```

## Build

### FRUT and CMake

Use the included [FRUT](https://github.com/McMartin/FRUT) builder to generate `CMakeList.txt`, and build with `cmake`.

There is a small shell script named `build` in the root folder for this purpose.

First build `FRUT` itself. This only needs to be done once per install.

```sh
./build frut
```

Then build the app.

```sh
./build app
```

The result will be in the folder `builds`.

For debug build:

```sh
./build debug
```

### Project files

If you have Projucer, use the provided project file at `libs/monju.jucer`.

#### Generated projects

The following are generated from Projucer.

For XCode on macOS, open `builds/mac/monju.xcodeproj`.

For Visual Studio on Windows, open `builds/win/monju_App.vcxproj`.

For Linux, open `builds/linux/Makefile`.
