cmake_minimum_required(VERSION 3.28)
project(Graph_homomorphisms)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(ASAN_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ASAN_FLAGS}")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} ${ASAN_FLAGS}")
    endif()
endif()

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
        src/main/NextInjection.cpp
        include/NextInjection.h
        include/Nauty_wrapper.h
        src/main/CalcHoms.cpp
        include/CalcHoms.h
        src/main/Spasm_k_Matching.cpp
        include/Spasm_k_Matching.h
        src/main/Injective_Hom_Count_By_Spasm.cpp
        include/Injective_Hom_Count_By_Spasm.h
        include/linear_Equations_F2_small.h
        src/main/GraphTemplate.cpp
        include/GraphTemplate.h
        src/main/Spasm.cpp
        include/Spasm.h
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

##BOOST
cmake_policy(SET CMP0167 NEW)
# Boost suchen (ohne COMPONENTS)
find_package(Boost REQUIRED)

# Boost Include-Pfad hinzufügen
target_include_directories(bib PUBLIC ${Boost_INCLUDE_DIRS})

# Boost-Thread und System manuell verlinken
target_link_libraries(bib PRIVATE ${Boost_LIBRARIES})
##END BOOST

#Tests

# FetchContent-Modul laden
include(FetchContent)

# Google Test herunterladen und konfigurieren
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.12.1
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
    # Add ASan flags to the test target
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${test_name} PRIVATE "-fsanitize=address" "-fno-omit-frame-pointer")
        target_link_options(${test_name} PRIVATE "-fsanitize=address")
    endif()
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

# register tests
enable_testing()

#Add tests
add_gtest(Graph_tests tests/Graph_tests.cpp)
add_gtest(EvenSubsetIterator_tests tests/EvenSubsetIterator_tests.cpp)
add_gtest(CFI_tests tests/CFI_tests.cpp)
add_gtest(RandomGraphGenerator_tests tests/RandomGraphGenerator_tests.cpp)
add_gtest(SpeedComparisons_tests tests/SpeedComparisons.cpp)
add_gtest(NextInjection_tests tests/NextInjection_tests.cpp)
add_gtest(Utitlities_tests tests/Utilities_tests.cpp)
add_gtest(Closed_Formula_Trying tests/Closed_Formula_Trying.cpp)
add_gtest(Spasm_tests tests/Spasm_tests.cpp)
add_gtest(Linear_Equations_F2_small_tests tests/linear_Equations_F2_small_tests.cpp)
add_gtest(CalcHoms_tests tests/CalcHoms_tests.cpp)
add_gtest(CalcHoms_Christoph tests/CalcHoms_Christoph_tests.cpp)


#Fuer Nauty:
set(NAUTY_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/external/nauty/nauty.a)
target_link_libraries(bib PRIVATE ${NAUTY_LIBRARY})
target_include_directories(bib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/nauty)


#Some stuff doesn't work: To run everything you need to run ./configure in the external/nauty and make afterwards
#boost MUSS installiert sein!
#--> sudo apt update
#sudo apt install libboost-all-dev