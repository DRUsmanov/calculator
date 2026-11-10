#include <string_view>

namespace calculator {

struct Task
{
    int value_1{0};
    int value_2{0};
    char op {'\0'};
    int result;
    bool status = false;
};

class Application {
public:
    void run (int argc, char* argv[]);

private:
    void makeTask(int argc, char* argv[]);
    void makeCalculate();
    void printResult() const;
    void parseFunction(std::string_view function);
    bool processFlags(int argc, char* argv[]) const;
protected:
    Task task_;
    static constexpr std::string_view help_{
        R"(Use: calculator '{"value_1":<value_1>, "op":<operator>, "value_2":<value_2>}'
        Available operators: +, -, *, /, ^, !)"
    };
};    

} // namespace calculator
