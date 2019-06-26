# ![xeus](docs/source/xeus.svg)

[![Travis](https://travis-ci.org/QuantStack/xeus.svg?branch=master)](https://travis-ci.org/QuantStack/xeus)
[![AppVeyor](https://ci.appveyor.com/api/projects/status/5alkw5iiere4mox2?svg=true)](https://ci.appveyor.com/project/QuantStack/xeus)
[![Documentation Status](http://readthedocs.org/projects/xeus/badge/?version=latest)](https://xeus.readthedocs.io/en/latest/?badge=latest)
[![Join the Gitter Chat](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/QuantStack/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

C++ implementation of the Jupyter Kernel protocol

## Introduction

`xeus` is a library meant to facilitate the implementation of kernels for Jupyter. It takes the
burden of implementing the Jupyter Kernel protocol so developers can focus on implementing the
interpreter part of the kernel.

Several Jupyter kernels are built upon xeus, such as [`xeus-cling`](https://github.com/QuantStack/xeus-cling),
a kernel for the C++ programming language, and [`xeus-python`](https://github.com/QuantStack/xeus-python), an alternative Python kernel for Jupyter.

## Installation

`xeus` has been packaged on all platforms for the conda package manager.

```
conda install xeus -c conda-forge
```

## Documentation

To get started with using `xeus`, check out the full documentation

http://xeus.readthedocs.io/

## Usage

`xeus` enables custom kernel authors to implement Jupyter kernels more easily. It takes the burden of implementing the Jupyter Kernel protocol so developers can focus on implementing the interpreter part of the Kernel.

The easiest way to get started with a new kernel is to inherit from the base interpreter class `xeus::xinterpreter` and implement the private virtual methods:

- `execute_request_impl`
- `complete_request_impl`
- `inspect_request_impl`
- `is_complete_request_impl`

as seen in the [documentation](http://xeus.readthedocs.io/).


```cpp
#include "xeus/xinterpreter.hpp"

#include "nlohmann/json.hpp"

using xeus::xinterpreter;

namespace nl = nlohmann;

namespace custom
{
    class custom_interpreter : public xinterpreter
    {

    public:

        custom_interpreter() = default;
        virtual ~custom_interpreter() = default;

    private:

        void configure() override;

        nl::json execute_request_impl(int execution_counter,
                                      const std::string& code,
                                      bool silent,
                                      bool store_history,
                                      const nl::json::node_type* user_expressions,
                                      bool allow_stdin) override;

        nl::json complete_request_impl(const std::string& code,
                                       int cursor_pos) override;

        nl::json inspect_request_impl(const std::string& code,
                                      int cursor_pos,
                                      int detail_level) override;

        nl::json is_complete_request_impl(const std::string& code) override;

        nl::json kernel_info_request_impl() override;
    };
}
```

Kernel authors can then rebind to the native APIs of the interpreter that is being interfaced, providing richer information than with the classical approach of a wrapper kernel capturing textual output.

## Building from Source

`xeus` depends on the following libraries: [`libzmq`](https://github.com/zeromq/libzmq),
[`cppzmq`](https://github.com/zeromq/cppzmq), [`OpenSSL`](https://github.com/openssl/openssl),
[`nlohmann_json`](https://github.com/nlohmann/json), and [`xtl`](https://github.com/QuantStack/xtl).

|  xeus  | libzmq  | cppzmq  |   xtl          | nlohmann json | OpenSSL |    cryptopp    |
|--------|---------|---------|----------------|---------------|---------|----------------|
| master |  ^4.2.5 |  ^4.3.0 | >=0.5.0,<0.7.0 |      ^3.2.0   |  ^1.0.1 |  not required  |
| 0.20.0 |  ^4.2.5 |  ^4.3.0 | >=0.5.0,<0.7.0 |      ^3.2.0   |  ^1.0.1 |  not required  |
| 0.19.2 |  ^4.2.5 |  ^4.3.0 | >=0.5.0,<0.7.0 |      ^3.2.0   |  ^1.0.1 |  not required  |
| 0.19.1 |  ^4.2.5 |  ^4.3.0 | >=0.5.0,<0.7.0 |      ^3.2.0   |  ^1.0.1 |  not required  |
| 0.19.0 |  ^4.2.5 |  ^4.3.0 | >=0.5.0,<0.7.0 |      ^3.2.0   |  ^1.0.1 |  not required  |
| 0.18.1 |  ^4.2.5 |  ^4.3.0 | ^0.5.0         |      ^3.2.0   |         |         ^7.0.0 |
| 0.18.0 |  ^4.2.5 |  ^4.3.0 | ^0.5.0         |      ^3.2.0   |         |         ^7.0.0 |
| 0.17.0 |  ^4.2.5 |  ^4.3.0 | ^0.5.0         |      ^3.2.0   |         |         ^7.0.0 |
| 0.16.0 |  ^4.2.5 |  ^4.3.0 | ^0.4.0         |       3.2.0   |         |         ^7.0.0 |
| 0.15.0 |  ^4.2.5 |  ^4.3.0 | ^0.4.0         |       3.2.0   |         | ^5.6.5, ^7.0.0 |
| 0.14.1 |  ^4.2.5 |  ^4.3.0 | ^0.4.0         |       3.1.2   |         | ^5.6.5, ^7.0.0 |
| 0.14.0 |  ^4.2.5 |  ^4.3.0 | ^0.4.0         |       3.1.2   |         | ^5.6.5, ^7.0.0 |
| 0.13.0 |  ^4.2.3 |  ^4.2.5 | ^0.4.0         |       3.1.1   |         | ^5.6.5, ^7.0.0 |

On Linux platforms, `xeus` also requires `libuuid`, which is available in all linux distributions (`uuid-dev` on Debian).

We have packaged all these dependencies for the conda package manager. The simplest way to install them with
conda is to run:

```bash
conda install cmake pkg-config zeromq cppzmq xtl OpenSSL -c conda-forge
conda install nlohmann_json -c conda-forge/label/gcc7
```

On Linux platforms, you will also need:

```bash
conda install util-linux -c conda-forge
```

Once you have installed the dependencies, you can build and install `xeus`:

```bash
cmake -D CMAKE_BUILD_TYPE=Release
make
make install
```

## Installing the Dependencies from Source

The dependencies can also be installed from source. Simply clone the directories and run the following cmake (cmake >= 3.8)  and make instructions.

### libzmq

[libzmq](https://github.com/zeromq/libzmq):

```bash
cmake -D WITH_PERF_TOOL=OFF -D ZMQ_BUILD_TESTS=OFF -D ENABLE_CPACK=OFF
-D CMAKE_BUILD_TYPE=Release
make
make install
```

### cppzmq

[cppzmq](https://github.com/zeromq/cppzmq) is a header only library:

```bash
cmake -D CMAKE_BUILD_TYPE=Release
make install
```

### json for modern cpp

[nlohmann_json](https://github.com/nlohmann/json) is a header only library

```bash
cmake
make install
```

### xtl

[xtl](https://github.com/QuantStack/xtl) is a header only library:

```bash
cmake -D CMAKE_BUILD_TYPE
make install
```

## License

We use a shared copyright model that enables all contributors to maintain the
copyright on their contributions.

This software is licensed under the BSD-3-Clause license. See the [LICENSE](LICENSE) file for details.
