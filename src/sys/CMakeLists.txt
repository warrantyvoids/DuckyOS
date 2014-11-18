if(DuckyOS_ARCH STREQUAL "i686")
  SET( DuckyOS_CC_TARGET "i686-none-eabi" )
  add_subdirectory( arch/i686 )
endif()

add_definitions( ARCH=${DuckyOS_ARCH} )
