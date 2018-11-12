# include directories
include_directories( BEFORE include ${ROOT_INCLUDE_DIRS} )

aux_source_directory( src SRC_FILES )

add_library( ${PROJECT_NAME} SHARED ${SRC_FILES} )
target_link_libraries(${PROJECT_NAME} PUBLIC EDepSim::edepsim_io ${ROOT_LIBRARIES})

install( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib )

# -------------------------------------------------
# build all binaries in main directory
add_executable(duneana main/duneana.cpp)
target_link_libraries(duneana ${PROJECT_NAME})

add_executable(duneplot main/duneplot.cpp)
target_link_libraries(duneplot ${PROJECT_NAME})

add_executable(edepana main/edepana.cpp)
target_link_libraries(edepana ${PROJECT_NAME})

install( TARGETS duneana duneplot edepana RUNTIME DESTINATION bin )
