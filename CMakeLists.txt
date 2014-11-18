cmake_minimum_required (VERSION 2.8)
project (DuckyOS C CXX)

set (DuckyOS_VERSION 0.0.1)
#set (DuckyOS_ARCH "i686")
set (DuckyOS_ARCH "i686" CACHE STRING "Target architecture")
set_property(CACHE DuckyOS_ARCH PROPERTY STRINGS i686 amd64 arm arm64)
#option( DuckyOS_TARGET_ARCH "Target architecture (x86/amd64/ARM)" "x86" )

add_subdirectory( tools )
add_subdirectory( src )
