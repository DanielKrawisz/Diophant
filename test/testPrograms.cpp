// Copyright (c) 2019 Katrina Swales
// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <Diophant.hpp>
#include "gtest/gtest.h"

namespace Diophant {
    
    void values_test_case (const string &expected, const string &program) {
        value result;
        EXPECT_NO_THROW (result = run (program));
        value expected_result {expected};
        EXPECT_TRUE (expected_result.valid ());
        EXPECT_EQ (expected_result, result) << "expected program " << program << " to evaluate to " << expected << " but got " << result;
    }
    
    TEST (ProgramTest, TestValue) {
        values_test_case ("1", "1");
        values_test_case ("1", "(@ x -> x) 1");
        values_test_case ("[1, 2, 3]", "[1, 2, 3]");
        values_test_case ("3", "n -> 3; n");
        /*
        test_case ("false", "!true");
        test_case ("true", "!false");
        test_case ("false", "and false true");
        test_case ("true", "or false true");
        test_case ("2", "1 + 1");
        test_case ("3 / 5", "6 / 10");
        test_case ("44", "9 + 7 * 5");
        test_case ("80", "(9 + 7) * 5");
        test_case ("true", "1 > 2");
        test_case ("false", "1 < 2");*/
        //test_case ("13", "fib 0 = 0; fib 1 = 1; fib x:N = fib (x - 1) + fib (x - 2)")
    }
    /*
    void program_test_case (const string &expected, const string &program) {
        std::cout << "testing program " << program << std::endl;
        expression result;
        Machine m;
        EXPECT_NO_THROW (result = evaluate (expression {program}, m));
        EXPECT_EQ (expression {expected}, result) << "expected program " << program << " to evaluate to " << expected << " but got " << result;
    }
    
    TEST (ProgramTest, TestEvaluate) {
        program_test_case ("(@ x -> x)", "(@ x -> x x) (@ x -> x)");
    }*/

}

#pragma clang diagnostic pop

