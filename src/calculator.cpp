#include <errno.h>
#include <math_functions.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <climits>

const char* help{
    R"(
    Use -x <value> -o <operator> -y <value>\n
    Available operators: +, -, *, /, ^, !
    )"
};

struct Task
{
    int x = 0;
    int y = 0;
    char op = '\0';
    int result;
    bool status = false;
};

int stringToInt(const char* str);
void makeTask(int argc, char* argv[], Task& task);
void makeCalculate(Task& task);
void applicationRun(int argc, char* argv[]);
void printResult(const Task& task);

int stringToInt(const char* str)
{
    errno = 0;
    char* end;
    auto result = strtol(str, &end, 10);
    if (errno == ERANGE)
    {
        return 0;
    }
    if (end == str || *end != '\0')
    {
        errno = EINVAL;
        return 0;
    }
    if (result < INT_MIN || result > INT_MAX)
    {
        errno = ERANGE;
        return 0;
    }
    return (int)result;
}

void makeTask(int argc, char* argv[], Task& task)
{
    int opt;
    task.status = false;
    while ((opt = getopt(argc, argv, ":x:o:y:h")) != -1)
    {
        switch (opt)
        {
            case 'x':
            {
                int x = stringToInt(optarg);
                if (errno == 0)
                {
                    task.x = x;
                    break;
                }
                else
                {
                    printf("Overflow x\n");
                    return;
                }
            };
            case 'y':
            {
                int y = stringToInt(optarg);
                if (errno == 0)
                {
                    task.y = y;
                    break;
                }
                else
                {
                    printf("Overflow x\n");
                    return;
                }
            }
            case 'o':
            {
                task.op = *optarg;
                break;
            }
            case ':':
            {
                printf("No arguments for %c\n", optopt);
                return;
            }
            case '?':
            {
                printf("Unknown option %c\n", optopt);
                return;
            }
            case 'h':

            default:
            {
                return;
            }
        }
    }
    task.status = true;
}

void makeCalculate(Task& task)
{
    if (!task.status)
    {
        printf("Task not ready for calculating\n");
        return;
    }
    task.status = false;
    math::Result result;
    switch (task.op)
    {
        case '+':
        {
            result = math::add(task.x, task.y);
            break;
        }
        case '-':
        {
            result = math::sub(task.x, task.y);
            break;
        }
        case '*':
        {
            result = math::mul(task.x, task.y);
            break;
        }
        case '/':
        {
            result = math::div(task.x, task.y);
            break;
        }
        case '^':
        {
            result = math::pow(task.x, task.y);
            break;
        }
        case '!':
        {
            result = math::factorial(task.x);
            break;
        }
        default:
        {
            printf("Unknown operator %c\n", task.op);
            return;
        }
    }
    if (result.error)
    {
        printf("Calculation error\n");
        return;
    }
    task.result = result.result;
    task.status = true;
}

void applicationRun(int argc, char* argv[])
{
    Task task;
    makeTask(argc, argv, task);
    makeCalculate(task);
    printResult(task);
}

void printResult(const Task& task)
{
    if (task.status)
    {
        printf("%d\n", task.result);
        return;
    }
}

int main(int argc, char* argv[])
{
    applicationRun(argc, argv);
}
