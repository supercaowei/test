#include <iostream>
#include <vector>

int calcGcd(int x, int y) {
    x = abs(x);
    y = abs(y);
    if (x == 0 || y == 0) {
        return x == 0 ? y : x;
    }
    while (x != y) {
        (x > y) ? (x = x - y) : (y = y - x);
    }
    return x;
}

enum Operater {
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE
};

const char* OperaterStr[] = {"+", "-", "×", "÷"};

struct Num {
    int num;
    int den;
    bool valid;

    Num(int n = 0, int d = 1) {
        setValue(n, d);
    }

    void setValue(int n, int d = 1) {
        valid = d != 0;
        if (valid) {
            int gcd = calcGcd(n, d);
            num = n / gcd;
            den = d / gcd;
            if (den < 0) {
                num *= -1;
                den *= -1;
            }
        }
    }

    std::string toString() {
        if (!valid) {
            return "INVALID";
        }
        std::string ret = (den == 1) ? std::to_string(num) : std::to_string(num) + '/' + std::to_string(den);
        if (num * den < 0) {
            ret = "(" + ret + ")";
        }
        return ret;
    }

    bool operator==(const Num& other) const {
        return valid && other.valid && (num * other.den == den * other.num);
    }

    bool operator!=(const Num& other) const {
        return !(*this == other);
    }

    static Num Calc(const Num& operand1, const Num& operand2, Operater operater) {
        if (!operand1.valid || !operand2.valid) {
            return Num(0, 0); //invalid
        }

        switch (operater)
        {
        case PLUS:
        case MINUS:
        {
            int i1 = operand1.num * operand2.den;
            int i2 = operand2.num * operand1.den;
            int num = operater == PLUS ? (i1 + i2) : (i1 - i2);
            int den = operand1.den * operand2.den;
            return Num(num, den);
        }
        case MULTIPLY:
            return Num(operand1.num * operand2.num, operand1.den * operand2.den);
        case DIVIDE:
            return Num(operand1.num * operand2.den, operand1.den * operand2.num);
        default:
            return Num(0, 0); //invalid
        }
    }
};

struct Equation {
    Num operand1;
    Num operand2;
    Num value;
    Operater operater;

    Equation(const Num& op1, const Num& op2, const Num& val, Operater opt) 
        : operand1(op1), operand2(op2), value(val), operater(opt) {}

    std::string toString() {
        return operand1.toString() + " " + OperaterStr[operater] + " " 
            + operand2.toString() + " = " + value.toString();
    }

    bool operator==(const Equation& other) const {
        bool operandEqual = (operand1 == other.operand1) && (operand2 == other.operand2);
        if (operater == PLUS || operater == MULTIPLY) {//加法/乘法交换律
            operandEqual = operandEqual || (operand1 == other.operand2) && (operand2 == other.operand1);
        }
        return operandEqual && value == other.value && operater == other.operater;
    }

    bool operator!=(const Equation& other) const {
        return !(*this == other);
    }
};

typedef std::vector<Equation> Solution;

struct CalcItem {
    Num value;
    Solution equations;

    CalcItem() = default;
    CalcItem(int in) : value(in) {}
    bool valid() { return value.valid; }

    static CalcItem calc(const CalcItem& operand1, const CalcItem& operand2, Operater operater) {
        CalcItem result;
        result.equations.insert(result.equations.end(), operand1.equations.begin(), operand1.equations.end());
        result.equations.insert(result.equations.end(), operand2.equations.begin(), operand2.equations.end());
        result.value = Num::Calc(operand1.value, operand2.value, operater);
        result.equations.push_back(Equation(operand1.value, operand2.value, result.value, operater));
        return result;
    }
};

void calc24(const std::vector<CalcItem>& vec, std::vector<Solution>& solutions) {
    if (vec.empty()) {
        return;
    }

    if (vec.size() == 1) {
        if (vec.front().value.num == 24 && vec.front().value.den == 1) {
            const Solution& equations = vec.front().equations;
            bool equal = false;
            for (const auto& solution : solutions) {
                equal = solution.size() == equations.size();
                for (int i = 0; equal && i < solution.size(); i++) {
                    if (equations[i] != solution[i]) {
                        equal = false;
                    }
                }
                if (equal) {
                    break;
                }
            }
            if (!equal) {
                solutions.push_back(vec.front().equations);
            }
        }
        return;
    }

    //下标i和j从vec中取2个数
    //k表示四则运算中的一个
    for (int i = 0; i < vec.size(); i++) {
        for (int j = 0; j < vec.size(); j++) {
            if (i == j) {
                continue;
            }
            for (char k = 0; k < 4; k++) {
                CalcItem result = CalcItem::calc(vec[i], vec[j], (Operater)k);
                if (!result.valid()) {
                    continue;
                }
                std::vector<CalcItem> newVec;
                newVec.push_back(result);
                for (int m = 0; m < vec.size(); m++) {
                    if (m != i && m != j) {
                        newVec.push_back(vec[m]);
                    }
                }
                calc24(newVec, solutions);
            }
        }
    }
}

int main() {
    std::cout << "请输入4个1~13之间的整数，每个数以回车键结束：" << std::endl;
    std::vector<CalcItem> vec;
    int in = 0;
    for (int i = 0; i < 4; i++) {
        std::cin >> in;
        if (in < 1 || in > 13) {
            std::cout << "输入的数" << in << "不符合要求，请重新输入第" << (i + 1) << "个数：" << std::endl;
            i--;
            continue;
        }
        vec.push_back(CalcItem(in));
    }

    std::vector<Solution> solutions;
    calc24(vec, solutions);
    if (!solutions.empty()) {
        for (int i = 0; i < solutions.size(); i++) {
            std::cout << "计算24方案" << (i + 1) << "：" << std::endl;
            for (int j = 0; j < solutions[i].size(); j++) {
                std::cout << solutions[i][j].toString() << std::endl;
            }
        }
    } else {
        std::cout << "此组数无法计算出24。" << std::endl;
    }
    return 0;
}