cmake_minimum_required(VERSION 3.1...3.14)

# Back compatibility for VERSION range
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Diophant VERSION 1.0
		DESCRIPTION "a pure functional programming language that implements the Curry Howard isomorphism"
		LANGUAGES CXX)

# Set cmake as import path for Find*.cmake files
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

## Check if GTests is installed. If not, install it

# option(PACKAGE_TESTS "Build the tests" ON)

## Enable testing


# if(PACKAGE_TESTS)
#    include(CTest)
#    find_package(GTest REQUIRED)
#    enable_testing()
#    add_subdirectory(test)
# endif()

add_library(Diophant STATIC
    
    source/Diophant/replace.cpp
    source/Diophant/expression.cpp
    source/Diophant/expressions/builder.cpp
    source/Diophant/lambda.cpp
    source/Diophant/evaluate.cpp
    
)

target_link_libraries(Diophant PUBLIC CONAN_PKG::data util CONAN_PKG::boost CONAN_PKG::openssl CONAN_PKG::cryptopp CONAN_PKG::nlohmann_json CONAN_PKG::gmp )

target_include_directories(Diophant PUBLIC include)

# Set C++ version
target_compile_features(Diophant PUBLIC cxx_std_20)
set_target_properties(Diophant PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options(Diophant PUBLIC "-fconcepts")

add_executable(Lambda lambda.cpp )

target_link_libraries(Lambda PUBLIC Diophant)
target_include_directories(Lambda PUBLIC include)

target_compile_features(Lambda PUBLIC cxx_std_20)
set_target_properties(Lambda PROPERTIES CXX_EXTENSIONS OFF)
