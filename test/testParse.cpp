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
        
        Parser eval {[] (Expression &x) {}};
        
        if (valid) {
            EXPECT_NO_THROW (eval.read_line (program));
        } else {
            EXPECT_ANY_THROW (eval.read_line (program));
        }
    }
    
    TEST (ParseTest, TestParse) {
        test_case ("1");
        test_case ("1; 2");
        test_case ("n := 3; n");
        test_case ("a b c");
        test_case (
            "/* this is a list of numbers */"
            "[1, 2, 3]");
        test_case ("a b c");
        test_case (R"("a string")");
        test_case ("f .x /; x:N := x + x; f 5");
        test_case (R"(f .x .y = (y x) y; f "a" "b")");
        test_case (
            "/*this is a lambda*/"
            "(@ x -> x x) z");
        test_case (
            "Null := null;"
            "Bool := false | true;"
            "Sign := negative | zero | positive;"
            "Value := Null | Bool | Sign | N | Z | Q | String | List .x | map .x .y;");
        test_case (
            "null : Null := null;"
            "true : Bool := null;"
            "false : Bool := null;"
            "0 : N := null;"
            ".n : N /; --n : N := null;"
            "\"\" : String := null;"
            "nonzero n. := @ x -> x : n & n != 0");
        test_case (
            "`==` .a .a /; a:Value := true;"
            "`==` .a .b /; a:Value & b:Value := false;"
            "`!=` .a .b = ! (a == b);"
            "`>=` .a .b = ! (a < b);"
            "`<=` .a .b = ! (a > b);"
            "`==` /; `==` : Value => Value => Bool; "
            "`!=` /; `!=` : Value => Value => Bool; "
            "`&&` /; `&&` : Bool => Bool => Bool; "
            "`||` /; `||` : Bool => Bool => Bool; "
            "`>=` /; `>=` : String => String => Bool; "
            "`<=` /; `<=` : String => String => Bool; "
            "`>` /; `>` : String => String => Bool; "
            "`<` /; `<` : String => String => Bool; "
            "`>=` /; `>=` : N => N => Bool; "
            "`<=` /; `<=` : N => N => Bool; "
            "`>` /; `>` : N => N => Bool; "
            "`<` /; `<` : N => N => Bool; "
            "`++` /; `++` : N => +N; "
            "`--` /; `--` : +N => N; "
            "`+u` /; `+u` : N => Z; "
            "`-u` /; `-u` : N => Z; "
            "`+` /; `+` : N => N => N; "
            "`*` /; `*` : N => N => N; "
            "`-` /; `-` : N => N => Null | N; "
            "`/%` /; `/%` : N => N => {quotient: N, remainder: N}; "
            "`>=` /; `>=` : Z => Z => Bool; "
            "`<=` /; `<=` : Z => Z => Bool; "
            "`>` /; `>` : Z => Z => Bool; "
            "`<` /; `<` : Z => Z => Bool; "
            "`++` /; `++` : Z => Z; "
            "`++` /; `++` : Z => Z; "
            "`+u` /; `+u` : Z => N; "
            "`+` /; `+` : Z => Z => Z; "
            "`*` /; `*` : Z => Z => Z; "
            "`-` /; `-` : Z => Z => Z; "
            "`/%` /; `/%` : Z => Z => {quotient: Z, remainder: N}; "
            "`==` /; `==` : Q => Q => Bool; "
            "`!=` /; `!=` : Q => Q => Bool; "
            "`>=` /; `>=` : Q => Q => Bool; "
            "`<=` /; `<=` : Q => Q => Bool; "
            "`>` /; `>` : Q => Q => Bool; "
            "`<` /; `<` : Q => Q => Bool; "
            "`+` /; `+` : Q => Q => Q; "
            "`*` /; `*` : Q => Q => Q; "
            "`-` /; `-` : Q => Q => Q; "
            "`/` /; `/` : Q => nonzero Q => Q; ");/*
        test_case (
            "`==` :: Bool => Bool => Bool; "
            "`!=` :: Bool => Bool => Bool; "
            "`&&` :: Bool => Bool => Bool; "
            "`||` :: Bool => Bool => Bool; "
            "`==` :: N => N => Bool; "
            "`!=` :: N => N => Bool; "
            "`>=` :: N => N => Bool; "
            "`<=` :: N => N => Bool; "
            "`>` :: N => N => Bool; "
            "`<` :: N => N => Bool; "
            "increment :: N => +N; "
            "decrement :: +N => N; "
            "`+` :: N => N => N; "
            "`*` :: N => N => N; "
            "`-` :: N => N => Null | N; "
            "`/%` :: N => N => {quotient: N, remainder: N}; "
            "`==` :: Z => Z => Bool; "
            "`!=` :: Z => Z => Bool; "
            "`>=` :: Z => Z => Bool; "
            "`<=` :: Z => Z => Bool; "
            "`>` :: Z => Z => Bool; "
            "`<` :: Z => Z => Bool; "
            "increment :: Z => Z; "
            "decrement :: Z => Z; "
            "`+` :: Z => Z => Z; "
            "`*` :: Z => Z => Z; "
            "`-` :: Z => Z => Z; "
            "`/%` :: Z => Z => {quotient: Z, remainder: N}; "
            "`==` :: Q => Q => Bool; "
            "`!=` :: Q => Q => Bool; "
            "`>=` :: Q => Q => Bool; "
            "`<=` :: Q => Q => Bool; "
            "`>` :: Q => Q => Bool; "
            "`<` :: Q => Q => Bool; "
            "`+` :: Q => Q => Q; "
            "`*` :: Q => Q => Q; "
            "`-` :: Q => Q => Q; "
            "`/` :: Q => nonzero Q => Q; ", false);*/
    }

}

#pragma clang diagnostic pop


