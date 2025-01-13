cmake_minimum_required(VERSION 3.28)
project(Graph_homomorphisms)

set(CMAKE_CXX_STANDARD 14)

add_library(bib STATIC
        include/Graph.h
        src/main/Graph.cpp
        src/main/Node.cpp
        include/Node.h
        include/utilities.h
        src/main/utilities.cpp
        src/main/CFIGraph.cpp
        include/CFIGraph.h
        src/main/CFINode.cpp
        include/CFINode.h
        src/main/EvenSubsetIterator.cpp
        include/EvenSubsetIterator.h
        src/main/RandomGraphGenerator.cpp
        include/RandomGraphGenerator.h
        src/main/InjectiveHomomorphismRange.cpp
        include/InjectiveHomomorphismRange.h
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

target_include_directories(bib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(Graph_homomorphisms
        src/Main.cpp
)

target_link_libraries(Graph_homomorphisms
        PRIVATE
        bib
)

#Tests

# FetchContent-Modul laden
include(FetchContent)

# Google Test herunterladen und konfigurieren
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.12.1  # oder eine neuere Version
)

FetchContent_MakeAvailable(googletest)

#Function to add a gtest
function(add_gtest test_name)
    add_executable(${test_name} ${ARGN})
    target_link_libraries(${test_name}
            GTest::gtest
            GTest::gtest_main
            bib
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

# register tests
enable_testing()

#Add tests
add_gtest(Graph_tests tests/Graph_tests.cpp)
add_gtest(EvenSubsetIterator_tests tests/EvenSubsetIterator_tests.cpp)
add_gtest(CFI_tests tests/CFI_tests.cpp)
add_gtest(RandomGraphGenerator_tests tests/RandomGraphGenerator_tests.cpp)
add_gtest(InjectiveHomomorphismRange_tests tests/InjectiveHomomorphismRange_tests.cpp)
add_gtest(SpeedComparisons tests/SpeedComparisons.cpp)