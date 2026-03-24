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


// pre: 1st parameter refers to an open text file that contains source
// code in the language, 2nd parameter refers to an open empty output
// file
// post: If no error, the token and lexeme pairs for the given input
// file have been written to the output file (token lexeme).
// If there is an error, the incomplete token/lexeme pairs, as well as
// an error message have been written to the output file.
// A success or fail message has printed to the console.
void LexAnalyzer::scanFile(istream &infile, ostream &outfile) {
    string line;
    bool error = false;
    int lineNum = 0;

    while (getline(infile, line) && !error) {
        lineNum++;
        int n = line.length();

        for (int i = 0; i < n && !error; i++) {
            char c = line[i];
            if (isspace(c)) {
                continue;
            }

            if (isalpha(c) || c == '_') {
                string buffer;
                buffer += c;
                while (i + 1 < n && (isalpha(line[i + 1]) || isdigit(line[i + 1]))) {
                    i++;
                    buffer += line[i];
                }
                if (tokenmap.contains(buffer)) {
                    lexemes.push_back(buffer);
                    tokens.emplace_back(tokenmap[buffer]);
                } else {
                    lexemes.push_back(buffer);
                    tokens.emplace_back("t_id");
                }
            } else if (isdigit(c)) {
                string buffer;
                buffer += c;
                while (i + 1 < n && (isdigit(line[i + 1]))) {
                    i++;
                    buffer += line[i];
                }
                lexemes.push_back(buffer);
                tokens.emplace_back("t_number");
            } else if (c == '"') {
                string buffer;
                bool close = false;
                while (i + 1 < n) {
                    i++;
                    if (line[i] == '"') {
                        close = true;
                        break;
                    }
                    buffer += line[i];
                }
                if (close) {
                    lexemes.push_back(buffer);
                    tokens.emplace_back("t_text");
                } else {
                    error = true;
                    lexemes.push_back(buffer);
                    tokens.emplace_back("ERROR_UNCLOSED_STRING");

                }
            } else {
                string buffer;
                buffer += c;
                if ((c == '>' || c == '<') && i+1 < n && line[i + 1] == '=' ) {
                    buffer += line[i + 1];
                    i++;
                }
                if (tokenmap.contains(buffer)) {
                    lexemes.push_back(buffer);
                    tokens.emplace_back(tokenmap[buffer]);
                }else {
                    error = true;
                    lexemes.push_back(buffer);
                    tokens.emplace_back("ERROR_NO_SYMBOL_Found");
                    break;
                }
            }
        }
    }
    for (int i = 0; i < tokens.size(); i++) {
        outfile << tokens[i] << " " << lexemes[i] << endl;
    }
    if (error) {
        cout << "Scanning failed: Lexical errors encountered." << endl;
    } else {
        cout << "Scanning completed successfully." << endl;
    }
}