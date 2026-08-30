#ifndef INTERACT_H
#define INTERACT_H
#include<bits/stdc++.h>
using namespace std;

extern unordered_map<string, string> variables;

void init();
void divider();
void exit_os();
void clear_os();
void rememberCommand(string cmd, string md);
void checkExpressionWithMemory();
void handleMemoryCommand(string cmd);
void findMemory();
void handleCheckCommand(string param="");
void handleVarCommand();
string substituteVariables(const string& expr);

#endif