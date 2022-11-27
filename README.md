![workflow](https://github.com/maneljimeno/bootstrap/actions/workflows/ci.yml/badge.svg)
[![GPLv3 License](https://img.shields.io/badge/license-MIT-blue?style=flat)](/LICENSE)


# Bootstrap
A template with a bootstrap C++ project that uses CMake and Conan.
It includes:
- A QT6 application
- A library that implements a proxy class to encapsulate other libraries
- Generation of native installer for MacOS (DMG), Windows (EXE) and Debian (deb)
- Creation of project documentation using Doxygen and Sphinx
- CI/CD with GitHub Actions on all three platforms that stores binary installers when creating a branch **(coming soon)**

## Table of contents

- [Prerequisites](#prerequisites)
  - [Installation on macOS](#installation-on-macos)
  - [Install on Linux](#install-on-linux)
  - [Install on Windows](#install-on-windows)
  - [Install Python dependencies](#install-python-dependencies)
  - [Project customization](#project-customization)
- [Building](#building)

## Prerequisites
To compile this project we need:
- CMake >= 3.22
- Python >= 3.8
- llvm >= 10.0
- The python dependencies on the requirements.txt

### Installation on macOS
```shell
sudo brew install cmake llvm python3-pip
```

### Install on Linux
```shell
sudo apt-get install -y cmake clang-format clang-tidy python3-pip
```

### Install on Windows
Using [winget](https://github.com/microsoft/winget-cli/releases)
```powershell
winget install cmake llvm python
```

Using [chocolatey](https://chocolatey.org/install)
```powershell
choco install cmake llvm python
```

### Install Python dependencies
```shell
python -m pip install -r requierements.txt --upgrade
```

### Project customization
The project contains the project_customization.txt file that allows you to customize the
following values throughout the project:
- PROJECT_NAME=MyProject
- PROJECT_DESCRIPTION=Template CMake project
- APPLICATION_NAME=application
- LIBRARY_NAME=custom_library
- COPYRIGHT_PROJECT=2022, Manel J
- AUTHOR_PROJECT=Manel J.


## Building

The project generates a script file in the ${CMAKE_BINARY_DIR} folder, called project_env.
This file allows the building and runs of the project while we are developing, the installers
setting the OS to avoid this need.

```shell
cmake -S. -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
