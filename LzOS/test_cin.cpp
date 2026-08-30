#include <iostream>
#include <string>
using namespace std;

int main() {
    string arg;
    while (cin >> arg) {
        cout << "[" << arg << "]\n";
        if (cin.peek() == '\n' || cin.peek() == '\r') break;
    }
    return 0;
}