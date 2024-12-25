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

add_executable(tests
        tests/tests.cpp
)

target_link_libraries(Graph_homomorphisms
        PRIVATE
        bib
)

target_link_libraries(tests
        PRIVATE
        bib
)
