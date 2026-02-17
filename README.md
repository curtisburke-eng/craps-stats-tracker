# Craps Stats Tracker (CST)
A companion app for an in-home Craps table; Used to track players win/loss, average rolls per shoot, and more.

**Contents:**

- Showcase
- Prerequisites
- Building
- MVVM Architechture

## Showcase
// TODO: Update with screenshots

## Prerequisites
- vckpg
- direnv
- cmake
- Ninja

### direnv
[direnv](https://direnv.com/) is a shell-agnostic environment variable manager that automatically loads and unloads environment variables based on your current directory.
To install: use global package manager (i.e. apt).
```bash
sudo apt update && sudo apt install direnv -y
```

### vcpkg
[vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/overview) is used to manage dependencies. vcpkg uses the system's globally installed build tools. That means any dependencies that are built with vcpkg require you to have the build dependencies installed locally. If a vcpkg dependency fails to build, it is almost certainly because you don't have the build dependency installed locally.
Ideally, a Docker image should be used to manage the vcpkg build dependencies and build environment. **TODO:** This has not been set up yet.
To build the project, the current CMake preset requires you to set:
`VCPKG_ROOT=`

vcpkg can be installed by cloning it from GitHub and running the bootstrap script:
```bash
git clone https://github.com/microsoft/vcpkg.git
```
```bash
cd vcpkg
```
```bash
./bootstrap-vcpkg.sh -disableMetrics
```
Then update the `.envrc` file with the path to the vcpkg directory
```
export VCPKG_ROOT=/path/to/vcpkg // Update this line
...
```
Now, navigate out of the project directory and back in, then allow direnv to update the PATH
```bash
cd ..; cd -; direnv allow
```
Now each time this projecto directory is accessed direnv automatically adds vcpkg to the PATH

#### Build Dependency Hints
The vcpkg build log will indicate missing dependencies if a build fails.

### cmake
[cmake](https://cmake.org/cmake/help/latest/manual/cmake.1.html) is used to generate the build files.
To install: use global package manager (i.e. apt).
```bash
sudo apt update && sudo apt install cmake
```

### Ninja
[Ninja](https://github.com/ninja-build/ninja) is used as the build system.
To install: use global package manager (i.e. apt).
```bash
sudo apt update && sudo apt install ninja-build
```

## Building
This app can be built in two ways: by manually invoking cmake and ninja or by using vcpkg options.

### With vcpkg
1. Generate cmake preset
```bash
cmake --preset=default
```
2. Build
```bash
cmake --build build
```
3. Run the application
```bash
./build/craps-stats-tracker
```

### Manual building (alternatively)
1. Generate the cmake files using Ninja as the builder, `./build` as the build directory, and the current directory as the source
```bash
cmake -G Ninja -B build -S .
```
2. Navigate into the build directory and invoke Ninja
```bash
cd build; ninja
```
3. Run the application
```bash
./craps-stats-tracker
```

## MVVM Architecture

This application is intended to use MVVM architecture.

### Model

- Standalone application, does not use a frontend.
- Contains all business logic.
- Provides a callback registration system for updates to system state.
- Can be driven by a command line interface, GUI, or any other input method.

### ViewModel

- Constructor takes a Model and View instance.
- Translates inputs from the View into model commands, e.g., via slots if using Qt.
- Registers with the model’s callback system and generates Qt signals as the model updates.

### View

- The GUI and user input layer.
- Uses a ViewModel and interacts only with it.
- Connects signals and slots to the ViewModel (if using Qt).

View ──────> ViewModel ──────> Model
