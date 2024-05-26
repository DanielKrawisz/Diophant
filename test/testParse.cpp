// Copyright (c) 2019 Katrina Swales
// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <Diophant.hpp>
#include "gtest/gtest.h"

namespace Diophant {
    
    void test_case (const string &program) {
        
        Parser eval {[] (Expression &x) {}};
        
        EXPECT_NO_THROW (eval.read_line (program));
    }
    
    TEST (ParseTest, TestParse) {
        test_case ("1");
        test_case ("1; 2");
        test_case ("n = 3; n");
        test_case ("{1, 2, 3}");
        test_case ("a b c");
        test_case (R"("a string")");
        test_case ("f .x /; x:N = x + x; f 5");
        test_case (R"(f .x .y = (y x) y; f "a" "b")");
    }

}

#pragma clang diagnostic pop


