project(peripheral.xarcade)

cmake_minimum_required(VERSION 2.6)
enable_language(CXX)

# --- Add-on Dependencies ------------------------------------------------------

find_package(Kodi REQUIRED)

include_directories(${INCLUDES}
                    ${PROJECT_SOURCE_DIR}/src
                    ${KODI_INCLUDE_DIR}
)

set(XARCADE_SOURCES
  src/xarcade/XArcadeDevice.cpp
  src/xarcade/XArcadeScanner.cpp
  src/xarcade/XArcadeUtils.cpp
  src/addon.cpp
)

# ------------------------------------------------------------------------------

build_addon(${PROJECT_NAME} XARCADE DEPLIBS)

include(CPack)
