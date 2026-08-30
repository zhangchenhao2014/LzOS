#include "interact.h"
#include "expression.h"
using namespace std;

extern string op, s, mode;
extern string rememberedOp;
extern string rememberedMode;
extern bool hasMemory;

void init() {
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
        vector<string> args;
        string arg;
        while(cin >> arg) {
            args.push_back(arg);
            if(cin.peek() == '\n' || cin.peek() == '\r') break;
        }
        
        string md = "";
        string expr = "";
        size_t startIdx = 0;
        
        if(!args.empty() && args[0] == "tf") {
            md = "tf";
            startIdx = 1;
        }
        
        for(size_t i = startIdx; i < args.size(); i++) {
            if(i > startIdx) expr += " ";
            expr += args[i];
        }
        
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
    vector<string> args;
    string arg;
    while(cin >> arg) {
        args.push_back(arg);
        if(cin.peek() == '\n' || cin.peek() == '\r') break;
    }
    
    string currentMode = "";
    string expression = "";
    size_t startIdx = 0;
    
    if(!args.empty() && args[0] == "tf") {
        currentMode = "tf";
        startIdx = 1;
    }
    
    for(size_t i = startIdx; i < args.size(); i++) {
        if(i > startIdx) expression += " ";
        expression += args[i];
    }
    
    s = expression;
    mode = currentMode;
    
    if(hasMemory && rememberedOp == "/e") {
        string md = rememberedMode;
        string checkExpr = expression;
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
        
        if(expression.empty() || !isExpression(checkExpr, md)) {
            cout << "错误：数字表达式或逻辑表达式有问题\n";
            return;
        }
        
        handle(expression, md);
        return;
    }
    
    string checkExpr = expression;
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
    
    if(expression.empty() || !isExpression(checkExpr, currentMode)) {
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
    } else {
        cout << "未知参数: " << param << "\n";
        cout << "合法参数: mr\n";
    }
}