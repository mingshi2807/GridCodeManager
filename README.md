# GridCodeManager Enabler

This is a temporary WiP repository to prepare AC V2G EVSE SUC implementation with Grid support (code)
and Energy services and electrical Protection features

This implementation shall respect maximum software modularity and within Solid design principle.  


# Todo list
- [x] Add `Gabarit` gauge utilities codes to generate grid code data stocked locally within csv formatting
- [x] Add unit test to confirm the accuracy convergence limit
- [x] Add core grid code value / class layout design
- [x] Add some numerical math algebra support
- [ ] Add first time performance profiling support, thread safe improvement in future work
- [x] Add event driven (observer) design to notify grid code related event and required service toward to setpoint computing entity
- [ ] Add factory pattern to create abstracted grid service manager faced to up or downstream components (ex. metering, iEMS)
- [ ] Separate concerns of grid code type, criticality and associated energy services by strategy design pattern
- [ ] Mermaid UML functional sequence scheme
- [ ] Simplified SUC logic charts
- [ ] Software architecture integration scheme within Charge Point firmware
- [ ] Functional unitary workflow specification list for testability purpose
- [x] Build system improvement by Cmake
- [x] Add conan packages dependencies manager
- [ ] CI build and test with Cmake workflow
- [ ] Software packaging improvement by Cmake tooling Cpack
- [ ] Compare or benchmark software packaging approach (Cpack, Devcontainer)
- [ ] Improve the code design or smells by dependencies inversion and SOLID principle

# Build

### Prepare venv python build env.

```shell
python3 -m venv conan
source conan/bin/activate.fish
pip install conan==1.62.0
```

### Configure Conan profile

Suppose we use compiler type : gcc with C++17
```shell
conan profile new default --detect
conan profile update settings.compiler.libcxx=libstdc++11 default
```

### Install dependencies
Suppose we are at root project path and build mode is Debug
```shell
mkdir build
conan install . -s build_type=Debug --install-folder=build
```

### Compilation
Suppose we use `CMAKE_BINARY_DIR` (build folder here)
```shell
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build --config Debug
```
Then, the binary file shall be this path : `/build/bin/`

ps : we will improve this build procedure by env. variable in yaml file of our CI pipeline gitlab env.



