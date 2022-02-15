cmake_minimum_required (VERSION 3.21)
project (Gates)

set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

file(GLOB_RECURSE Gates_src
     "src/*.hpp"
     "src/*.cpp"
)

add_executable(Gates ${Gates_src})

add_compile_options(
  -Wall -Werror -Werror -Wno-unused-parameter
   $<$<CONFIG:RELEASE>:-Ofast>
   $<$<CONFIG:RELEASE>:-flto>
   $<$<CONFIG:DEBUG>:-O0>
   $<$<CONFIG:DEBUG>:-ggdb3>
)

add_compile_definitions(
  $<$<CONFIG:RELEASE>:NDEBUG>
)

set_property(TARGET Gates PROPERTY CXX_STANDARD 20)
set_property(TARGET Gates PROPERTY C_STANDARD 17)

target_include_directories(Gates PRIVATE include)
target_include_directories(Gates PRIVATE lib)

# Libraries

add_subdirectory(lib/pixel EXCLUDE_FROM_ALL)
target_link_libraries(Gates PRIVATE pixel)
