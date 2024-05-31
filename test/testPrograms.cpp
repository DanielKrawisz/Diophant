// Copyright (c) 2019 Katrina Swales
// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <Diophant.hpp>
#include "gtest/gtest.h"

namespace Diophant {
    
    void test_case (const string& expected, const string &program) {
        value result;
        EXPECT_NO_THROW (result = run (program));
        EXPECT_EQ (value {expected}, result) << "expected program " << program << " to evaluate to " << expected << " but got " << result;
    }
    
    TEST (ProgramTest, TestPrograms) {
        test_case ("1", "1");
        test_case ("1", "(@ x -> x) 1");
        test_case ("[1, 2, 3]", "[1, 2, 3]");
        test_case ("2", "1; 2");
        test_case ("false", "!true");
        test_case ("true", "!false");
        test_case ("false", "and false true");
        test_case ("true", "or false true");
        test_case ("3", "n -> 3; n");
        /*
        test_case ("2", "1 + 1");
        test_case ("3 / 5", "6 / 10");
        test_case ("44", "9 + 7 * 5");
        test_case ("80", "(9 + 7) * 5");
        test_case ("true", "1 > 2");
        test_case ("false", "1 < 2");*/
        //test_case ("13", "fib 0 = 0; fib 1 = 1; fib x:N = fib (x - 1) + fib (x - 2)")
    }

}

#pragma clang diagnostic pop

