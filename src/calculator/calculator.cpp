#include "calculator/calculator.h"

#include "logger/logger.h"

#include <getopt.h>
#include <math_functions.h>

#include <nlohmann/json.hpp>

#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

namespace calculator
{

using nlohmann::json;

void Application::run(int argc, char* argv[])
{
    try
    {
        if (processFlags(argc, argv))
        {
            return;
        }
        makeTask(argc, argv);
        makeCalculate();
        printResult();
    }
    catch (const std::exception& exp)
    {
        logger::Logger::instance().logError(exp.what());
        std::cout << exp.what() << std::endl;
    }
}

void Application::makeTask(int argc, char* argv[])
{
    logger::Logger::instance().logInfo("Start making task");

    if (argc < 2)
    {
        logger::Logger::instance().logError("No function for making task");
        return;
    }

    std::string function{argv[1]};

    parseFunction(function);

    task_.status = true;

    logger::Logger::instance().logInfo("Success making task");
}

void Application::makeCalculate()
{
    logger::Logger::instance().logInfo("Start task calculating");

    if (!task_.status)
    {
        logger::Logger::instance().logInfo("Task is not ready for calculation");
        return;
    }
    task_.status = false;
    math::Result result;
    switch (task_.op)
    {
        case '+':
        {
            result = math::add(task_.value_1, task_.value_2);
            break;
        }
        case '-':
        {
            result = math::sub(task_.value_1, task_.value_2);
            break;
        }
        case '*':
        {
            result = math::mul(task_.value_1, task_.value_2);
            break;
        }
        case '/':
        {
            result = math::div(task_.value_1, task_.value_2);
            break;
        }
        case '^':
        {
            result = math::pow(task_.value_1, task_.value_2);
            break;
        }
        case '!':
        {
            result = math::factorial(task_.value_1);
            break;
        }
        default:
        {
            logger::Logger::instance().logError(
                std::format("Unknown operator {}", task_.op));
            return;
        }
    }
    if (result.error)
    {
        logger::Logger::instance().logError("Caclculation error");
        throw std::runtime_error("Caclculation error");
    }
    task_.result = result.result;
    task_.status = true;
    logger::Logger::instance().logInfo("Succes calculation");
}

void Application::printResult() const
{
    logger::Logger::instance().logInfo("Start printing task result");

    if (task_.status)
    {
        std::cout << task_.result << std::endl;
        return;
    }

    logger::Logger::instance().logInfo("Task is not ready for printing");
}

void Application::parseFunction(std::string_view function)
{
    logger::Logger::instance().logInfo("Start function parsing");

    json data = json::parse(function);
    auto value_1 = data["value_1"].get<int>();
    auto value_2 = data["value_2"].get<int>();
    auto op = data["op"].get<std::string>();

    task_.value_1 = value_1;
    task_.value_2 = value_2;
    task_.op = op[0];

    logger::Logger::instance().logInfo("Succes function parsing");
}

bool Application::processFlags(int argc, char* argv[]) const
{
    logger::Logger::instance().logInfo("Start prcessing flags");

    static constexpr option long_options[]{{"help", no_argument, 0, 'h'},
                                           {0, 0, 0, 0}};

    int opt;

    while ((opt = getopt_long(argc, argv, ":h", long_options, NULL)) != -1)
    {
        switch (opt)
        {
            case ':':
            {
                logger::Logger::instance().logError(
                    std::format("No arguments for {}", optopt));
                return true;
            }
            case '?':
            {
                logger::Logger::instance().logError(
                    std::format("Unknown option {}", optopt));
                return true;
            }
            case 'h':
            {
                std::cout << help_ << std::endl;
                return true;
            }
        }
    }
    logger::Logger::instance().logInfo("No flags");
    return false;
}

} // namespace calculator
