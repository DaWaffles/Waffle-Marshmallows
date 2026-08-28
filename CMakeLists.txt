cmake_minimum_required(VERSION 3.15)
project(WaffleMallows CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_TOOLCHAIN_FILE "$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake")

find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(WaffleMallows main.cpp)

target_link_libraries(WaffleMallows PRIVATE fmt::fmt glfw)
target_link_libraries(WaffleMallows PRIVATE glad::glad)
