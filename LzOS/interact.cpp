#include "interact.h"
#include "expression.h"
using namespace std;

extern "C" {
    int __stdcall SetConsoleOutputCP(unsigned int wCodePageID);
    int __stdcall SetConsoleCP(unsigned int wCodePageID);
}

extern string op, s, mode;
extern string rememberedOp;
extern string rememberedMode;
extern bool hasMemory;

void init() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    cout << "LzOS v1.0\n";
    cout << "LzOS启动成功\n";
}

void divider() {
    cout << "--------------------\n";
}

void exit_os() {
    cout << "感谢使用，输入y确认退出，输入n取消\n";
    char op;
    cin >> op;
    if(op == 'y' || op == 'Y') {
        exit(0);
    } 
    else if(op == 'n' || op == 'N') {
        system("cls");
        divider();
    }
    else {
        cout << "输入无效字符: " << op << endl;
        system("pause");
        system("cls");
        exit_os();
    }
}

void clear_os() {
    system("cls");
    init();
}

void rememberCommand(string cmd, string md) {
    rememberedOp = cmd;
    rememberedMode = md;
    hasMemory = true;
    cout << "已记忆指令: " << cmd << " (模式: " << (md.empty() ? "普通" : md) << ")" << endl;
}

void executeCommand(string cmd, string md) {
    if(cmd == "/e") {
        if(!s.empty()) {
            handle(s, md);
        } else {
            cout << "错误：表达式为空" << endl;
        }
    }
    else if(cmd == "exit") {
        exit_os();
    }
    else if(cmd == "clear") {
        clear_os();
    }
    else {
        cout << "未知指令: " << cmd << "，输入错误" << endl;
    }
}

void handleMemoryCommand(string cmd) {
    if(cmd == "/e") {
        string line;
        getline(cin, line);
        if(!line.empty() && line.back() == '\r') line.pop_back();
        
        string md = "";
        string expr = "";
        size_t pos = 0;
        
        while (pos < line.size() && isspace(line[pos])) pos++;
        
        if(pos < line.size() && line.substr(pos, 2) == "tf") {
            md = "tf";
            pos += 2;
        }
        else if(pos < line.size() && line.substr(pos, 2) == "bw") {
            md = "bitwise";
            pos += 2;
        }
        
        while (pos < line.size() && isspace(line[pos])) pos++;
        expr = line.substr(pos);
        
        s = expr;
        mode = md;
        rememberCommand(cmd, md);
        
        if(!expr.empty()) {
            handle(expr, md);
        }
    } else if(cmd == "exit" || cmd == "clear") {
        rememberCommand(cmd, "");
    } else {
        cout << "无法记忆 " << cmd << " 指令" << endl;
    }
}

void findMemory() {
    if(hasMemory && rememberedOp == "/e") {
        string expr = op;
        string arg;
        while(cin.peek() != '\n' && cin.peek() != '\r' && cin >> arg) {
            expr += " " + arg;
        }
        s = expr;
        handle(expr, rememberedMode);
    } else {
        cout << "未知指令: " << op << "，输入错误" << endl;
    }
}

void checkExpressionWithMemory() {
    string line;
    getline(cin, line);
    if(!line.empty() && line.back() == '\r') line.pop_back();
    
    string currentMode = "";
    string expression = "";
    size_t pos = 0;
    
    while (pos < line.size() && isspace(line[pos])) pos++;
    
    if(pos < line.size() && line.substr(pos, 2) == "tf") {
        currentMode = "tf";
        pos += 2;
    }
    else if(pos < line.size() && line.substr(pos, 2) == "bw") {
        currentMode = "bitwise";
        pos += 2;
    }
    
    while (pos < line.size() && isspace(line[pos])) pos++;
    expression = line.substr(pos);
    
    s = expression;
    mode = currentMode;
    
    if(hasMemory && rememberedOp == "/e") {
        string md = rememberedMode;
        string resolvedExpr = substituteVariables(expression);
        string checkExpr = resolvedExpr;
        if(md == "tf") {
            size_t pos = 0;
            while((pos = checkExpr.find("&&", pos)) != string::npos) {
                checkExpr.replace(pos, 2, "&");
                pos += 1;
            }
            pos = 0;
            while((pos = checkExpr.find("||", pos)) != string::npos) {
                checkExpr.replace(pos, 2, "|");
                pos += 1;
            }
            pos = 0;
            while((pos = checkExpr.find("==", pos)) != string::npos) {
                checkExpr.replace(pos, 2, "=");
                pos += 1;
            }
            pos = 0;
            while((pos = checkExpr.find("!=", pos)) != string::npos) {
                checkExpr.replace(pos, 2, "#");
                pos += 1;
            }
            pos = 0;
            while((pos = checkExpr.find(">=", pos)) != string::npos) {
                checkExpr.replace(pos, 2, "G");
                pos += 1;
            }
            pos = 0;
            while((pos = checkExpr.find("<=", pos)) != string::npos) {
                checkExpr.replace(pos, 2, "L");
                pos += 1;
            }
            pos = 0;
            while((pos = checkExpr.find("!", pos)) != string::npos) {
                checkExpr.replace(pos, 1, "~");
                pos += 1;
            }
        }
        else if(md == "bitwise") {
            size_t pos = 0;
            while((pos = checkExpr.find("<<", pos)) != string::npos) {
                checkExpr.replace(pos, 2, "l");
                pos += 1;
            }
            pos = 0;
            while((pos = checkExpr.find(">>", pos)) != string::npos) {
                checkExpr.replace(pos, 2, "r");
                pos += 1;
            }
        }
        
        if(resolvedExpr.empty() || !isExpression(checkExpr, md)) {
            cout << "错误：数字表达式或逻辑表达式有问题\n";
            return;
        }
        
        handle(expression, md);
        return;
    }
    
    string resolvedExpr = substituteVariables(expression);
    string checkExpr = resolvedExpr;
    if(currentMode == "tf") {
        size_t pos = 0;
        while((pos = checkExpr.find("&&", pos)) != string::npos) {
            checkExpr.replace(pos, 2, "&");
            pos += 1;
        }
        pos = 0;
        while((pos = checkExpr.find("||", pos)) != string::npos) {
            checkExpr.replace(pos, 2, "|");
            pos += 1;
        }
        pos = 0;
        while((pos = checkExpr.find("==", pos)) != string::npos) {
            checkExpr.replace(pos, 2, "=");
            pos += 1;
        }
        pos = 0;
        while((pos = checkExpr.find("!=", pos)) != string::npos) {
            checkExpr.replace(pos, 2, "#");
            pos += 1;
        }
        pos = 0;
        while((pos = checkExpr.find(">=", pos)) != string::npos) {
            checkExpr.replace(pos, 2, "G");
            pos += 1;
        }
        pos = 0;
        while((pos = checkExpr.find("<=", pos)) != string::npos) {
            checkExpr.replace(pos, 2, "L");
            pos += 1;
        }
        pos = 0;
        while((pos = checkExpr.find("!", pos)) != string::npos) {
            checkExpr.replace(pos, 1, "~");
            pos += 1;
        }
    }
    else if(currentMode == "bitwise") {
        size_t pos = 0;
        while((pos = checkExpr.find("<<", pos)) != string::npos) {
            checkExpr.replace(pos, 2, "l");
            pos += 1;
        }
        pos = 0;
        while((pos = checkExpr.find(">>", pos)) != string::npos) {
            checkExpr.replace(pos, 2, "r");
            pos += 1;
        }
    }
    
    if(resolvedExpr.empty() || !isExpression(checkExpr, currentMode)) {
        cout << "错误：数字表达式或逻辑表达式有问题\n";
        return;
    }
    
    handle(expression, currentMode);
}
void handleCheckCommand(string param) {
    if(param == "mr") {
        if(hasMemory) {
            if(rememberedOp == "/e") {
                cout << "模式为: /e " << (rememberedMode.empty() ? "" : rememberedMode) << "\n";
            } else {
                cout << "记忆指令: " << rememberedOp << "\n";
            }
        } else {
            cout << "没有记忆\n";
        }
    } else if(param == "var") {
        if(variables.empty()) {
            cout << "没有存储的变量\n";
        } else {
            cout << "存储的变量:\n";
            for(auto& p : variables) {
                cout << "  " << p.first << " = " << p.second << "\n";
            }
        }
    } else {
        cout << "未知参数: " << param << "\n";
        cout << "合法参数: mr, var\n";
    }
}

bool isValidVarName(const string& name) {
    if(name.empty()) return false;
    if(!isalpha(name[0])) return false;
    for(char c : name) {
        if(!isalnum(c)) return false;
    }
    return true;
}

string substituteVariables(const string& expr) {
    string result = expr;
    bool changed = true;
    int iterations = 0;
    while(changed && iterations < 100) {
        changed = false;
        iterations++;
        for(auto& p : variables) {
            size_t pos = 0;
            while((pos = result.find(p.first, pos)) != string::npos) {
                bool leftOk = (pos == 0) || !isalnum(result[pos-1]);
                size_t endPos = pos + p.first.size();
                bool rightOk = (endPos >= result.size()) || !isalnum(result[endPos]);
                if(leftOk && rightOk) {
                    result.replace(pos, p.first.size(), p.second);
                    changed = true;
                } else {
                    pos++;
                }
            }
        }
    }
    return result;
}

void handleVarCommand() {
    string rest;
    getline(cin, rest);
    if(!rest.empty() && rest.back() == '\r') rest.pop_back();
    size_t eqPos = rest.find('=');
    if(eqPos == string::npos) {
        cout << "错误：var指令格式应为 var 变量名 = 表达式\n";
        return;
    }
    string varName = rest.substr(0, eqPos);
    string expr = rest.substr(eqPos + 1);
    
    size_t start = 0;
    while(start < varName.size() && varName[start] == ' ') start++;
    size_t end = varName.size() - 1;
    while(end >= start && varName[end] == ' ') end--;
    varName = varName.substr(start, end - start + 1);
    
    size_t estart = 0;
    while(estart < expr.size() && expr[estart] == ' ') estart++;
    size_t eend = expr.size() - 1;
    while(eend >= estart && expr[eend] == ' ') eend--;
    expr = expr.substr(estart, eend - estart + 1);
    
    if(!isValidVarName(varName)) {
        cout << "错误：变量名必须以字母开头，只能包含字母和数字\n";
        return;
    }
    
    string resolved = substituteVariables(expr);
    
    string checkExpr = resolved;
    size_t pos = 0;
    while((pos = checkExpr.find("&&", pos)) != string::npos) {
        checkExpr.replace(pos, 2, "&");
        pos += 1;
    }
    pos = 0;
    while((pos = checkExpr.find("||", pos)) != string::npos) {
        checkExpr.replace(pos, 2, "|");
        pos += 1;
    }
    pos = 0;
    while((pos = checkExpr.find("==", pos)) != string::npos) {
        checkExpr.replace(pos, 2, "=");
        pos += 1;
    }
    pos = 0;
    while((pos = checkExpr.find("!=", pos)) != string::npos) {
        checkExpr.replace(pos, 2, "#");
        pos += 1;
    }
    pos = 0;
    while((pos = checkExpr.find(">=", pos)) != string::npos) {
        checkExpr.replace(pos, 2, "G");
        pos += 1;
    }
    pos = 0;
    while((pos = checkExpr.find("<=", pos)) != string::npos) {
        checkExpr.replace(pos, 2, "L");
        pos += 1;
    }
    pos = 0;
    while((pos = checkExpr.find("!", pos)) != string::npos) {
        checkExpr.replace(pos, 1, "~");
        pos += 1;
    }
    pos = 0;
    while((pos = checkExpr.find("<<", pos)) != string::npos) {
        checkExpr.replace(pos, 2, "l");
        pos += 1;
    }
    pos = 0;
    while((pos = checkExpr.find(">>", pos)) != string::npos) {
        checkExpr.replace(pos, 2, "r");
        pos += 1;
    }
    
    if(!isExpression(checkExpr, "")) {
        cout << "错误：表达式有问题，无法存储变量\n";
        return;
    }
    
    double val = evaluateExpr(resolved, "");
    string result;
    if(val == (long long)val) {
        result = to_string((long long)val);
    } else {
        result = to_string(val);
    }
    variables[varName] = result;
    cout << "已存储变量: " << varName << " = " << result << "\n";
}