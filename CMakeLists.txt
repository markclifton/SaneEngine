cmake_minimum_required(VERSION 2.8)

project(SaneEngine)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCEFILES
	src/**/*.cpp
	src/*.cpp
)

file(GLOB HEADERFILES
	include/se/**/*.h
	include/se/*.h
	include/KHR/*.h
	include/gl.h
)

set(BuildDir ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BuildDir}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BuildDir}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BuildDir}/${CMAKE_BUILD_TYPE})

add_library(${PROJECT_NAME} SHARED ${SOURCEFILES} ${HEADERFILES} )

if(WIN32)
	include_directories(${CMAKE_SOURCE_DIR}/dep/Windows/glfw3.2.1/include)
	target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/dep/Windows/glfw3.2.1/lib/glfw3.lib)

	include_directories(${CMAKE_SOURCE_DIR}/dep/Windows/glm/include)
	target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/dep/Windows/glm/lib/glm_static.lib)

elseif(APPLE)

else()

endif()

#Test Code
add_executable(TestProg main.cpp)
target_link_libraries(TestProg ${PROJECT_NAME})
