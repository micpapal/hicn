# Define a few configuration variables that we want accessible in the software

include(BuildMacros)
configure_file(config.h.in config.h @ONLY)

if(NOT ANDROID_API AND NOT COMPILE_FOR_IOS)
  add_subdirectory(command_line)
endif ()

add_subdirectory(config)
add_subdirectory(content_store)
add_subdirectory(core)
add_subdirectory(api)
add_subdirectory(io)
add_subdirectory(messenger)
add_subdirectory(platforms)
add_subdirectory(processor)
add_subdirectory(socket)
add_subdirectory(strategies)
add_subdirectory(utils)

list(APPEND HEADER_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux" AND ENABLE_PUNTING)
  list(APPEND COMPILER_DEFINITIONS
    "-DPUNTING"
  )
endif()

list(APPEND COMPILER_DEFINITIONS
  "-DWITH_MAPME -DWITH_MAPME_FIXES"
)

list(INSERT HICN_LIGHT_INCLUDE_DIRS 0
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)

build_library(${LIBHICN_LIGHT}
  STATIC NO_DEV
  SOURCES ${SOURCE_FILES}
  INSTALL_HEADERS ${TO_INSTALL_HEADER_FILES}
  LINK_LIBRARIES ${LIBRARIES}
  DEPENDS ${DEPENDENCIES}
  COMPONENT ${LIBHICN_LIGHT}
  INCLUDE_DIRS ${HICN_LIGHT_INCLUDE_DIRS}
  INSTALL_ROOT_DIR hicn
  DEFINITIONS ${COMPILER_DEFINITIONS}
)

