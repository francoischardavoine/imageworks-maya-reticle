cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0074 NEW) # recognize <Package>_Root variables
project(spReticleLoc VERSION $ENV{SPDEV_COMPONENT_VERSION})

find_package(Maya REQUIRED)

set(PLUGIN_NAME "spReticleLoc")

#if (DEFINED ENV{SPK_PKG_VERSION})
#    message(STATUS "SPK_PKG_VERSION: $ENV{SPK_PKG_VERSION}")
#    add_definitions(-DPACKAGE_VERSION=$ENV{SPK_PKG_VERSION})
#endif()

# enable for debugging mode
set(CMAKE_BUILD_TYPE Debug)

# Needed by some Maya include files:
add_definitions(-DLINUX)

add_definitions(-DNDEBUG)
add_definitions(-O3)

file(GLOB SPRETICLELOC_SOURCES *.cpp)

add_library(
    ${PLUGIN_NAME}
    SHARED
    ${SPRETICLELOC_SOURCES}
)

target_link_libraries(
    ${PLUGIN_NAME}
    PRIVATE
    Maya::OpenMaya
    Maya::OpenMayaAnim
    Maya::OpenMayaUI
)

target_include_directories(${PLUGIN_NAME} PRIVATE ${LIBS_INCLUDE})

install(FILES
    spReticleLoc.mel
    spReticleLocWindows.mel
    AEspReticleLocTemplate.mel
    DESTINATION scripts/maya/spReticleLoc
)

set_target_properties(${PLUGIN_NAME} PROPERTIES PREFIX "")

install(TARGETS ${PLUGIN_NAME} DESTINATION plugins/maya)

