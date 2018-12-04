#################################################
cmake_minimum_required( VERSION 2.6 FATAL_ERROR )
#################################################

# ----- project name -----
PROJECT( DuneNDEcalReco )

set( ${PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_PATCH 0 )

message( "cmake base dir: ${CMAKE_SOURCE_DIR}" )

list( APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS} )
find_package( ROOT REQUIRED COMPONENTS Geom )

include_directories ( "${CMAKE_SOURCE_DIR}/source/" )
include_directories ( "${ROOT_INCLUDE_DIRS}" )

file ( GLOB HEADER_FILES "source/*.h" ) 
file ( GLOB SOURCE_FILES "source/*.cxx" ) 

message (" CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}" )
message (" header files: ${HEADER_FILES}" )
message (" source files: ${SOURCE_FILES}" )
message (" ROOT_LIBRARIES: ${ROOT_LIBRARIES}" )
 
add_library( DuneNDEcalRecoLib SHARED ${HEADER_FILES} ${SOURCE_FILES} )
target_link_libraries ( DuneNDEcalRecoLib ${ROOT_LIBRARIES} )

add_executable(RecoTest ${CMAKE_SOURCE_DIR}/test/TestReco.cxx) 
target_link_libraries (RecoTest DuneNDEcalRecoLib ${ROOT_LIBRARIES} )
