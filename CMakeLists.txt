cmake_minimum_required(VERSION 2.6)
 
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
 
project(inside-game)
 
find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(PNG REQUIRED)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -fPIC" )

set(CMAKE_BUILD_TYPE Debug CACHE STRING "default to debug" FORCE)

SET(LIBRARIES ${GLFW_LIBRARY} ${OPENGL_LIBRARY} ${PNG_LIBRARY} RocketCore RocketControls RocketDebugger)



aux_source_directory("${PROJECT_SOURCE_DIR}/src/." SRC_LIST)
file(GLOB INCLUDE_LIST "${PROJECT_SOURCE_DIR}/include/*.h")

file(GLOB RESOURCES "${PROJECT_SOURCE_DIR}/resources/shaders/*"
                    "${PROJECT_SOURCE_DIR}/resources/layouts/*")

include_directories("${PROJECT_SOURCE_DIR}/include/")

include_directories("${PROJECT_SOURCE_DIR}/libRocket/Include")
include_directories("${PROJECT_SOURCE_DIR}/libRocket/Samples/shell/include")

# Build libRocket 
add_subdirectory("${PROJECT_SOURCE_DIR}/libRocket/Build")

link_directories (${OPENGLEXAMPLES_BINARY_DIR}/bin)

 
add_executable(${PROJECT_NAME} ${SRC_LIST} ${INCLUDE_LIST} ${RESOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
