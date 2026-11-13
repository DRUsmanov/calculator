#include <gtest/gtest.h>
#include "calculator/calculator.h"
#include <sstream>

class CaclculatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        buffer.str() = "";
        buffer.clear();
        old_buf = std::cout.rdbuf(buffer.rdbuf());
    }

    void TearDown() override {
        buffer.clear();
        std::cout.rdbuf(old_buf);
    }

    std::stringstream buffer;
    std::streambuf* old_buf;
};

TEST_F(CaclculatorTest, Help){
    int argc = 2;
    char arg0[]{"calculator"};
    char arg1[]{"--help"};
    char* argv[] = {arg0, arg1};
    calculator::Application application;
    application.run(argc, argv);

    std::string expected{R"(Use: calculator '{"value_1":<value_1>, "op":"<operator>", "value_2":<value_2>}'
        Available operators: +, -, *, /, ^, !
        Example: calculator '{"value_1":10,"op":"+","value_2":50}')"};

    EXPECT_EQ(buffer.str(), expected.append("\n"));
}

TEST_F(CaclculatorTest, Addition){
    int argc = 2;
    char arg0[]{"calculator"};
    char arg1[]{R"(
        {
        "value_1":10,
        "op":"+",
        "value_2":50}
        )"};
    char* argv[] = {arg0, arg1};
    calculator::Application application;
    application.run(argc, argv);

    int result = 0;
    buffer >> result;

    EXPECT_EQ(result, 60);
}

TEST_F(CaclculatorTest, Subtraction){
    int argc = 2;
    char arg0[]{"calculator"};
    char arg1[]{R"(
        {
        "value_1":10,
        "op":"-",
        "value_2":50}
        )"};
    char* argv[] = {arg0, arg1};
    calculator::Application application;
    application.run(argc, argv);

    int result = 0;
    buffer >> result;

    EXPECT_EQ(result, -40);
}

TEST_F(CaclculatorTest, Multiplication){
    int argc = 2;
    char arg0[]{"calculator"};
    char arg1[]{R"(
        {
        "value_1":10,
        "op":"*",
        "value_2":50}
        )"};
    char* argv[] = {arg0, arg1};
    calculator::Application application;
    application.run(argc, argv);

    int result = 0;
    buffer >> result;

    EXPECT_EQ(result, 500);
}

TEST_F(CaclculatorTest, Multiplication_overflow){
    int argc = 2;
    char arg0[]{"calculator"};
    char arg1[]{R"(
        {
        "value_1":100000,
        "op":"*",
        "value_2":500000}
        )"};
    char* argv[] = {arg0, arg1};
    calculator::Application application;
    application.run(argc, argv);

    EXPECT_EQ(buffer.str(), "Caclculation error\n");
}

TEST_F(CaclculatorTest, Multiplication_negative_overflow){
    int argc = 2;
    char arg0[]{"calculator"};
    char arg1[]{R"(
        {
        "value_1":100000,
        "op":"*",
        "value_2":-500000}
        )"};
    char* argv[] = {arg0, arg1};
    calculator::Application application;
    application.run(argc, argv);

    EXPECT_EQ(buffer.str(), "Caclculation error\n");
}

TEST_F(CaclculatorTest, Division){
    int argc = 2;
    char arg0[]{"calculator"};
    char arg1[]{R"(
        {
        "value_1":100,
        "op":"/",
        "value_2":-50}
        )"};
    char* argv[] = {arg0, arg1};
    calculator::Application application;
    application.run(argc, argv);

    int result = 0;
    buffer >> result;

    EXPECT_EQ(result, -2);
}

TEST_F(CaclculatorTest, Division_by_zero){
    int argc = 2;
    char arg0[]{"calculator"};
    char arg1[]{R"(
        {
        "value_1":100,
        "op":"/",
        "value_2":0}
        )"};
    char* argv[] = {arg0, arg1};
    calculator::Application application;
    application.run(argc, argv);

    EXPECT_EQ(buffer.str(), "Caclculation error\n");
}

TEST_F(CaclculatorTest, Exponentiation){
    int argc = 2;
    char arg0[]{"calculator"};
    char arg1[]{R"(
        {
        "value_1":10,
        "op":"^",
        "value_2":3}
        )"};
    char* argv[] = {arg0, arg1};
    calculator::Application application;
    application.run(argc, argv);

    int result = 0;
    buffer >> result;

    EXPECT_EQ(result, 1000);
}

TEST_F(CaclculatorTest, Exponentiation_negative){
    int argc = 2;
    char arg0[]{"calculator"};
    char arg1[]{R"(
        {
        "value_1":100,
        "op":"^",
        "value_2":-20}
        )"};
    char* argv[] = {arg0, arg1};
    calculator::Application application;
    application.run(argc, argv);

    EXPECT_EQ(buffer.str(), "Caclculation error\n");
}


TEST_F(CaclculatorTest, Factorial){
    int argc = 2;
    char arg0[]{"calculator"};
    char arg1[]{R"(
        {
        "value_1":3,
        "op":"!",
        "value_2":3}
        )"};
    char* argv[] = {arg0, arg1};
    calculator::Application application;
    application.run(argc, argv);

    int result = 0;
    buffer >> result;

    EXPECT_EQ(result, 6);
}

TEST_F(CaclculatorTest, Factorial_negative){
    int argc = 2;
    char arg0[]{"calculator"};
    char arg1[]{R"(
        {
        "value_1":-100,
        "op":"!",
        "value_2":-20}
        )"};
    char* argv[] = {arg0, arg1};
    calculator::Application application;
    application.run(argc, argv);

    EXPECT_EQ(buffer.str(), "Caclculation error\n");
}