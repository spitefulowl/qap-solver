cmake_minimum_required(VERSION 3.10)
project (GeneticLib)

if (MSVC)
	set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

if (MSVC)
    add_compile_options(/EHsc)
endif ()

set(SOURCES src/genetic/GenAlg.cpp
            src/genetic/Generation.cpp
            src/genetic/Mutation.cpp
            src/genetic/Selection.cpp
            src/genetic/Crossover.cpp
            src/genetic/utils.cpp)

add_library(GeneticLib SHARED ${SOURCES})


set_target_properties(GeneticLib PROPERTIES LINKER_LANGUAGE C)

target_include_directories(GeneticLib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
