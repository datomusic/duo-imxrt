./clean.sh

BUILD_TYPE="$1"

if [[ -z "$BUILD_TYPE" ]]; then
  BUILD_TYPE=debug
fi

TOOLCHAIN_DIR="${TOOLCHAIN_DIR:/usr}"

cmake -DTOOLCHAIN_DIR="$TOOLCHAIN_DIR" -DCMAKE_TOOLCHAIN_FILE="./arm-none-eabi-gcc.cmake" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$BUILD_TYPE  .
make -j 2>&1 | tee build_log.txt
