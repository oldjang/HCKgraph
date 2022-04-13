cmake_minimum_required(VERSION 3.21)
project(HCK)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.78.0 REQUIRED COMPONENTS filesystem system)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(HCK main.cpp KMeans.cpp KMeans.h Utils.h Data.h Utils.cpp KGraph.cpp KGraph.h BruteForce.cpp BruteForce.h test.h test.cpp HCK.cpp HCK.h)

target_link_libraries(HCK ${BOOST_LIBRARIES})