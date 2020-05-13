cmake_minimum_required(VERSION 3.10)
project(qap)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
if(MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SRC_LIST 
    src/bound.cpp
    src/helpers.cpp
    src/permutation.cpp
    src/QAP.cpp
    src/qap_executor.cpp
    src/QuadraticAssignmentProblem.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})
find_package(Threads REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/genetic/include)
add_subdirectory(genetic)

target_link_libraries(qap Threads::Threads
	GeneticLib)
