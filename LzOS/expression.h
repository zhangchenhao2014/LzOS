#ifndef EXPRESSION_H
#define EXPRESSION_H
#include<bits/stdc++.h>
using namespace std;

bool isRightAssociative(char op);
long long qpow(long long base, long long exp);
void evaluate(stack<double>& numbers, stack<char>& operators);
void evaluateLogic(stack<double>& numbers, stack<char>& operators);
bool isOperator(char c);
bool isLogicOperator(char c);
double evaluateExpr(const string& expr, const string& mode);
void handle(string expression, string mode);
bool isExpression(const string& str,string mode); 
void checkExpression();

#endif