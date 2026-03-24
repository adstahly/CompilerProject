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

bool LexAnalyzer::isNumber(const char c) {
    return (c >= '0' && c <= '9');
}

bool LexAnalyzer::isAlpha(const char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool LexAnalyzer::isWhitespace(const char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

bool LexAnalyzer::isDelimiter(const char c) {
    string delimiter = "\t\n\r=,:;(){}-+*<>";
    return delimiter.find(c) != string::npos;
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
            if (isWhitespace(c)) {
                continue;
            }

            if (isAlpha(c)) {
                string buffer;
                buffer += c;
                while (i + 1 < n && (isAlpha(line[i + 1]) || isNumber(line[i + 1]))) {
                    i++;
                    buffer += line[i];
                }
                if (i + 1 < n && !isDelimiter(line[i + 1])) {
                    error = true;
                    tokens.emplace_back("ERROR: INVALID DELIMITER");
                    lexemes.push_back(buffer + line[i + 1]);
                    break;
                }

                if (auto it = tokenmap.find(buffer); it != tokenmap.end()) {
                    lexemes.push_back(it->first);
                    tokens.push_back(it->second);
                } else {
                    lexemes.push_back(buffer);
                    tokens.emplace_back("t_id");
                }
            } else if (isNumber(c)) {
                string buffer;
                buffer += c;
                while (i + 1 < n && (isNumber(line[i + 1]))) {
                    i++;
                    buffer += line[i];
                }

                if (i + 1 < n && !isDelimiter(line[i + 1])) {
                    error = true;
                    tokens.emplace_back("ERROR: INVALID DELIMITER");
                    lexemes.push_back(buffer + line[i + 1]);
                    break;
                }

                lexemes.push_back(buffer);
                tokens.emplace_back("t_number");
            } else if (c == '"') {
                string buffer;
                buffer += c;
                while (i + 1 < n && line[i + 1] != '"') {
                    i++;
                    buffer += line[i];
                }
                if (i + 1 < n && line[i + 1] == '"') {
                    i++;
                    buffer += line[i];

                    if (i + 1 < n && !isDelimiter(line[i + 1])) {
                        error = true;
                        tokens.emplace_back("ERROR: INVALID DELIMITER");
                        lexemes.push_back(buffer + line[i + 1]);
                        break;
                    }
                    lexemes.push_back(buffer);
                    tokens.emplace_back("t_text");
                } else {
                    error = true;
                    lexemes.push_back(buffer);
                    tokens.emplace_back("ERROR_UNCLOSED_STRING");
                }
            } else {
                switch (c) {
                    case '{':
                    case '}':
                    case ';':
                    case ':':
                    case '(':
                    case ')':
                    case ',':
                    case '=':
                    case '+':
                    case '-':
                    case '*': {
                        string s(1, c);
                        lexemes.push_back(s);
                        tokens.push_back(tokenmap[s]);
                        break;
                    }
                    case '<':
                    case '>': {
                        string s(1, c);

                        if (i + 1 < n && line[i + 1] == '=') {
                            i++;
                            s += line[i];
                        }

                        lexemes.push_back(s);
                        tokens.push_back(tokenmap[s]);
                        break;
                    }
                    default:
                        error = true;
                        lexemes.emplace_back(1, c);
                        tokens.emplace_back("LEXICAL_ERROR");
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
