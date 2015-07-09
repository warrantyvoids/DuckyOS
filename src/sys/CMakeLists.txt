add_compile_options( "-ffreestanding" "-nostdlib" "-mno-sse" "-mno-mmx" )
add_compile_options( "-Wall" "-Wextra" )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )


set( DuckyOS_TARGET_TRIPLE "${DuckyOS_ARCH}-pc-none-eabi" )

set( CMAKE_ASM_COMPILER clang )
set( CMAKE_C_COMPILER   clang )
set( CMAKE_CXX_COMPILER clang )

enable_language( ASM )


set( CMAKE_SHARED_LIBRARY_LINK_C_FLAGS   "" )
set( CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "" )

##DDucks - This does not work yet...
#set( CMAKE_ASM_COMPILER_TARGET  ${DuckyOS_TARGET_TRIPLE} )
#set( CMAKE_C_COMPILER_TARGET   ${DuckyOS_TARGET_TRIPLE} )
#set( CMAKE_CXX_COMPILER_TARGET ${DuckyOS_TARGET_TRIPLE} )
add_compile_options( "-target" "${DuckyOS_TARGET_TRIPLE}" )

list ( APPEND DuckyOS_KERNEL_SRCS kernel.c terminal.c keyboard.c )

add_executable( DuckyOS.bin ${DuckyOS_KERNEL_SRCS} )
set_target_properties( DuckyOS.bin PROPERTIES LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/kernel.ld" )
add_dependencies( DuckyOS.bin tools )
#  "${CMAKE_BINARY_DIR}/tools/binutils-${DuckyOS_ARCH}/bin/${DuckyOS_ARCH}-elf-ld"
#  "${CMAKE_BINARY_DIR}/tools/grub-${DuckyOS_ARCH}/grub-mkrescue" )

if(DuckyOS_ARCH STREQUAL "i686")
  add_subdirectory( arch/i686 )
  target_link_libraries( DuckyOS.bin arch_i686 )
  find_program( QEMU qemu-system-x86 )

endif()



add_definitions( "-DARCH=${DuckyOS_ARCH}" )

