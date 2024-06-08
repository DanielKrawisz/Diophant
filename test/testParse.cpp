// Copyright (c) 2019 Katrina Swales
// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <Diophant.hpp>
#include "gtest/gtest.h"

namespace Diophant {
    
    void test_case (const string &program, bool valid = true) {
        Parser *eval {nullptr};
        EXPECT_NO_THROW (eval = new Parser {[] (Expression &x) {}});
        
        if (valid) {
            EXPECT_NO_THROW (eval->read_line (program));
        } else {
            EXPECT_ANY_THROW (eval->read_line (program));
        }
        
        delete eval;
    }
    
    TEST (ParseTest, TestParse) {
        test_case ("1");
        test_case ("/< This is a comment >/1");
        test_case ("1/< This is a comment >/");
        test_case ("1; 2", false);
        test_case ("n := 3; n");
        test_case ("n -> 3; n", false);
        test_case ("a b c");
        test_case (
            "/< this is a list of numbers >/"
            "[1, 2, 3]");
        test_case (R"("a string")");
        test_case ("f _x /; x:N := x + x; f 5", false);
        test_case ("f _x ? x:N := x + x; f 5");
        test_case (R"(f _x _y := (y x) y; f "a" "b")");
        test_case (
            "/< this is a lambda >/"
            "(@ x -> x x) z");
        test_case (
            "/< SKI calculus >/"
            "I _x := x; K _x _y := x; S _x _y _z := x z (y z); S K I");
        test_case ("eq _x _x ? x:Value := true; _x == _y ? x:Value & y:Value := false; eq 5 5");
        test_case ("let x -> 3 in x + x");
        test_case ("let x -> 3, y -> 7 in x ^ y");
        test_case ("if 2 < 0 then 222 else 6666");
    }

}

#pragma clang diagnostic pop


