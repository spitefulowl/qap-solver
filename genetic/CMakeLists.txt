cmake_minimum_required(VERSION 3.10)
project (GeneticLib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES src/genetic/GenAlg.cpp
            src/genetic/Generation.cpp
            src/genetic/Mutation.cpp
            src/genetic/Selection.cpp
            src/genetic/Crossover.cpp
            src/genetic/utils.cpp)

add_library(GeneticLib SHARED ${SOURCES})
set_target_properties(GeneticLib PROPERTIES LINKER_LANGUAGE C)
