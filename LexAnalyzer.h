//
// Created by Alex Torres on 3/18/26.
//


#include "LexAnalyzer.h"

// pre: parameter refers to open data file consisting of token and
// lexeme pairs i.e.  t_and and
// Each pair appears on its own input line.
// post: tokenmap has been populated - key: lexeme, value: token

LexAnalyzer::LexAnalyzer(istream& infile) {
    string line;
    while (getline(infile, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        istringstream iss(line);
        string token, lexeme;

        if (!(iss >> token >> lexeme)) continue;

        tokens.push_back(token);
        lexemes.push_back(lexeme);
        tokenmap[lexeme] = token;
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
void LexAnalyzer::scanFile(istream& infile, ostream& outfile) {
    string word;
    char ch;
    while (infile.get(ch)) {
        if (isalnum(ch)) {
            word += ch;
        } else if (isspace(ch)) {
            if (!word.empty()) {
                if (tokenmap.find(word) != tokenmap.end()) {
                    outfile << tokenmap[word] << " " << word << endl;
                    word = "";
                } else if (all_of(word.begin(), word.end(), ::isdigit)) {
                    outfile << "t_number " << word << endl;
                    word = "";
                } else if (all_of(word.begin(), word.end(), ::isalnum)) {
                    outfile << "t_id " << word << endl;
                    word = "";
                } else {
                    outfile << "could not recognize '" << word << "'" << endl;
                    cout << "could not recognize '" << word << "'" << endl;
                    cout << "Lexical Analysis Failed" << endl;
                    return;
                }
            }
        } else {
            if (!word.empty()) {
                if (tokenmap.find(word) != tokenmap.end()) {
                    outfile << tokenmap[word] << " " << word << endl;
                    word = "";
                } else if (all_of(word.begin(), word.end(), ::isdigit)) {
                    outfile << "t_number " << word << endl;
                    word = "";
                } else if (all_of(word.begin(), word.end(), ::isalnum)) {
                    outfile << "t_id " << word << endl;
                    word = "";
                } else {
                    outfile << "could not recognize '" << word << "'" << endl;
                    cout << "could not recognize '" << word << "'" << endl;
                    cout << "Lexical Analysis Failed" << endl;
                    return;
                }
            }
            if (ch == '"') {
                string str;
                while (infile.get(ch) && ch != '"') {
                    str += ch;
                }
                outfile << "t_text " << str << endl;
            } else {
                string sym(1, ch);
                if (infile.peek() == '=') {
                    infile.get(ch);
                    sym += ch;
                }
                if (tokenmap.find(sym) != tokenmap.end()) {
                    outfile << tokenmap[sym] << " " << sym << endl;
                } else {
                    outfile << "could not recognize '" << sym << "'" << endl;
                    cout << "could not recognize '" << sym << "'" << endl;
                    cout << "Lexical Analysis Failed" << endl;
                    return;
                }
            }
        }
    }
    if (!word.empty()) {
        if (tokenmap.find(word) != tokenmap.end()) {
            outfile << tokenmap[word] << " " << word << endl;
        } else if (all_of(word.begin(), word.end(), ::isdigit)) {
            outfile << "t_number " << word << endl;
        } else if (all_of(word.begin(), word.end(), ::isalnum)) {
            outfile << "t_id " << word << endl;
        } else {
            outfile << "could not recognize '" << word << "'" << endl;
            cout << "could not recognize '" << word << "'" << endl;
            cout << "Lexical Analysis Failed" << endl;
            return;
        }
    }
    cout << "Lexical Analysis Complete" << endl;
}