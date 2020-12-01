cmake_minimum_required(VERSION 3.10)
project(qap)

if (MSVC)
    set(WARNING_FLAG /W4)
else()
    set(WARNING_FLAG "-Wall -Wextra")
endif()

if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /GL")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${WARNING_FLAG}")

set(SRC_LIST
    src/bound.cpp
    src/helpers.cpp
    src/permutation.cpp
    src/QAP.cpp
    src/qap_executor.cpp
    src/QuadraticAssignmentProblem.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})
find_package(Threads REQUIRED)

target_include_directories(qap
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

add_subdirectory(genetic)

target_link_libraries(qap Threads::Threads
	GeneticLib)
