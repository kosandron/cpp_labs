#pragma once

#include <filesystem>
#include <istream>


namespace omfl {

    enum ReadType {
        void_type,
        key_type,
        value_type,
        string_type,
        array_type,
        section_type,
        comment
    };

    enum LineType {
        not_defined,
        key_value,
        section,
        array
    };

    class Variable {
    private:
        LineType status;
        Variable* parent;
        bool read_status;
    public:
        std::string name = "";
        std::string value = "";
        std::vector<Variable> array;

        bool valid() const;
        void printStructure(uint16_t k = 0) const;
        void wrongFormat();
        void changeStatus(const omfl::LineType& new_status);
        const Variable& Get(const std::string& param, int16_t pos = 0) const;
        Variable& ChangeSection(const std::string& param, int16_t pos = 0);
        Variable* GoUp() const;
        const Variable& operator[](uint32_t index) const;

        bool IsInt() const;
        bool IsFloat() const;
        bool IsBool() const;
        bool IsString() const;
        bool IsArray() const;
        bool IsKey() const;

        int32_t AsInt() const;
        float AsFloat() const;
        bool AsBool() const;
        std::string AsString() const;

        int32_t AsIntOrDefault(int32_t param) const;
        float AsFloatOrDefault(float param) const;
        bool AsBoolOrDefault(bool param) const;
        std::string AsStringOrDefault(const std::string& param) const;

        Variable(Variable* parent);
        Variable();
        //Variable(const std::string& key, const std::string& p);
    };

    Variable parse(const std::filesystem::path& path);
    Variable parse(const std::string& str);
    bool IsSection(const std::string& name);
}// namespace