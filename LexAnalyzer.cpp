
#include "LexAnalyzer.h"

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
void LexAnalyzer::scanFile(istream& infile, ostream& outfile) {
    char ch;
    bool errorFound = false;

    while (infile.get(ch) && !errorFound) {
        if (isspace(ch)) continue;  // ignore whitespace

        if (ch == '"') {  // must scroll through and find second "
            string stringBuilder = "";
            while (infile.get(ch) && ch != '"') {
                stringBuilder += ch;
                if (infile.peek() == infile.eof()) {
                    outfile << stringBuilder << "ERROR: No closing quote for text" << endl;  // error msg
                    errorFound = true;
                }
            }
            outfile << "t_text" << ":" << stringBuilder << endl;
        } else if (isalnum(ch)) {
            string numBuilder = "";
            while (isalnum(infile.peek())) {
                char next;
                infile.get(next);
                numBuilder += next;
            }
            outfile << numBuilder << endl;
        }


    }
}
