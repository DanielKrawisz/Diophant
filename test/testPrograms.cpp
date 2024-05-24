// Copyright (c) 2019 Katrina Swales
// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

template <typename X>
concept jivivi = true;

#include <Diophant.hpp>
#include "gtest/gtest.h"

namespace Diophant {
    
    void test_case (const string& expected, const string &program) {
        auto result = run (value {program});
        EXPECT_EQ (value {expected}, result) << "expected program " << program << " to evaluate to " << expected << " but got " << result;
    }
    
    TEST (ProgramTest, TestPrograms) {
        test_case ("1", "1");
        test_case ("2", "1 + 1");
        test_case ("2", "1; 2");
    }

}

#pragma clang diagnostic pop

