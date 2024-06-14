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
        value expected_result {expected};
        EXPECT_TRUE (expected_result.valid ());
        EXPECT_NO_THROW (result = run (program));
        EXPECT_EQ (expected_result, result) << "expected program " << program << " to evaluate to " << expected << " but got " << result;
    }
    
    TEST (ProgramTest, TestValue) {
        values_test_case ("1", "1");
        values_test_case ("1234567890", "1234567890");
        values_test_case ("1", "(@ x -> x) 1");
        values_test_case ("[1, 2, 3]", "[1, 2, 3]");
        values_test_case ("3", "n := 3; n");
        values_test_case ("17", "if true then 17 else 35");
        values_test_case ("35", "if false then 17 else 35");
        values_test_case ("2", "1 + 1");
        //values_test_case ("[true, false]", "eq _x _x ? x:Value := true; _x == _y ? x:Value & y:Value := false; [eq 5 5, eq 7 3]");
        /*
        values_test_case ("[false, true]", "not true := false; not false := true; ! := not [!true, !false]");
        values_test_case ("true", "!false");
        values_test_case ("false", "and false true");
        values_test_case ("true", "or false true");
        values_test_case ("3 / 5", "6 / 10");
        values_test_case ("44", "9 + 7 * 5");
        values_test_case ("80", "(9 + 7) * 5");
        values_test_case ("true", "1 > 2");
        values_test_case ("false", "1 < 2");
        values_test_case ("13", "fib 0 = 0; fib 1 = 1; fib x:N = fib (x - 1) + fib (x - 2)");*/
    }
    /*
    void cast_test_case (const string &T, const string &expr, bool valid = true) {
        Machine m {};
        EXPECT_TRUE (cast (type {T}, expression {expr}, m) == yes);
    }
    
    TEST (ProgramTest, TestCast) {
        cast_test_case ("Null", "null");
        cast_test_case ("Value", "null");
        cast_test_case ("Bool", "true");
        cast_test_case ("Bool", "false");
        cast_test_case ("Value", "true");
        cast_test_case ("Bool", "null", false);
        cast_test_case ("Null", "true", false);
        cast_test_case ("Bool", "unknown", false);
        cast_test_case ("Intuit", "unknown");
        cast_test_case ("N", "0");
        cast_test_case ("N", "1");
    }*/
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

