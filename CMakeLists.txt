cmake_minimum_required(VERSION 2.8)

project(SaneEngine)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCEFILES
	src/**/*.cpp
)

file(GLOB HEADERFILES
	include/se/**/*.h
)

set(BuildDir ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BuildDir}/${CMAKE_BUILD_TYPE})

add_library(${PROJECT_NAME} SHARED ${SOURCEFILES} ${HEADERFILES} )

#Test Code
add_executable(TestProg main.cpp)
target_link_libraries(TestProg ${PROJECT_NAME})
