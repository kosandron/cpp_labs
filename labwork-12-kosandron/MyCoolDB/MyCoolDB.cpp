#include "MyCoolDB.h"

uint64_t MyCoolDB::size() {
    return tables.size();
}

table MyCoolDB::request(const std::string& request_line) {
    return parse(SplitString(request_line));
}

void MyCoolDB::load(std::filesystem::path path = "cash.txt") {
    std::ifstream f(path);
    //f << 5;
    uint64_t n;
    f >> n;
    //std::cout << n;
    //std::cout << n;
    table temp;
    for (size_t i = 0; i < n; i++) {
        size_t m;
        std::string table_name;
        f >> table_name >> m;
        std::string column_name, datatype;
        bool primary_key, foreign_key, not_null;
        Column::DataType type;
        //std::vector<std::string> column;
        for (size_t j = 0; j < m; j++) {
            f >> column_name >> primary_key >> foreign_key >> not_null >> datatype;
            if (datatype == "INT") {
                type = Column::Int;
            } else if (datatype == "BOOL") {
                type = Column::Bool;
            } else if (datatype == "FLOAT") {
                type = Column::Float;
            } else if (datatype == "DOUBLE") {
                type = Column::Double;
            } else if (datatype == "VARCHAR") {
                type = Column::Varchar;
            }
            //std::cout << column_name << primary_key << foreign_key << not_null << datatype << '\n';
            size_t size;
            f >> size;
            std::vector<std::string> column(size);
            for (size_t z = 0; z < size; z++) {
                f >> column[z];
            }
            temp.addColumn(column_name, type, column, primary_key, foreign_key, not_null);
        }
        this->addTable(table_name, temp);
    }
    f.close();
}

void MyCoolDB::save(std::filesystem::path path = "cash.txt") {
    std::ofstream f(path);
    uint64_t n = tables.size();
    f << n << '\n';
    for (auto& [table_name, a_table] : tables) {
        f << table_name << ' ' << a_table.size() << '\n';
        for (auto& [column_name, a_column] : a_table.columns) {
            f << column_name << ' ' << a_column->getPrimaryKey() << ' ' << a_column->getForeignKey() << ' ' << a_column->isNotNull() << ' ';
            Column::DataType type = a_column->type();
            if (type == Column::Int) {
                f << "INT\n";
            } else if (type == Column::Bool) {
                f << "BOOL\n";
            } else if (type == Column::Float) {
                f << "FLOAT\n";
            } else if (type == Column::Double) {
                f << "DOUBLE\n";
            } else if (type == Column::Varchar) {
                f << "VARCHAR\n";
            }
            std::vector<std::string> column = a_column->getColumn();
            f << column.size() << " ";
            for (auto& i : column) {
                f << i << ' ';
            }
        }
        f << '\n';
    }
    f.close();
}
/*
template<typename T>
bool IsMore(const T& lhs, const std::vector<std::string>) {

}*/

std::set<uint64_t> MyCoolDB::conclusion(const std::string& table_name, const std::string& lhs, const std::string& rhs, const std::string& operation) {
    std::set<uint64_t> answer;
    if (tables[table_name].columns.contains(lhs) and tables[table_name].columns.contains(rhs)) {
        std::vector<std::string> tmp1 = tables[table_name].columns[lhs]->getColumn();
        std::vector<std::string> tmp2 = tables[table_name].columns[rhs]->getColumn();
        for (size_t i = 0; i < tmp1.size(); i++) {
            if (tmp1[i] == tmp2[i]) {
                answer.insert(i);
            }
        }
    } else if (tables[table_name].columns.contains(lhs)) {
        //std::cout << "yeah";
        Column::DataType type = tables[table_name].columns[lhs]->type();
        if (type == Column::DataType::Int) {
            //std::cout << "ohh";
            int temp = std::stoi(rhs);
            uint64_t pos = 0;
            if (operation == ">")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stoi(ceil) > temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == ">=")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stoi(ceil) >= temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<=")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stoi(ceil) <= temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stoi(ceil) < temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "=")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stoi(ceil) == temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<>")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stoi(ceil) != temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }

        } else if (type == Column::DataType::Float) {
            float temp = std::stof(rhs);
            uint64_t pos = 0;
            if (operation == ">")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stof(ceil) > temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == ">=")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stof(ceil) >= temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<=")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stof(ceil) <= temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stof(ceil) < temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "=")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stof(ceil) == temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<>")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stof(ceil) != temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
        } else if (type == Column::DataType::Double) {
            double temp = std::stod(rhs);
            uint64_t pos = 0;
            if (operation == ">")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stod(ceil) > temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == ">=")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stod(ceil) >= temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<=")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stod(ceil) <= temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stod(ceil) < temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "=")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stod(ceil) == temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<>")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (std::stod(ceil) != temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
        } else if (type == Column::DataType::Bool) {
            bool temp = (rhs == "true" or rhs == "TRUE");
            uint64_t pos = 0;
            if (operation == "=")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if ((ceil == "TRUE" or ceil == "true") == temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<>")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if ((ceil == "TRUE" or ceil == "true") != temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
        } else {
            uint64_t pos = 0;
            if (operation == "=")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (ceil == rhs) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<>")
                for (auto& ceil : tables[table_name].columns[lhs]->getColumn()) {
                    if (ceil != rhs) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
        }
    } else if (tables[table_name].columns.contains(rhs)) {
        Column::DataType type = tables[table_name].columns[rhs]->type();
        if (type == Column::DataType::Int) {
            //std::cout << "ohh";
            int temp = std::stoi(lhs);
            uint64_t pos = 0;
            if (operation == ">")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (temp > std::stoi(ceil)) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == ">=")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (temp >= std::stoi(ceil)) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<=")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (temp <= std::stoi(ceil)) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (temp < std::stoi(ceil)) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "=")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (std::stoi(ceil) == temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<>")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (std::stoi(ceil) != temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }

        } else if (type == Column::DataType::Float) {
            float temp = std::stof(lhs);
            uint64_t pos = 0;
            if (operation == ">")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (temp > std::stof(ceil)) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == ">=")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (temp >= std::stof(ceil)) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<=")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (temp <= std::stof(ceil)) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (temp < std::stof(ceil)) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "=")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (std::stof(ceil) == temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<>")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (std::stof(ceil) != temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
        } else if (type == Column::DataType::Double) {
            double temp = std::stod(lhs);
            uint64_t pos = 0;
            if (operation == ">")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (temp > std::stod(ceil)) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == ">=")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (temp >= std::stod(ceil)) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<=")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (temp <= std::stod(ceil)) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (temp < std::stod(ceil)) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "=")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (std::stod(ceil) == temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<>")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (std::stod(ceil) != temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
        } else if (type == Column::DataType::Bool) {
            bool temp = (lhs == "true" or lhs == "TRUE");
            uint64_t pos = 0;
            if (operation == "=")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if ((ceil == "TRUE" or ceil == "true") == temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<>")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if ((ceil == "TRUE" or ceil == "true") != temp) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
        } else {
            uint64_t pos = 0;
            if (operation == "=")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (ceil == lhs) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
            else if (operation == "<>")
                for (auto& ceil : tables[table_name].columns[rhs]->getColumn()) {
                    if (ceil != lhs) {
                        answer.insert(pos);
                    }
                    ++pos;
                }
        }
    } else {
        throw DBException("Error in name of column!");
    }
    return answer;
}

void notFunction(std::set<uint64_t>& ans, uint64_t number_of_notes) {
    std::set<uint64_t> tmp;
    for (size_t i = 0; i < number_of_notes; i++) {
        if (!(ans.contains(i))) {
            tmp.insert(i);
        }
    }
    ans = tmp;
}

std::set<uint64_t> MyCoolDB::where(const std::string& table_name, const std::vector<std::string> words, uint32_t index) {
    std::vector<std::string> operators, lhs, rhs, and_or_vector;
    //std::vector<char> operators;
    std::string tmp1, tmp2, oper = "";
    for (size_t i = index; i < words.size(); i++) {
        //std::cout << words[i] << " ";
        if (words[i] == "AND" or words[i] == "OR" or words[i] == "NOT") {
            and_or_vector.emplace_back(words[i]);
            if (oper != "") {
                lhs.emplace_back(tmp1);
                rhs.emplace_back(tmp2);
                operators.emplace_back(oper);
                tmp1 = "";
                tmp2 = "";
                oper = "";
            }
            continue;
        } else if (words[i][0] != '>' and words[i][0] != '<' and words[i][0] != '=') {
            if (oper == "" and tmp1 != "") {
                tmp1 += ' ';
            } else if (oper != "" and tmp2!= "") {
                tmp2 += ' ';
                //std::cout << "Problemword" << words[i];
            }
        }
        for (size_t j = 0; j < words[i].size(); j++) {
            if (words[i][j] == '=' or words[i][j] == '>' or words[i][j] == '<') {
                oper += words[i][j];
            } else if (oper == "") {
                tmp1 += words[i][j];
            } else {
                tmp2 += words[i][j];
            }
        }
    }
    if (oper != "") {
        lhs.emplace_back(tmp1);
        rhs.emplace_back(tmp2);
        operators.emplace_back(oper);
    }
    rhs[rhs.size() - 1] = rhs[rhs.size() - 1].substr(0, rhs[rhs.size() - 1].size() - 1);
    /*for (auto& t : operators) {
        std::cout << t << " ";
    }
    std::cout << '\n';
    for (auto& t : lhs) {
        std::cout << t << " ";
    }
    std::cout << '\n';
    for (auto& t : rhs) {
        std::cout << t << " ";
    }
    std::cout << '\n';
    for (auto& t : and_or_vector) {
        std::cout << t << " ";
    }
    std::cout << '\n';*/

    std::vector<std::set<uint64_t>> results;
    for (size_t i = 0; i < lhs.size(); i++) {
        results.emplace_back(conclusion(table_name, lhs[i], rhs[i], operators[i]));
    }

    size_t temp = 0, temp2 = 0;
    std::set<uint64_t> answer;

    std::set<uint64_t> second_set;
    while(temp != and_or_vector.size()) {
        if (and_or_vector[temp] == "NOT") {
            notFunction(results[temp2], tables[table_name].number_of_notes);
            temp++;
        } else {
            temp++;
            temp2++;
        }
    }
    std::set<uint64_t> first_set = results[0];
    temp = 1;
    for (auto& operation : and_or_vector) {
        if (operation != "NOT") {
            if (operation == "AND") {
                results[temp].merge(first_set);
                temp++;
            } else if (operation == "OR") {
                results[temp].merge(first_set);
                first_set = results[temp];
                temp++;
            }
        }
    }

    return first_set;
}

void MyCoolDB::addTable(const std::string& table_name, const table& new_table) {
    if (!tables.contains(table_name))
        tables.insert({table_name, new_table});
}

void MyCoolDB::drop(const std::string& table_name) {
    for (auto it = tables.begin(); it != tables.end(); it++) {
        if (it->first == table_name) {
            for (auto& [_, column] : tables[table_name].columns) {
                if (column->getForeignKey()) {
                    throw DBException("Trying to delete table with foreign key!");
                    return;
                }
            }
            it = tables.erase(it);
            return;
        }
    }
}

table MyCoolDB::parse(const std::vector<std::string>& words) {
    if (words[0] == "CREATE") {
        uint16_t i = 2;
        std::string table_name = words[i].substr(0, words[i].find('('));
        //std::cout << table_name << ' ' << i << '\n';
        for (auto& [names, _] : tables) {
            if (names == table_name) {
                throw DBException("This table already exists!");
                return table();
            }
        }
        i++;
        if (words[i] == "(") {
            i++;
        }
        table temp(table_name);
        std::string column_name, datatype;
        Column::DataType data;
        bool not_null, primary_key, foreign_key;
        Column* reference = nullptr;
        while (i < words.size() - 1) {
            not_null = false;
            primary_key = false;
            foreign_key = false;

            column_name = words[i];
            i++;
            datatype = words[i];
            if (datatype == "INT" or datatype == "INT,") {
                data = Column::DataType::Int;
            } else if (datatype == "BOOL" or datatype == "BOOL,") {
                data = Column::DataType::Bool;
            } else if (datatype == "VARCHAR" or datatype == "VARCHAR,") {
                data = Column::DataType::Varchar;
            } else if (datatype == "FLOAT" or datatype == "FLOAT,") {
                data = Column::DataType::Float;
            } else if (datatype == "DOUBLE" or datatype == "DOUBLE,") {
                data = Column::DataType::Double;
            }
            i++;
            if (words[i] == "PRIMARY") {
                i += 2;
                primary_key = true;
                temp.setPrimaryKey(column_name);
            } else if (words[i] == "FOREIGN") {
                i += 3;
                std::string reference_table = words[i].substr(0, words[i].find('('));
                std::string reference_column = words[i].substr(words[i].find('(') + 1, words[i].find(')') - words[i].find('('));
                reference = tables[reference_table].columns[reference_column];
                i++;
                foreign_key = true;
            } else if (words[i] == "NOT") {
                i += 2;
                not_null = true;
            } else {
                temp.addColumn(column_name, data, primary_key, foreign_key, reference, not_null);
                reference = nullptr;
                //std::cout << column_name <<  " "<< datatype <<" "<< primary_key<<" "<< foreign_key <<" "<< not_null << '\n' << '\n';
                continue;
            }

            if (words[i] == "PRIMARY") {
                i += 2;
                primary_key = true;
                temp.setPrimaryKey(column_name);
            } else if (words[i] == "FOREIGN") {
                i += 3;
                std::string reference_table = words[i].substr(0, words[i].find('('));
                std::string reference_column = words[i].substr(words[i].find('(') + 1, words[i].find(')') - words[i].find('('));
                reference = tables[reference_table].columns[reference_column];
                i++;
                foreign_key = true;
            } else if (words[i] == "NOT") {
                i += 2;
                not_null = true;
            } else {
                temp.addColumn(column_name, data, primary_key, foreign_key, reference, not_null);
                reference = nullptr;
                //std::cout << column_name << " " <<  datatype <<" "<< primary_key<<" "<< foreign_key <<" "<< not_null << '\n';
                continue;
            }
            temp.addColumn(column_name, data, primary_key, foreign_key, reference,  not_null);
            reference = nullptr;
            //std::cout << column_name << " "  << datatype <<" "<< primary_key<<" "<< foreign_key <<" "<< not_null << '\n';

        }
        //ADD TABLE METHOD
        this->addTable(table_name, temp);

        return temp;
    } else if (words[0] == "DROP") {
        drop(words[2].substr(0, words[2].find(';')));
        return table();
    } else if (words[0] == "DELETE") {
        std::string table_name = words[2];
        std::set<uint64_t> w;
        try {
            w = where(table_name, words, 4);
        } catch (std::exception& ex) {
            std::cout << "Error while parsing!\n";
        }

        for (auto& [column_name, column] : tables[table_name].columns) {
            std::vector<std::string> answer;
            std::vector<std::string> ceils = column->getColumn();
            for (size_t i = 0; i < ceils.size(); i++) {
                if (!(w.contains(i))) {
                    answer.emplace_back(ceils[i]);
                }
            }
            tables[table_name].columns[column_name]->setColumn(answer);
        }
        tables[table_name].number_of_notes -= w.size();

        return tables[table_name];
    } else if (words[0] == "UPDATE") {
            std::string table_name = words[1];
            size_t i = 3;
            std::vector<std::string> values;
            //std::vector<std::string> words_copy = SplitEqual(words);
            while (words[i] != "WHERE") {
                values.emplace_back(words[i]);
                i++;
            }
            values = SplitEqual(values);
            /*std::cout << "\nValues:\n";
            for (auto& r : values) {
                std::cout << r << '\n';
            }*/
            //parse where!
            std::set<uint64_t> w = where(table_name, words, i + 1);
            // update function
            for(size_t z = 0; z < values.size(); z += 2) {
                for (auto& row : w) {
                    tables[table_name].columns[values[z]]->setValue(row, values[z + 1]);
                }
            }

            return tables[table_name];
    } else if (words[0] == "INSERT") {
        std::vector<std::string> request_words;
        request_words = SplitScopes(words);
        std::string request_table = request_words[2];
        std::vector<std::pair<std::string, std::string>> values;
        size_t i = 3;
        while (request_words[i] != "VALUES") {
            values.emplace_back(std::make_pair(request_words[i], ""));
            i++;
        }
        i++;
        while (i < request_words.size()) {
            values[i - 4 - values.size()].second = request_words[i];
            i++;
        }

        for (size_t h = 0; h < values.size(); h++) {
            if (values[h].first == tables[request_table].getPrimaryKey()) {
                if (values[h].first == "NULL" or values[h].first == "null") {
                    throw DBException("Primary key is equal NULL!");
                    return tables[request_table];
                }
                for (auto& t : tables[request_table].getColumnValues(tables[request_table].getPrimaryKey())) {
                    if (t == values[h].second) {
                        throw DBException("New note with old primary key value!");
                        return tables[request_table];
                    }
                }
            }
        }

        for (auto [column_name, column] : tables[request_table].columns) {
            for (size_t j = 0; j < values.size(); j++) {
                if (column_name == values[j].first) {
                    if (tables[request_table].columns[column_name]->isNotNull() and (values[j].second == "NULL" or values[j].second == "null")) {
                        throw DBException("Not null value is null!");
                        return tables[request_table];
                    }
                }
            }
        }

        for (auto [column_name, column] : tables[request_table].columns) {
            for (size_t j = 0; j < values.size(); j++) {
                if (column_name == values[j].first) {
                    column->newRow(values[j].second);
                    break;
                }
            }
        }
        tables[request_table].IncNotes();

        return tables[request_table];
    } else if (words[0] == "SELECT") {
        size_t i = 1;
        std::vector<std::string> columns;
        while (words[i] != "FROM") {
            columns.emplace_back(words[i].substr(0, words[i].find(',')));
            i++;
        }

        i++;
        std::string table_name = words[i];
        if (columns.size() == 1 and columns[0] == "*") {
            columns.clear();
            for(auto& [column_name, _] : tables[table_name].columns) {
                columns.emplace_back(column_name);
            }
        }
        i++;
        if (words[i] == "WHERE") {
            std::set<uint64_t> result = where(table_name, words, i + 1);
            table t;
            for (auto& column : columns) {
                std::vector<std::string> values;
                for (auto& index : result) {
                    values.emplace_back(tables[table_name].columns[column]->getString(index));
                }
                t.addColumn(column, tables[table_name].columns[column]->type(), values, tables[table_name].columns[column]->getPrimaryKey(), tables[table_name].columns[column]->getForeignKey(), tables[table_name].columns[column]->isNotNull());
            }
            return t;
        } else if (words[i] == "INNER" && words[i + 1] == "JOIN") {
            //std::cout << "trg\n";
            i += 2;
            std::string second_table, column1, column2;
            join_parse(words, i, second_table, column1, column2);
            std::vector<std::pair<uint64_t, uint64_t>> are_same;
            uint64_t tmp1 = 0, tmp2 = 0;
            //std::cout << table_name << " " << second_table << " "  << column1<< " " << column2 << '\n';
            for (auto& j : tables[table_name].columns[column1]->getColumn()) {
                tmp2 = 0;
                for (auto& z : tables[second_table].columns[column2]->getColumn()) {
                    if (j == z) {
                        are_same.emplace_back(std::make_pair(tmp1, tmp2));
                        //std::cout << tmp1 << " " << tmp2 << '\n';
                    }
                    tmp2++;
                }
                tmp1++;
            }

            table temp;
            for (size_t j = 0; j < columns.size(); j++) {
                std::string table1, column_of_table1;
                table1 = columns[j].substr(0, columns[j].find('.'));
                column_of_table1 = columns[j].substr(columns[j].find('.') + 1, columns[j].find(',') - columns[j].find('.') - 2);
                //std::cout << table1 << " " << column_of_table1 << '\n';
                std::vector<std::string> values;
                if (table1 == table_name) {
                    for (size_t z = 0; z < are_same.size(); z++) {
                        values.emplace_back(tables[table1].columns[column_of_table1]->getString(are_same[z].first));
                    }
                } else {
                    for (size_t z = 0; z < are_same.size(); z++) {
                        values.emplace_back(tables[table1].columns[column_of_table1]->getString(are_same[z].second));
                    }
                }
                temp.addColumn(column_of_table1, tables[table1].columns[column_of_table1]->type(), values, tables[table1].columns[column_of_table1]->getPrimaryKey(), tables[table1].columns[column_of_table1]->getForeignKey(), tables[table1].columns[column_of_table1]->isNotNull());
            }
            //table temp;
            return temp;
        } else if (words[i] == "LEFT" && words[i + 1] == "JOIN") {
            i += 2;
            std::string second_table, column1, column2;
            join_parse(words, i, second_table, column1, column2);

            std::vector<std::pair<uint64_t, uint64_t>> are_same;
            uint64_t tmp1 = 0, tmp2 = 0;
            bool flag;
            for (auto& j : tables[table_name].columns[column1]->getColumn()) {
                tmp2 = 0;
                flag = false;
                for (auto& z : tables[second_table].columns[column2]->getColumn()) {
                    //std::cout << j <<" " << z << '\n';
                    if (j == z) {
                        are_same.emplace_back(std::make_pair(tmp1, tmp2));
                        flag = true;
                    }
                    tmp2++;
                }
                if (!flag) {
                    are_same.emplace_back(std::make_pair(tmp1, -1));
                }
                tmp1++;
            }
            //std::cout << are_same.size() << '\n';

            table temp;
            for (size_t j = 0; j < columns.size(); j++) {
                std::string table1, column_of_table1;
                table1 = columns[j].substr(0, columns[j].find('.'));
                column_of_table1 = columns[j].substr(columns[j].find('.') + 1, columns[j].find(',') - columns[j].find('.') - 2);
                std::vector<std::string> values;
                if (table1 == table_name) {
                    for (size_t z = 0; z < are_same.size(); z++) {
                        values.emplace_back(tables[table1].columns[column_of_table1]->getString(are_same[z].first));
                    }
                } else {
                    for (size_t z = 0; z < are_same.size(); z++) {
                        if (are_same[z].second != -1)
                            values.emplace_back(tables[table1].columns[column_of_table1]->getString(are_same[z].second));
                        else
                            values.emplace_back("NULL");
                    }
                }
                temp.addColumn(column_of_table1, tables[table1].columns[column_of_table1]->type(), values, tables[table1].columns[column_of_table1]->getPrimaryKey(), tables[table1].columns[column_of_table1]->getForeignKey(), tables[table1].columns[column_of_table1]->isNotNull());
            }

            return temp;
        } else if (words[i] == "RIGHT" && words[i + 1] == "JOIN") {
            i += 2;
            std::string second_table, column1, column2;
            join_parse(words, i, second_table, column1, column2);

            std::vector<std::pair<uint64_t, uint64_t>> are_same;
            uint64_t tmp1 = 0, tmp2 = 0;
            bool flag;
            for (auto& j : tables[second_table].columns[column2]->getColumn()) {
                tmp2 = 0;
                flag = false;
                for (auto& z : tables[table_name].columns[column1]->getColumn()) {
                    if (j == z) {
                        are_same.emplace_back(std::make_pair(tmp1, tmp2));
                        flag = true;
                    }
                    tmp2++;
                }
                if (!flag) {
                    are_same.emplace_back(std::make_pair(tmp1, -1));
                }
                tmp1++;
            }

            table temp;
            for (size_t j = 0; j < columns.size(); j++) {
                std::string table1, column_of_table1;
                table1 = columns[j].substr(0, columns[j].find('.'));
                column_of_table1 = columns[j].substr(columns[j].find('.') + 1, columns[j].find(',') - columns[j].find('.') - 2);
                std::vector<std::string> values;
                if (table1 == second_table) {
                    for (size_t z = 0; z < are_same.size(); z++) {
                        values.emplace_back(tables[table1].columns[column_of_table1]->getString(are_same[z].first));
                    }
                } else {
                    for (size_t z = 0; z < are_same.size(); z++) {
                        if (are_same[z].second != -1)
                            values.emplace_back(tables[table1].columns[column_of_table1]->getString(are_same[z].second));
                        else
                            values.emplace_back("NULL");
                    }
                }
                temp.addColumn(column_of_table1, tables[table1].columns[column_of_table1]->type(), values, tables[table1].columns[column_of_table1]->getPrimaryKey(), tables[table1].columns[column_of_table1]->getForeignKey(), tables[table1].columns[column_of_table1]->isNotNull());
            }

            return temp;
        }
    }
}
