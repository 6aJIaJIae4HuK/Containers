cmake_minimum_required(VERSION 2.8)

project(Containers)
enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(containers INTERFACE)

target_include_directories(containers INTERFACE ${PROJECT_SOURCE_DIR}/include)

set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

file(GLOB SRC_DIRECTORY "${PROJECT_SOURCE_DIR}/tests/*.cpp")
add_executable(test_executable ${SRC_DIRECTORY})
target_include_directories(test_executable PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(test_executable ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

if (WIN32)
	message(${CMAKE_BINARY_DIR})
	add_definitions(-DBOOST_ALL_NO_LIB)
	get_filename_component(PATH_NAME ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG} PATH)
	get_filename_component(DEBUG_NAME ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG} NAME_WE)
	get_filename_component(RELEASE_NAME ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_RELEASE} NAME_WE)
	install(FILES ${PATH_NAME}/${DEBUG_NAME}.dll
		DESTINATION ${CMAKE_BINARY_DIR}/Debug
		CONFIGURATIONS Debug)
	install(FILES ${PATH_NAME}/${RELEASE_NAME}.dll
		DESTINATION ${CMAKE_BINARY_DIR}/Release
		CONFIGURATIONS Release)
endif()

add_test(NAME test1 COMMAND test_executable)
