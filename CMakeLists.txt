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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ${WARNING_FLAG}")

if (UNIX)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif()

set(SRC_LIST
    src/utils.cpp
    src/permutation.cpp
    src/bound.cpp
    src/qap.cpp
    src/qap_executor.cpp
    src/genetic/generation.cpp
    src/genetic/mutation.cpp
    src/genetic/crossover.cpp
    src/genetic/selection.cpp
    src/genetic/gen_alg.cpp
    src/main.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})

if (TBB_DIR)
    find_package(TBB)
    target_link_libraries(${PROJECT_NAME} TBB::tbb)
    if (WIN32)
        target_link_libraries(${PROJECT_NAME} TBB::tbbmalloc)
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_TBB")
endif()

find_package(Threads REQUIRED)

target_include_directories(qap
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
