
#include "LexAnalyzer.h"

// pre: parameter refers to open data file consisting of token and
// lexeme pairs i.e.  t_and and
// Each pair appears on its own input line.
// post: tokenmap has been populated - key: lexeme, value: token

LexAnalyzer::LexAnalyzer(istream& infile) {
    string tokenName;
    string lexemeName;

    while (infile >> tokenName >> lexemeName) {
        tokenmap.insert(make_pair(lexemeName, tokenName));
    }
}

bool LexAnalyzer::isSymbol(const char c) {
    return (c == '{' || c == '}' || c == '+' || c == '-' ||
            c == '*' || c == '<' || c == '>' || c == ';' ||
            c == '(' || c == ')' || c == '=' || c == ':' || c == ',');
}
bool LexAnalyzer::isNumber(const char c) {
    return (c >= '0' && c <= '9');
}
bool LexAnalyzer::isAlpha(const char c) {
    return (c >= 'a' && c <= 'z' ) ||
           (c >= 'A' && c <= 'Z') ||
               c == '_';
}
bool LexAnalyzer::isWhitespace(const char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}
// pre: 1st parameter refers to an open text file that contains source
// code in the language, 2nd parameter refers to an open empty output
// file
// post: If no error, the token and lexeme pairs for the given input
// file have been written to the output file (token lexeme).
// If there is an error, the incomplete token/lexeme pairs, as well as
// an error message have been written to the output file.
// A success or fail message has printed to the console.
void LexAnalyzer::scanFile(istream& infile, ostream& outfile) {

}
