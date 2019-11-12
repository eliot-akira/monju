# Monju = Lua + Sol + JUCE

An experimental framework for building audio/visual applications with the [Lua](https://github.com/lua/lua) language, [Sol](https://github.com/ThePhD/sol2/) for C++ bindings, and [JUCE](https://github.com/WeAreROLI/JUCE).

## Install

```sh
git clone --recursive https://github.com/eliot-akira/monju.git
cd monju
```

## Build

### Projucer

If you have Projucer, use the provided project file at `libs/monju.jucer`, and export to an IDE of choice.

### FRUT and CMake

Alternatively, use the included [FRUT](https://github.com/McMartin/FRUT) builder and build with `cmake`.

First build `FRUT` itself. This is a builder that exports a Projucer file to `CMakeList.txt`.

```sh
./build frut
```

Then build the app.

```sh
./build app
```

For debug build:

```sh
./build debug
```
