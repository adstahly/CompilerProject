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
                    tokens.push_back(tokenmap[buffer]);
                } else {
                    lexemes.push_back(buffer);
                    tokens.push_back("t_id");
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
        cout << tokens[i] << " " << lexemes[i] << endl;
    }
    if (error) {
        cout << "Scanning failed: Lexical errors encountered." << endl;
    } else {
        cout << "Scanning completed successfully." << endl;
    }
}