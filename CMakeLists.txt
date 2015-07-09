cmake_minimum_required (VERSION 2.8)
project (DuckyOS C CXX)

set (DuckyOS_VERSION 0.0.1)
set (DuckyOS_ARCH "i686" CACHE STRING "Target architecture")
set_property(CACHE DuckyOS_ARCH PROPERTY STRINGS i686 amd64 arm arm64)
option (DuckyOS_BUILD_TOOLS "Download + Build the tools required to compile DuckyOS" ON)

if (${DuckyOS_BUILD_TOOLS})
  add_subdirectory( tools )
endif()
add_subdirectory( src )

file( MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/img/boot/grub )
file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/img/boot/grub/grub.cfg
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/img/boot/grub )

add_custom_target( image DEPENDS DuckyOS.iso )
add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/img/boot/DuckyOS.bin
  COMMAND cp "${CMAKE_CURRENT_BINARY_DIR}/src/sys/DuckyOS.bin" "${CMAKE_CURRRENT_BINARY_DIR}/img/boot/DuckyOS.bin"
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/src/sys/DuckyOS.bin)
add_custom_command( OUTPUT DuckyOS.iso 
  COMMAND ${DuckyOS_MKIMAGE} "-o" "DuckyOS.iso" "${CMAKE_CURRENT_BINARY_DIR}/img"
  DEPENDS  ${CMAKE_CURRENT_BINARY_DIR}/img/boot/grub
           ${CMAKE_CURRENT_BINARY_DIR}/img/boot/DuckyOS.bin
           ${CMAKE_CURRENT_BINARY_DIR}/img/boot/grub/grub.cfg )

if (${DuckyOS_ARCH} STREQUAL i686)
  find_program( QEMU "qemu-system-i386" )
elseif(${DuckyOS_ARCH} STREQUAL amd64 )
  find_program( QEMU "qemu-system-x86_64" )
endif()

if (NOT ${QEMU} STREQUAL "QEMU-NOTFOUND" )
  add_custom_target( simulate DEPENDS DuckyOS.iso
                     COMMAND ${QEMU} "-cdrom" DuckyOS.iso )
endif()