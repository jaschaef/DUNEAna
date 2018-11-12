#################################################
cmake_minimum_required( VERSION 2.6 FATAL_ERROR )
#################################################

# ----- project name -----
PROJECT( DuneAna )

set( ${PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_PATCH 0 )

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo
MinSizeRel."
       FORCE )
endif()

if( CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" )
  set( CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}" )
endif()

# write this variable to cache
set( CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" CACHE PATH "Where to install ${PROJECT_NAME}" FORCE )

set( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
mark_as_advanced( CMAKE_INSTALL_RPATH )

#append link pathes to rpath list
set( CMAKE_INSTALL_RPATH_USE_LINK_PATH 1 )
mark_as_advanced( CMAKE_INSTALL_RPATH_USE_LINK_PATH )

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS Core Hist Rint HistPainter Graf Graf3d MathCore Net RIO Tree Spectrum )

find_package(EDepSim)

message ( STATUS "Root found in $ENV{ROOTSYS}" )

# ----- Add source directory -----
add_subdirectory( ./source )
