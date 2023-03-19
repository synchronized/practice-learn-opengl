learn from https://learnopengl-cn.github.io/

安装vcpkg

进入项目目录下, 运行:
vcpkg install

cmake -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build
