#include<bits/stdc++.h>
#include "expression.h"
#include "interact.h"
using namespace std;

string op, s, mode;
string rememberedOp = "";
string rememberedMode = "";
bool hasMemory = false;
unordered_map<string, string> variables;

int main(int argc, char** argv) {
    init();
    divider();
    while(1) {
        cin >> op;
        if(op == "/e") {
            checkExpressionWithMemory();
        }
        else if(op == "mr+") {
            string cmd;
            cin >> cmd;
            handleMemoryCommand(cmd);
        }
        else if(op == "var") {
            handleVarCommand();
        }
        else if(op == "clear") {
            clear_os();
            hasMemory = false;
            rememberedOp = "";
            rememberedMode = "";
            s = "";
            cout << "已清空聊天记录\n";
        }
        else if(op == "exit") {
            exit_os();
        }
        else if(op == "check") {
            string param;
            cin >> param;
            handleCheckCommand(param);
        }
        else {
            if(hasMemory && rememberedOp == "/e") {
                string expr = op;
                string arg;
                while(cin.peek() != '\n' && cin.peek() != '\r' && cin >> arg) {
                    expr += " " + arg;
                }
                s = expr;
                handle(expr, rememberedMode);
            } else {
                cout << "未知指令: " << op << "，输入错误\n";
            }
        }
        divider();
    }
    
    return 0;
}