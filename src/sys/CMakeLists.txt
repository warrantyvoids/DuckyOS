add_compile_options( "-ffreestanding" "-nostdlib" "-mno-sse" "-mno-mmx" )
add_compile_options( "-Wall" "-Wextra" )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

set( DuckyOS_TARGET_TRIPLE "${DuckyOS_ARCH}-pc-none-eabi" )

set( CMAKE_AS_COMPILER  clang )
set( CMAKE_C_COMPILER   clang )
set( CMAKE_CXX_COMPILER clang )

set( CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "" )
set( CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "" )

##DDucks - This does not work yet...
#set( CMAKE_AS_COMPILER_TARGET  ${DuckyOS_TARGET_TRIPLE} )
#set( CMAKE_C_COMPILER_TARGET   ${DuckyOS_TARGET_TRIPLE} )
#set( CMAKE_CXX_COMPILER_TARGET ${DuckyOS_TARGET_TRIPLE} )
add_compile_options( "-target" "${DuckyOS_TARGET_TRIPLE}" )

add_executable( DuckyOS.bin kernel.c terminal.c keyboard.c )
add_dependencies( DuckyOS.bin tools )

if(DuckyOS_ARCH STREQUAL "i686")
  add_subdirectory( arch/i686 )
endif()

add_definitions( "-DARCH=${DuckyOS_ARCH}" )

