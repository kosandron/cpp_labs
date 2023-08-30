#include "splitFunctions.h"

std::vector<std::string> SplitString(const std::string& str, char delimiter) {
    std::vector<std::string> words;
    std::string cur;

    for (size_t i = 0; i < str.size(); i++) {
        if (std::isspace(str[i])) {
            if (cur != "") {
                words.push_back(cur);
                cur = "";
            }
        } else {
            if (str[i] == '=') {
                if (cur != "")
                    words.push_back(cur);
                words.push_back("=");
                cur = "";
                continue;
            }
            if (!(str[i] == '\'' or str[i] == '\"'))
                cur += str[i];
        }
    }
    if (cur != "") {
        words.push_back(cur);
        cur = "";
    }

    return words;
}

std::vector<std::string> SplitEqual(const std::vector<std::string>& strings) {
    std::vector<std::string> words;
    std::string word = "";
    //std::cout << strings.size() << '\n';
    for (size_t i = 0; i < strings.size(); i++) {
        for (size_t j = 0; j < strings[i].size(); j++) {
            if (strings[i][j] == ',' or strings[i][j] == '=') {
                //std::cout << word << '\n';
                if (word != "" and word != " ") {
                    //std::cout << word << '\n';
                    words.emplace_back(word);
                    word = "";
                }
            } else {
                word += strings[i][j];
            }
        }
        /*if (word != "")
            word += ' ';*/
    }
    words.emplace_back(word);

    return words;
}

std::vector<std::string> SplitScopes(const std::vector<std::string>& strings) {
    std::vector<std::string> words;
    std::string word = "";
    for (size_t i = 0; i < strings.size(); i++) {
        for (size_t j = 0; j < strings[i].size(); j++) {
            if (strings[i][j] == '(' or strings[i][j] == ')' or strings[i][j] == ';') {
                continue;
            }
            if (strings[i][j] == ',') {
                words.emplace_back(word);
                word = "";
            } else {
                word += strings[i][j];
            }
        }

        if (word != "") {
            if (word[0] == '\"' or word[0] == '\'') {
                if (word[word.size() - 1] == '\"' or word[word.size() - 1] == '\'') {
                    words.emplace_back(word);
                    word = "";
                } else {
                    word += ' ';
                    continue;
                }
            } else {
                words.emplace_back(word);
                word = "";
            }

        }
    }

    return words;
}

void join_parse(const std::vector<std::string>& words, size_t index, std::string& second_table, std::string& column1, std::string& column2) {
    second_table = words[index];
    index += 2;
    std::string temp = words[index].substr(0, words[index].find('.'));
    //std::cout << temp << '\n';
    if (temp == second_table) {
        temp = "";
        for (size_t j = words[index].find('.') + 1; j < words[index].size(); j++) {
            if (words[index][j] != '=') {
                column2 += words[index][j];
            }
        }
        //std::cout << column2 << '\n';
        index++;
        if (words[index] == "=") {
            index++;
        }
        column1 = words[index].substr(words[index].find('.') + 1, words[index].find(';') - words[index].find('.') - 1);

    } else {
        temp = "";
        for (size_t j = words[index].find('.') + 1; j < words[index].size(); j++) {
            if (words[index][j] != '=') {
                column1 += words[index][j];
            }
        }
        //std::cout << column1 << '\n';
        index++;
        if (words[index] == "=") {
            index++;
        }
        column2 = words[index].substr(words[index].find('.') + 1, words[index].find(';') - words[index].find('.') - 1);
    }

    //std::cout << column2 << '\n';
}
