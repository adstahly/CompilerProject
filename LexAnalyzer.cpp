
#include "LexAnalyzer.h"

#include <set>

// pre: parameter refers to open data file consisting of token and
// lexeme pairs i.e.  t_and and
// Each pair appears on its own input line.
// post: tokenmap has been populated - key: lexeme, value: token

LexAnalyzer::LexAnalyzer(istream& infile) {
    string token, lexeme;
    while (infile >> token >> lexeme) {
        tokenmap[lexeme] = token;
    }
}

// pre: 1st parameter refers to an open text file that contains source
// code in the language, 2nd parameter refers to an open empty output
// file
// post: If no error, the token and lexeme pairs for the given input
// file have been written to the output file (token : lexeme).
// If there is an error, the incomplete token/lexeme pairs, as well as
// an error message have been written to the output file.
// A success or fail message has printed to the console.

string LexAnalyzer::handleText(const string &line, int &pos) {  // MISSING ERROR HANDLING FOR MISSING QUOTE
    string stringBuilder = "";
    while (pos < line.length() && line[pos] != '"') {
        stringBuilder += line[pos];
        pos++;
    }
    if (pos < line.length() && line[pos] == '"') {
        pos++;
    }
    return stringBuilder;
}

string LexAnalyzer::handleNum(const string &line, int &pos) {
    string numBuilder = "";

    while (pos < line.length() && isdigit(line[pos])) {
        numBuilder += line[pos];
        pos++;
    }
    return numBuilder;
}

string LexAnalyzer::handleWord(const string &line, int &pos) {
    string stringBuilder = "";

    while (pos < line.length() && (isalnum(line[pos]) || line[pos] == '_')) {
        stringBuilder += line[pos];
        pos++;
    }
    return stringBuilder;
}


void LexAnalyzer::scanFile(istream& infile, ostream& outfile) {
    bool errorFound = false;
    string line;

    while (getline(infile, line) && !errorFound) {
        int pos = 0;

        while (pos < line.length() && !errorFound) {
            char ch = line[pos];

            if (isspace(ch)) {
                pos++;
                continue;  // ignore whitespace
            }
            if (ch == '"') {
                pos++;
                string stringBuilder = handleText(line, pos);
                outfile << "t_text" << " " << stringBuilder << endl;  // text
            } else if (isdigit(ch)) {
                string numBuilder = handleNum(line, pos);
                outfile << "t_number" << " " << numBuilder << endl;  // digit
            } else if (isalpha(ch)) {
                string stringBuilder = handleWord(line, pos);
                if (tokenmap.contains(stringBuilder))
                    outfile << tokenmap[stringBuilder] << " " << stringBuilder << endl;  // keyword
                else
                    outfile << "t_id" << " " << stringBuilder << endl;  // variable
            } else {
                string op(1, ch);
                string potentialDoubleOp = op + line[pos+1];

                if (pos+1 < line.length()) {
                    potentialDoubleOp += op + line[pos+1];
                }

                if (!potentialDoubleOp.empty() && tokenmap.contains(potentialDoubleOp)) {
                    outfile << tokenmap[potentialDoubleOp] << " " << potentialDoubleOp << endl;  // double operator
                    pos += 2;
                } else if (tokenmap.contains(op)) {
                    outfile << tokenmap[op] << " " << op << endl;  // single operator
                    pos += 1;
                } else {
                    outfile << "ERROR: Unrecognized character: " << op << endl;
                    errorFound = true;
                }
            }
        }
    }
    if (!errorFound) {
        cout << "SUCCESS READING SOURCE CODE" << endl;
    } else
        cout << "FAILURE READING SOURCE CODE" << endl;
}
