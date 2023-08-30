#pragma once

//#include "tests/lib_tests.cpp"
#include "Column.h"
#include "DBExeptions.h"
#include "MyCoolDB.h"
#include <map>

struct openTable {
    std::vector<std::string> names;
    std::vector<Column::DataType> types;
    std::vector<bool> primary_key, foreign_key, is_null;
    std::vector<std::vector<std::string>> columns;
    uint64_t number_of_notes = 0;

    uint64_t index(const std::string& name) {
        for (uint64_t i = 0; i < names.size(); i++) {
            if (names[i] == name) {
                return i;
            }
        }
    }
};

class table {
private:
    std::string name;
    std::string table_primary_key_ = "";
    std::map<std::string, Column*> columns;
    friend class MyCoolDB;
    uint64_t number_of_notes = 0;
    friend class ColumnsProve;
public:
    table(const std::string& table_name) : name(table_name) {}
    table() = default;
    uint64_t size() {
        return columns.size();
    }
    void setName(const std::string& table_name) {
        name = table_name;
    }

    void setPrimaryKey(const std::string& value) {
        if (table_primary_key_ == "") {
            table_primary_key_ = value;
        }
    }

    std::string getPrimaryKey() {
        return table_primary_key_;
    };

    std::vector<std::string> getColumnValues(const std::string& column_name) {
        return this->columns[column_name]->getColumn();
    }

    openTable getData() {
        openTable t;
        t.number_of_notes = number_of_notes;
        for (auto& [column_name, data] : columns) {
            t.columns.emplace_back(data->getColumn());
            t.types.emplace_back(data->type());
            t.names.emplace_back(data->getName());
            t.primary_key.emplace_back(data->getPrimaryKey());
            t.foreign_key.emplace_back(data->getForeignKey());
            t.is_null.emplace_back(data->isNotNull());
        }

        return t;
    }
    /*void addColumn(const std::string& column_name, Column::DataType type, const std::vector<std::string>& column, bool primary_key, bool foreight_key) {
        if (!columns.contains(column_name))
            columns.insert({column_name, new Column(column_name, type, primary_key, foreight_key)});
        //else throw error
    }*/

    void addColumn(const std::string& column_name, Column::DataType type,  bool primary_key, bool foreight_key, bool not_null) {
        if (!columns.contains(column_name))
            columns.insert({column_name, new Column(column_name, type, primary_key, foreight_key, not_null)});
        else {
            throw DBException("This column already exists");
        }
    }

    void addColumn(const std::string& column_name, Column::DataType type, const std::vector<std::string>& ceil,  bool primary_key, bool foreight_key, bool not_null) {
        if (!columns.contains(column_name)) {
            columns.insert({column_name, new Column(column_name, type, ceil, primary_key, foreight_key, not_null)});
            number_of_notes = ceil.size();
        } else {
            throw DBException("This column already exists");
        }
    }
    void addColumn(const std::string& column_name, Column::DataType type,  bool primary_key, bool foreight_key, Column* ref, bool not_null) {
        if (!columns.contains(column_name)) {
            columns.insert({column_name, new Column(column_name, type, primary_key, foreight_key, ref, not_null)});
        } else {
            throw DBException("This column already exists");
        }
    }

    void setValue(const std::string& column_name, uint64_t index, const std::string& value) {
        if (columns.contains(column_name)) {
            columns[column_name]->setValue(index, value);
        } else {
            throw DBException("This column NOT exists");
        }
    }

    void IncNotes() {
        number_of_notes++;
    }

    uint64_t getNotes() {
        return number_of_notes;
    }

    void addRow(const std::vector<std::string>& columns_names, const std::vector<std::string>& values) {
        for (size_t i = 0; i < columns_names.size(); i++) {
            if (columns.contains(columns_names[i])) {
                columns[columns_names[i]]->newRow(values[i]);
                number_of_notes++;
            }
            // else throw ex
        }
    }
};
