#include <iostream>
#include <sstream>
#include "CppUTest/TestHarness.h"


TEST_GROUP(FIRST_GROUP) {
    void setup() override {
        std::cout << "START OF MEM LEAK TERROR" << std::endl;
    }
   void teardown() override {
        std::cout << "ME NO LIKE MEM LEAKS, PLEASE DESTROY" << std::endl;
    }
};

TEST(FIRST_GROUP, SAMPLE) {
    const std::string consoleInput = "PINHO TEST";
    const std::string expected = "PINHO TEST";

    std::istringstream input{consoleInput};
    std::ostringstream output;

    output << input.str();

    CHECK(output.str() == expected);
}

TEST(FIRST_GROUP, FAIL_TEST) {
    FAIL("Fail me!");
}