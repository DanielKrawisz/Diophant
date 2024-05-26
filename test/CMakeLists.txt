
cmake_minimum_required (VERSION 3.1...3.14)

# Back compatibility for VERSION range
if (${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy (VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif ()

include (GoogleTest)
macro (package_add_test TESTNAME)
    # create an exectuable in which the tests will be stored
    add_executable (${TESTNAME} ${ARGN})
    # link the Google test infrastructure, mocking library, and a default main fuction to
    # the test executable.  Remove g_test_main if writing your own main function.
    
    target_include_directories (${TESTNAME} PUBLIC . ${CMAKE_SOURCE_DIR}/include
        boost::boost
        openssl::openssl
        cryptopp::cryptopp
        nlohmann_json::nlohmann_json
        gmp::gmp)
    
    target_link_libraries (${TESTNAME} gtest::gtest data::data Diophant)
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    
    gtest_discover_tests (${TESTNAME}
        # set a working directory so your project root so that you can find test data via paths relative to the project root
        SOURCES ${ARGN}
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    
    target_compile_features (${TESTNAME} PUBLIC cxx_std_20)
    set_target_properties (${TESTNAME} PROPERTIES CXX_EXTENSIONS OFF)
    target_compile_options (${TESTNAME} PUBLIC "-fconcepts")

    set_target_properties (${TESTNAME} PROPERTIES FOLDER tests)
endmacro ()

package_add_test (testParse testParse.cpp)
package_add_test (testPrograms testPrograms.cpp)
