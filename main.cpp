#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

#include "LexAnalyzer.h"

int main() {

    ifstream lexemes("lexemes.txt");
    ifstream source("source.txt");
    if (!lexemes.is_open()) {
        cout << "Error: Could not open lexemes.txt" << endl;
        return 1;
    }
    if (!source.is_open()) {
        cout << "Error: Could not open source.txt" << endl;
        return 1;
    }
    ofstream sourcelexemes("sourcelexemes.txt");
    LexAnalyzer lexanalyzer(lexemes);
    lexanalyzer.scanFile(source, sourcelexemes);
    return 0;
}