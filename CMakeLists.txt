cmake_minimum_required(VERSION 2.6)
 
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
 
project(inside-game)
 
find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(PNG REQUIRED)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -fPIC" )

set(CMAKE_BUILD_TYPE Debug CACHE STRING "default to debug" FORCE)

SET(LIBRARIES ${GLFW_LIBRARY} ${OPENGL_LIBRARY} ${FREEIMAGE_LIBRARY} ${PNG_LIBRARY})



aux_source_directory("${PROJECT_SOURCE_DIR}/src/." SRC_LIST)
file(GLOB INCLUDE_LIST "${PROJECT_SOURCE_DIR}/include/*.h")
file(GLOB RESOURCES "${PROJECT_SOURCE_DIR}/resources/shaders/*")


include_directories("${PROJECT_SOURCE_DIR}/include/")

link_directories (${OPENGLEXAMPLES_BINARY_DIR}/bin)

 
add_executable(${PROJECT_NAME} ${SRC_LIST} ${INCLUDE_LIST} ${RESOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
