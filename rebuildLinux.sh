set -euo pipefail
set -x

rm -rf build
conan install . --output-folder=build --build=missing
cmake --preset conan-release
cmake --build build --parallel
