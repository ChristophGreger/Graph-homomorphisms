cmake_minimum_required(VERSION 3.28)
project(Graph_homomorphisms)

set(CMAKE_CXX_STANDARD 14)

add_library(bib STATIC
        src/main/Standard_Graph.cpp
        src/main/Coloured_Graph.cpp
        include/Coloured_Graph.h
        include/Standard_Graph.h
        include/Graph.h
)

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

# Tests registrieren (damit ctest sie findet)
enable_testing()

#Add tests
add_gtest(Graph_Standard_tests tests/Graph_Standard_tests.cpp)