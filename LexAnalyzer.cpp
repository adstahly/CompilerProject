#include "LexAnalyzer.h"

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

        for (int i = 0; i < line.length() && !error; i++) {
            char c = line[i];
            if (isspace(c)) {continue;}

            if (isalpha(c)) {
                checkIdentifier(line,i);
            } else if (isdigit(c)) {
                checkNumber(line,i);
            } else if (c == '"') {
                checkText(infile, line,i, lineNum,error);
            } else {
                checkSymbols(line,i, error);
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

void LexAnalyzer::checkIdentifier(const string& line, int& i) {
    string buffer;
    buffer += line[i];

    while (i +1 < line.length() && (isalnum(line[i+1]) || line[i+1] == '_')) {
        i++;
        buffer += line[i];
    }
    if (tokenmap.contains(buffer)) {
        lexemes.push_back(buffer);
        tokens.emplace_back(tokenmap[buffer]);
    }else {
        lexemes.push_back(buffer);
        tokens.emplace_back("t_id");
    }
}
void LexAnalyzer::checkNumber(const string& line, int& i) {
    string buffer;
    buffer+= line[i];

    while (i+1 < line.length() && isdigit(line[i+1])) {
        i++;
        buffer += line[i];
    }
    lexemes.push_back(buffer);
    tokens.emplace_back("t_number");
}
void LexAnalyzer::checkText(istream& infile, string& line, int& i, int& lineNum, bool& error) {
    string buffer;
    bool close = false;
    while (!close && !error) {
        while (i + 1 < line.length()) {
            i++;
            if (line[i] == '"') {
                close = true;
                break;
            }
            buffer += line[i];
        }

        if (close) {
            break;
        }

        buffer += '\n';
        if (getline(infile, line)) {
            lineNum++;
            i = -1;
        } else {
            error = true;
        }
    }
    if (close) {
        lexemes.push_back(buffer);
        tokens.emplace_back("t_text");
    } else {
        lexemes.push_back(buffer);
        tokens.emplace_back("ERROR_UNCLOSED_STRING");
    }
}
void LexAnalyzer::checkSymbols(const string& line, int& i, bool& error) {
    string buffer;
    buffer += line[i];
    if ((line[i] == '>' || line[i] == '<') && i+1 < line.size() && line[i + 1] == '=' ) {
        buffer += line[i + 1];
        i++;
    }
    if (tokenmap.contains(buffer)) {
        lexemes.push_back(buffer);
        tokens.emplace_back(tokenmap[buffer]);
    }else {
        lexemes.push_back(buffer);
        tokens.emplace_back("ERROR_NO_SYMBOL_Found");
        error = true;
    }

}
