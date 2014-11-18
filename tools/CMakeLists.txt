set ( DuckyOS_BINUTILS_VERSION "2.24" )
set ( DuckyOS_GRUB_VERSION "grub-2.02-beta2" )

set ( DuckyOS_BINUTILS_MIRROR "http://ftp.gnu.org/gnu/binutils/" )
set ( DuckyOS_GRUB_MIRROR "http://git.savannah.gnu.org/cgit/grub.git/snapshot/" )

if ( DuckyOS_ARCH STREQUAL "i686" OR DuckyOS_ARCH STREQUAL "amd64" )

  if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/binutils.tar.bz2")
    file( DOWNLOAD
      "${DuckyOS_BINUTILS_MIRROR}binutils-${DuckyOS_BINUTILS_VERSION}.tar.bz2"
      "${CMAKE_CURRENT_SOURCE_DIR}/binutils.tar.bz2"
      SHOW_PROGRESS
      STATUS status
      LOG log)
  endif()

  if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/grub.tar.gz")
    file( DOWNLOAD
      "${DuckyOS_GRUB_MIRROR}grub-${DuckyOS_GRUB_VERSION}.tar.gz"
      "${CMAKE_CURRENT_SOURCE_DIR}/grub.tar.gz"
      SHOW_PROGRESS
      STATUS status
      LOG log)
  endif()

  add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/binutils-${DuckyOS_BINUTILS_VERSION}"
    COMMAND "tar" "-xjf" "${CMAKE_CURRENT_SOURCE_DIR}/binutils.tar.bz2"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/binutils.tar.bz2"
  )

  add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/grub-${DuckyOS_GRUB_VERSION}"
    COMMAND "tar" "-xzf" "${CMAKE_CURRENT_SOURCE_DIR}/grub.tar.gz"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/grub.tar.gz"
  )

  file(
    MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/binutils-${DuckyOS_ARCH}"
  )

  file(
    MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/grub-${DuckyOS_ARCH}"
  )

  add_custom_command (
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/binutils-${DuckyOS_ARCH}/bin/${DuckyOS_ARCH}-elf-ld"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/binutils-${DuckyOS_BINUTILS_VERSION}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/binutils-${DuckyOS_ARCH}"
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/binutils-${DuckyOS_BINUTILS_VERSION}/configure"
	    "--target=${DuckyOS_ARCH}-elf"
	    "--prefix=${CMAKE_CURRENT_BINARY_DIR}/binutils-${DuckyOS_ARCH}"
            "--disable-nls"
    COMMAND "make"
    COMMAND "make" "install" )

  add_custom_command (
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/grub-${DuckyOS_GRUB_VERSION}/configure"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/grub-${DuckyOS_GRUB_VERSION}"
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/grub-${DuckyOS_GRUB_VERSION}/autogen.sh"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/grub-${DuckyOS_GRUB_VERSION}"
  )

  add_custom_command (
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/grub-${DuckyOS_ARCH}/grub-mkrescue"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/grub-${DuckyOS_GRUB_VERSION}/configure"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/grub-${DuckyOS_ARCH}"
    COMMAND "${CMAKE_CURRENT_BINARY_DIR}/grub-${DuckyOS_GRUB_VERSION}/configure"
	    "--target=${DuckyOS_ARCH}-elf"
	    "--prefix=${CMAKE_CURRENT_BINARY_DIR}/grub-${DuckyOS_ARCH}"
            "--disable-nls"
    COMMAND "make"
    COMMAND "make" "install" )

   add_custom_target( tools
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/grub-${DuckyOS_ARCH}/grub-mkrescue"
            "${CMAKE_CURRENT_BINARY_DIR}/binutils-${DuckyOS_ARCH}/bin/${DuckyOS_ARCH}-elf-ld"
   )
  
   set ( CMAKE_C_LINK_EXECUTABLE
    "${CMAKE_CURRENT_BINARY_DIR}/binutils-${DuckyOS_ARCH}/bin/${DuckyOS_ARCH}-elf-ld <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>"
    PARENT_SCOPE )
   set ( CMAKE_CXX_LINK_EXECUTABLE
    "${CMAKE_CURRENT_BINARY_DIR}/binutils-${DuckyOS_ARCH}/bin/${DuckyOS_ARCH}-elf-ld <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>"
    PARENT_SCOPE )

   set ( DuckyOS_MKIMAGE
    "${CMAKE_CURRENT_BINARY_DIR}/grub-${DuckyOS_ARCH}/grub-mkimage"
    PARENT_SCOPE )

endif()