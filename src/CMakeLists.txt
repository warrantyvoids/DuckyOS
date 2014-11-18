add_subdirectory( sys )
option( DuckyOS_BUILD_USERLAND "Add the userland to the build (experimental)" OFF)

if (${DuckyOS_BUILD_USERLAND})
  add_subdirectory( usr )
endif()