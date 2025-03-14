cmake_minimum_required (VERSION 3.16)

# Back compatibility for VERSION range
if (${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy (VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif ()

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed")
endif ()

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE STRING "Modules for CMake" FORCE)

project (Diophant VERSION 0.0.1
    DESCRIPTION "a pure functional programming language that implements the Curry Howard isomorphism"
    LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package (Boost 1.86 COMPONENTS thread chrono log_setup log REQUIRED)

add_definitions ("-DHAS_BOOST")

find_package (OpenSSL REQUIRED Crypto SSL)

include (FetchContent)
FetchContent_Declare (json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable (json)

FetchContent_Declare (argh URL https://github.com/adishavit/argh/archive/refs/tags/v1.3.2.zip)
FetchContent_MakeAvailable (argh)

FetchContent_Declare (ctre URL https://github.com/hanickadot/compile-time-regular-expressions/archive/refs/tags/v3.9.0.zip)
FetchContent_MakeAvailable (ctre)

find_package (PkgConfig REQUIRED)

IF (WIN32)
  pkg_check_modules (cryptopp REQUIRED IMPORTED_TARGET cryptopp)
ELSE ()
  pkg_check_modules (cryptopp REQUIRED IMPORTED_TARGET libcrypto++)
ENDIF ()

find_package (data CONFIG REQUIRED)

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
    source/Diophant/statement.cpp
    source/Diophant/expressions/values.cpp
    source/Diophant/expressions/symbol.cpp
    source/Diophant/expressions/lambda.cpp
    source/Diophant/expressions/pattern.cpp
    
)

target_link_libraries (
    Diophant
    util
    OpenSSL::SSL
    OpenSSL::Crypto
    Boost::boost
    Boost::chrono
    Boost::thread
    Boost::log_setup
    Boost::log
    PkgConfig::cryptopp
    nlohmann_json::nlohmann_json
    secp256k1
    taocpp::pegtl
    Data::data
)

target_include_directories (Diophant PUBLIC include)

# Set C++ version
target_compile_features (Diophant PUBLIC cxx_std_20)
set_target_properties (Diophant PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options (Diophant PUBLIC "-fconcepts")

## Enable testing
if (PACKAGE_TESTS)
  include (CTest)
  FetchContent_Declare (
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable (googletest)
  enable_testing ()
  add_subdirectory (test)

endif ()

add_executable (Lambda diophant.cpp)

target_link_libraries (Lambda PUBLIC Diophant)
target_include_directories (Lambda PUBLIC include)

target_compile_features (Lambda PUBLIC cxx_std_20)
set_target_properties (Lambda PROPERTIES CXX_EXTENSIONS OFF)
target_compile_options (Lambda PUBLIC "-fconcepts")
