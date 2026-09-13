# algosee

## To compile the Project:  
0. `cmake --version` >= **4.4.3**  
   `gcc --version` >= **16.2.1 20260810**  
   `conan --version` >= **2.32.0**
1. `conan install . --output-folder=build --build=missing` to install dependencies  
2. `cmake -S . -B build \ -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake` to configure build files with conan dependencies  
3. `cmake --build build` to build 

