#include<bits/stdc++.h>
#include "expression.h"
#include "interact.h"
using namespace std;

static unordered_map<string, int> mathFunctions = {
    {"sqrt", 1},
    {"abs", 1},
    {"log", 1},
    {"log10", 1},
    {"log2", 1},
    {"sin", 1},
    {"cos", 1},
    {"tan", 1},
    {"asin", 1},
    {"acos", 1},
    {"atan", 1},
    {"ceil", 1},
    {"floor", 1},
    {"round", 1},
    {"exp", 1},
    {"pow", 2},
    {"max", 2},
    {"min", 2},
    {"cbrt", 1},
    {"hypot", 2}
};

double applyMathFunction(const string& name, const vector<double>& args) {
    if(name == "sqrt") {
        if(args[0] < 0) throw runtime_error("sqrt参数不能为负数");
        return sqrt(args[0]);
    }
    if(name == "abs") return abs(args[0]);
    if(name == "log") {
        if(args[0] <= 0) throw runtime_error("log参数必须为正数");
        return log(args[0]);
    }
    if(name == "log10") {
        if(args[0] <= 0) throw runtime_error("log10参数必须为正数");
        return log10(args[0]);
    }
    if(name == "log2") {
        if(args[0] <= 0) throw runtime_error("log2参数必须为正数");
        return log2(args[0]);
    }
    if(name == "sin") return sin(args[0]);
    if(name == "cos") return cos(args[0]);
    if(name == "tan") return tan(args[0]);
    if(name == "asin") {
        if(args[0] < -1 || args[0] > 1) throw runtime_error("asin参数必须在[-1,1]范围内");
        return asin(args[0]);
    }
    if(name == "acos") {
        if(args[0] < -1 || args[0] > 1) throw runtime_error("acos参数必须在[-1,1]范围内");
        return acos(args[0]);
    }
    if(name == "atan") return atan(args[0]);
    if(name == "ceil") return ceil(args[0]);
    if(name == "floor") return floor(args[0]);
    if(name == "round") return round(args[0]);
    if(name == "exp") return exp(args[0]);
    if(name == "pow") return pow(args[0], args[1]);
    if(name == "max") return max(args[0], args[1]);
    if(name == "min") return min(args[0], args[1]);
    if(name == "cbrt") return cbrt(args[0]);
    if(name == "hypot") return hypot(args[0], args[1]);
    throw runtime_error("未知函数: " + name);
}

static unordered_map<char,int> arithmeticPrecedence = {
    {'+', 1},
    {'-', 1},
    {'*', 2},
    {'/', 2},
    {'^', 3}
};

static unordered_map<char,int> bitwisePrecedence = {
    {'~', 5},
    {'&', 4},
    {'^', 3},
    {'|', 2},
    {'l', 1},
    {'r', 1}
};

bool isRightAssociative(char op) {
    return op == '^' || op == 'l' || op == 'r';
}

long long qpow(long long base, long long exp) {
    long long result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

void evaluate(stack<double>& numbers, stack<char>& operators) {
    double right = numbers.top(); numbers.pop();
    double left = numbers.top(); numbers.pop();
    char op = operators.top(); operators.pop();
    double result = 0;
    switch (op) {
        case '+': result = left + right; break;
        case '-': result = left - right; break;
        case '*': result = left * right; break;
        case '/': 
            if (right == 0) throw runtime_error("除数不能为0");
            result = left / right; 
            break;
        case '^': result = pow(left, right); break;
    }
    numbers.push(result);
}

void evaluateBitwise(stack<double>& numbers, stack<char>& operators) {
    if (operators.top() == '~') {
        operators.pop();
        double val = numbers.top(); numbers.pop();
        long long lv = (long long)val;
        numbers.push((double)(~lv));
        return;
    }
    double right = numbers.top(); numbers.pop();
    double left = numbers.top(); numbers.pop();
    char op = operators.top(); operators.pop();
    long long l = (long long)left;
    long long r = (long long)right;
    double result = 0;
    switch (op) {
        case '&': result = (double)(l & r); break;
        case '|': result = (double)(l | r); break;
        case '^': result = (double)(l ^ r); break;
        case 'l': result = (double)(l << r); break;
        case 'r': result = (double)(l >> r); break;
    }
    numbers.push(result);
}

void evaluateLogic(stack<double>& numbers, stack<char>& operators) {
    double right = numbers.top(); numbers.pop();
    double left = numbers.top(); numbers.pop();
    char op = operators.top(); operators.pop();
    double result = 0;
    switch (op) {
        case '&': result = (left != 0 && right != 0) ? 1.0 : 0.0; break;
        case '|': result = (left != 0 || right != 0) ? 1.0 : 0.0; break;
        case '=': result = (left == right) ? 1.0 : 0.0; break;
        case '#': result = (left != right) ? 1.0 : 0.0; break;
        case 'G': result = (left >= right) ? 1.0 : 0.0; break;
        case 'L': result = (left <= right) ? 1.0 : 0.0; break;
        case '>': result = (left > right) ? 1.0 : 0.0; break;
        case '<': result = (left < right) ? 1.0 : 0.0; break;
    }
    numbers.push(result);
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool isLogicOperator(char c) {
    return c == '&' || c == '|' || c == '~' || c == '=' || c == '#' || c == 'G' || c == 'L' || c == '>' || c == '<';
}

bool isBitwiseOperator(char c) {
    return c == '&' || c == '|' || c == '^' || c == '~' || c == 'l' || c == 'r';
}

double evaluateExpr(const string& expr, const string& mode) {
    string expression = expr;
    expression.erase(remove_if(expression.begin(), expression.end(), ::isspace), expression.end());
    
    if (expression.empty()) return 0;
    
    if (mode == "tf") {
        size_t pos = 0;
        
        vector<pair<string, string>> replacements = {
            {"&&", "&"},
            {"||", "|"},
            {"==", "="},
            {"!=", "#"},
            {">=", "G"},
            {"<=", "L"},
            {">", ">"},
            {"<", "<"},
            {"!", "~"}
        };
        
        for (auto& rep : replacements) {
            pos = 0;
            while ((pos = expression.find(rep.first, pos)) != string::npos) {
                expression.replace(pos, rep.first.length(), rep.second);
                pos += rep.second.length();
            }
        }
        
        pos = 0;
        while ((pos = expression.find("true", pos)) != string::npos) {
            expression.replace(pos, 4, "1");
            pos += 1;
        }
        pos = 0;
        while ((pos = expression.find("false", pos)) != string::npos) {
            expression.replace(pos, 5, "0");
            pos += 1;
        }
    }
    
    if (mode == "bitwise") {
        size_t pos = 0;
        while ((pos = expression.find("<<", pos)) != string::npos) {
            expression.replace(pos, 2, "l");
            pos += 1;
        }
        pos = 0;
        while ((pos = expression.find(">>", pos)) != string::npos) {
            expression.replace(pos, 2, "r");
            pos += 1;
        }
    }
    
    stack<double> numbers;
    stack<char> operators;
    size_t i = 0;
    
    static unordered_map<char,int> logicPrecedence = {
        {'~', 4},
        {'G', 3},
        {'L', 3},
        {'>', 3},
        {'<', 3},
        {'=', 3},
        {'#', 3},
        {'&', 2},
        {'|', 1}
    };
    
    bool useBitwise = (mode == "bitwise");
    
    while (i < expression.length()) {
        char c = expression[i];
        
        if (isdigit(c) || c == '.') {
            string numStr;
            bool hasDot = false;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                if (expression[i] == '.') {
                    if (hasDot) throw runtime_error("数字表达式错误：多个小数点");
                    hasDot = true;
                }
                numStr += expression[i];
                i++;
            }
            numbers.push(stod(numStr));
            continue;
        }
        else if (isalpha(c)) {
            string funcName;
            while (i < expression.length() && (isalpha(expression[i]) || isdigit(expression[i]))) {
                funcName += expression[i];
                i++;
            }
            if (mathFunctions.count(funcName)) {
                if (i >= expression.length() || expression[i] != '(') {
                    throw runtime_error("函数 " + funcName + " 缺少括号");
                }
                i++;
                
                vector<string> paramStrs;
                int depth = 1;
                string currentParam;
                while (i < expression.length() && depth > 0) {
                    if (expression[i] == '(') {
                        depth++;
                        currentParam += expression[i];
                    }
                    else if (expression[i] == ')') {
                        depth--;
                        if (depth > 0) currentParam += expression[i];
                    }
                    else if (expression[i] == ',' && depth == 1) {
                        paramStrs.push_back(currentParam);
                        currentParam = "";
                    }
                    else {
                        currentParam += expression[i];
                    }
                    i++;
                }
                if (depth != 0) throw runtime_error("函数 " + funcName + " 括号不匹配");
                paramStrs.push_back(currentParam);
                
                int expectedArgs = mathFunctions[funcName];
                if ((int)paramStrs.size() != expectedArgs) {
                    throw runtime_error("函数 " + funcName + " 需要 " + to_string(expectedArgs) + " 个参数");
                }
                
                vector<double> args;
                for (auto& ps : paramStrs) {
                    string resolved = substituteVariables(ps);
                    double val = evaluateExpr(resolved, mode);
                    args.push_back(val);
                }
                
                numbers.push(applyMathFunction(funcName, args));
                continue;
            }
            else if (mode == "tf" && (funcName == "true" || funcName == "false")) {
                numbers.push(funcName == "true" ? 1.0 : 0.0);
                continue;
            }
            else {
                throw runtime_error("未知标识符: " + funcName);
            }
        }
        else if (c == '(') {
            operators.push(c);
            i++;
        }
        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                if (mode == "tf" && isLogicOperator(operators.top())) {
                    evaluateLogic(numbers, operators);
                } else if (useBitwise && isBitwiseOperator(operators.top())) {
                    evaluateBitwise(numbers, operators);
                } else {
                    evaluate(numbers, operators);
                }
            }
            if (operators.empty()) throw runtime_error("括号不匹配");
            operators.pop();
            i++;
        }
        else if (mode == "tf" && isLogicOperator(c)) {
            if (c == '~') {
                bool isUnary = (i == 0) || 
                               (expression[i-1] == '(') || 
                               isLogicOperator(expression[i-1]) || 
                               isOperator(expression[i-1]);
                
                if (!isUnary) throw runtime_error("逻辑非运算格式错误");
                
                i++;
                if (i >= expression.length()) throw runtime_error("逻辑非运算缺少操作数");
                char nextC = expression[i];
                if (isdigit(nextC) || nextC == '.') {
                    string numStr;
                    bool hasDot = false;
                    while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                        if (expression[i] == '.') {
                            if (hasDot) throw runtime_error("数字表达式错误：多个小数点");
                            hasDot = true;
                        }
                        numStr += expression[i];
                        i++;
                    }
                    double val = stod(numStr);
                    numbers.push(val == 0 ? 1.0 : 0.0);
                    continue;
                }
                else if (nextC == '(') {
                    int depth = 1;
                    size_t start = i + 1;
                    size_t end = i + 1;
                    while (end < expression.length() && depth > 0) {
                        if (expression[end] == '(') depth++;
                        else if (expression[end] == ')') depth--;
                        end++;
                    }
                    string innerExpr = expression.substr(start, end - start - 1);
                    double val = evaluateExpr(innerExpr, mode);
                    numbers.push(val == 0 ? 1.0 : 0.0);
                    i = end;
                    continue;
                }
                else if (isLogicOperator(nextC)) {
                    throw runtime_error("逻辑非运算缺少操作数");
                }
                else {
                    throw runtime_error("逻辑非运算缺少操作数");
                }
            }
            
            int currentPrecedence = logicPrecedence[c];
            while (!operators.empty() && operators.top() != '(') {
                char topOp = operators.top();
                if (isLogicOperator(topOp)) {
                    if (logicPrecedence[topOp] >= currentPrecedence) {
                        evaluateLogic(numbers, operators);
                    } else {
                        break;
                    }
                } else {
                    break;
                }
            }
            operators.push(c);
            i++;
        }
        else if (useBitwise && isBitwiseOperator(c)) {
            if (c == '~') {
                bool isUnary = (i == 0) || 
                               (expression[i-1] == '(') || 
                               isBitwiseOperator(expression[i-1]) ||
                               isOperator(expression[i-1]);
                
                if (!isUnary) throw runtime_error("按位取反格式错误");
                
                i++;
                if (i >= expression.length()) throw runtime_error("按位取反缺少操作数");
                char nextC = expression[i];
                if (isdigit(nextC) || nextC == '.') {
                    string numStr;
                    bool hasDot = false;
                    while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                        if (expression[i] == '.') {
                            if (hasDot) throw runtime_error("数字表达式错误：多个小数点");
                            hasDot = true;
                        }
                        numStr += expression[i];
                        i++;
                    }
                    long long val = (long long)stod(numStr);
                    numbers.push((double)(~val));
                    continue;
                }
                else if (nextC == '(') {
                    int depth = 1;
                    size_t start = i + 1;
                    size_t end = i + 1;
                    while (end < expression.length() && depth > 0) {
                        if (expression[end] == '(') depth++;
                        else if (expression[end] == ')') depth--;
                        end++;
                    }
                    string innerExpr = expression.substr(start, end - start - 1);
                    double val = evaluateExpr(innerExpr, mode);
                    numbers.push((double)(~((long long)val)));
                    i = end;
                    continue;
                }
                else {
                    throw runtime_error("按位取反缺少操作数");
                }
            }
            
            int currentPrecedence = bitwisePrecedence[c];
            while (!operators.empty() && operators.top() != '(') {
                char topOp = operators.top();
                if (isBitwiseOperator(topOp)) {
                    if (isRightAssociative(c)) {
                        if (bitwisePrecedence[topOp] > currentPrecedence) {
                            evaluateBitwise(numbers, operators);
                        } else {
                            break;
                        }
                    } else {
                        if (bitwisePrecedence[topOp] >= currentPrecedence) {
                            evaluateBitwise(numbers, operators);
                        } else {
                            break;
                        }
                    }
                } else {
                    break;
                }
            }
            operators.push(c);
            i++;
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            if ((c == '-' || c == '+') && (i == 0 || expression[i-1] == '(' || isOperator(expression[i-1]))) {
                numbers.push(0);
            }
            
            while (!operators.empty() && operators.top() != '(') {
                char topOp = operators.top();
                if (isRightAssociative(c)) {
                    if (arithmeticPrecedence[topOp] > arithmeticPrecedence[c]) {
                        evaluate(numbers, operators);
                    } else {
                        break;
                    }
                } else {
                    if (arithmeticPrecedence[topOp] >= arithmeticPrecedence[c]) {
                        evaluate(numbers, operators);
                    } else {
                        break;
                    }
                }
            }
            operators.push(c);
            i++;
        }
        else {
            throw runtime_error(string("无效字符: ") + c);
        }
    }
    
    while (!operators.empty()) {
        if (operators.top() == '(' || operators.top() == ')') {
            throw runtime_error("括号不匹配");
        }
        if (mode == "tf" && isLogicOperator(operators.top())) {
            evaluateLogic(numbers, operators);
        } else if (useBitwise && isBitwiseOperator(operators.top())) {
            evaluateBitwise(numbers, operators);
        } else {
            evaluate(numbers, operators);
        }
    }
    
    if (numbers.empty()) return 0;
    return numbers.top();
}

void handle(string expression, string mode) {
    try {
        string resolved = substituteVariables(expression);
        double result = evaluateExpr(resolved, mode);
        
        if (mode == "tf") {
            if (result == 0) {
                cout << "false\n";
            } else {
                cout << "true\n";
            }
        } else {
            if (result == (long long)result) {
                cout << (long long)result << '\n';
            } else {
                cout << result << '\n';
            }
        }
    }
    catch (const exception& e) {
        cerr << "错误: " << e.what() << endl;
    }
}

bool isExpression(const string& str, string mode) {
    if (str.empty()) return false;
    
    stack<char> parentheses;
    bool hasOperand = false;
    bool expectOperand = true;
    bool isBitwiseMode = (mode == "bitwise");
    
    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        if (isspace(c)) continue;
        
        if (isdigit(c) || c == '.') {
            hasOperand = true;
            expectOperand = false;
            while (i < str.length() && (isdigit(str[i]) || str[i] == '.')) {
                i++;
            }
            i--;
        }
        else if (mode == "tf" && (c == '&' || c == '|' || c == '=' || c == '>' || c == '<' || c == '#' || c == '~' || c == 'G' || c == 'L')) {
            if (c == '~') {
                if (!expectOperand) return false;
                expectOperand = true;
                continue;
            }
            if (expectOperand) return false;
            hasOperand = true;
            expectOperand = true;
        }
        else if (isBitwiseMode && (c == '&' || c == '|' || c == '^' || c == '~' || c == 'l' || c == 'r')) {
            if (c == '~') {
                if (!expectOperand) return false;
                expectOperand = true;
                continue;
            }
            if (expectOperand) return false;
            hasOperand = true;
            expectOperand = true;
        }
        else if (isalpha(c)) {
            string word;
            size_t j = i;
            while (j < str.length() && (isalpha(str[j]) || isdigit(str[j]))) {
                word += str[j];
                j++;
            }
            if (mathFunctions.count(word)) {
                hasOperand = true;
                expectOperand = false;
                if (j >= str.length() || str[j] != '(') return false;
                int depth = 1;
                size_t k = j + 1;
                while (k < str.length() && depth > 0) {
                    if (str[k] == '(') depth++;
                    else if (str[k] == ')') depth--;
                    if (depth == 0) break;
                    k++;
                }
                if (depth != 0) return false;
                i = k;
                continue;
            }
            if (mode == "tf" && (word == "true" || word == "false")) {
                hasOperand = true;
                expectOperand = false;
                i = j - 1;
                continue;
            }
            return false;
        }
        else if (c == '(') {
            parentheses.push(c);
            expectOperand = true;
        }
        else if (c == ')') {
            if (parentheses.empty()) return false;
            parentheses.pop();
            expectOperand = false;
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            if (c == '-' && (i == 0 || str[i-1] == '(' || isOperator(str[i-1]))) {
                expectOperand = true;
                continue;
            }
            if (expectOperand) return false;
            hasOperand = true;
            expectOperand = true;
        }
        else if (c == ',') {
            if (expectOperand) return false;
            expectOperand = true;
            hasOperand = true;
        }
        else {
            return false;
        }
    }
    
    return parentheses.empty() && hasOperand && !expectOperand;
}

void checkExpression(){
    string line;
    getline(cin, line);
    if(!line.empty() && line.back() == '\r') line.pop_back();
    
    string mode = "";
    string expression = "";
    size_t pos = 0;
    
    while (pos < line.size() && isspace(line[pos])) pos++;
    
    if(pos < line.size() && line.substr(pos, 2) == "tf") {
        mode = "tf";
        pos += 2;
    }
    else if(pos < line.size() && line.substr(pos, 2) == "bw") {
        mode = "bitwise";
        pos += 2;
    }
    
    while (pos < line.size() && isspace(line[pos])) pos++;
    expression = line.substr(pos);
    
    string checkExpr = expression;
    if(mode == "tf"){
        size_t pos = 0;
        while((pos = checkExpr.find("&&", pos)) != string::npos){
            checkExpr.replace(pos, 2, "&");
            pos += 1;
        }
        pos = 0;
        while((pos = checkExpr.find("||", pos)) != string::npos){
            checkExpr.replace(pos, 2, "|");
            pos += 1;
        }
        pos = 0;
        while((pos = checkExpr.find("==", pos)) != string::npos){
            checkExpr.replace(pos, 2, "=");
            pos += 1;
        }
        pos = 0;
        while((pos = checkExpr.find("!=", pos)) != string::npos){
            checkExpr.replace(pos, 2, "#");
            pos += 1;
        }
        pos = 0;
        while((pos = checkExpr.find(">=", pos)) != string::npos){
            checkExpr.replace(pos, 2, "G");
            pos += 1;
        }
        pos = 0;
        while((pos = checkExpr.find("<=", pos)) != string::npos){
            checkExpr.replace(pos, 2, "L");
            pos += 1;
        }
        pos = 0;
        while((pos = checkExpr.find("!", pos)) != string::npos){
            checkExpr.replace(pos, 1, "~");
            pos += 1;
        }
    }
    else if(mode == "bitwise"){
        size_t pos = 0;
        while((pos = checkExpr.find("<<", pos)) != string::npos){
            checkExpr.replace(pos, 2, "l");
            pos += 1;
        }
        pos = 0;
        while((pos = checkExpr.find(">>", pos)) != string::npos){
            checkExpr.replace(pos, 2, "r");
            pos += 1;
        }
    }
    
    if(expression.empty() || !isExpression(checkExpr, mode)){
        cout << "错误：数字表达式或逻辑表达式有问题\n"; 
        return;
    }
    
    handle(expression, mode);
}