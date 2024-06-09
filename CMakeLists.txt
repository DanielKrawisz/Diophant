cmake_minimum_required (VERSION 3.1...3.14)

# Back compatibility for VERSION range
if (${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy (VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif ()

project (Diophant VERSION 0.0.1
    DESCRIPTION "a pure functional programming language that implements the Curry Howard isomorphism"
    LANGUAGES CXX)

find_package (data CONFIG REQUIRED)
find_package (Boost CONFIG REQUIRED)
find_package (OpenSSL CONFIG REQUIRED)
find_package (cryptopp CONFIG REQUIRED)
find_package (nlohmann_json CONFIG REQUIRED)
find_package (gmp CONFIG REQUIRED)
find_package (secp256k1 CONFIG REQUIRED)
find_package (pegtl CONFIG REQUIRED)

## Check if GTests is installed. If not, install it

option (PACKAGE_TESTS "Build the tests" ON)

add_library (Diophant STATIC
    
    source/Diophant/parser.cpp
    source/Diophant/machine.cpp
    source/Diophant/expression.cpp
    source/Diophant/type.cpp
    source/Diophant/evaluate.cpp
    source/Diophant/replace.cpp
    source/Diophant/match.cpp
    source/Diophant/pattern.cpp
    source/Diophant/value.cpp
    source/Diophant/type.cpp
    source/Diophant/symbol.cpp
    source/Diophant/expressions/values.cpp
    source/Diophant/expressions/symbol.cpp
    source/Diophant/expressions/lambda.cpp
    source/Diophant/expressions/pattern.cpp
    
)

target_link_libraries (
    Diophant
    util
    openssl::openssl
    boost::boost
    cryptopp::cryptopp
    nlohmann_json::nlohmann_json
    gmp::gmp
    secp256k1::secp256k1
    taocpp::pegtl
    data::data
)

target_include_directories (Diophant PUBLIC include)

# Set C++ version
target_compile_features (Diophant PUBLIC cxx_std_20)
set_target_properties (Diophant PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options (Diophant PUBLIC "-fconcepts")

## Enable testing
if (PACKAGE_TESTS)
   include (CTest)
   find_package (GTest REQUIRED)
   enable_testing ()
   add_subdirectory (test)
endif ()

add_executable (Lambda diophant.cpp)

target_link_libraries (Lambda PUBLIC Diophant)
target_include_directories (Lambda PUBLIC include)

target_compile_features (Lambda PUBLIC cxx_std_20)
set_target_properties (Lambda PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options (Lambda PUBLIC "-fconcepts")
