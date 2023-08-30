#include "archivator.h"
#include <filesystem>
const uint32_t kHeaderFileSize = 13;
const uint32_t kHeaderNameLength = 4;
const uint32_t kHeaderControlBitsCount = 3;




uint64_t gcd (uint64_t a, uint64_t b) {
    return b ? gcd(b, a % b) : a;
}

uint64_t lcm (uint64_t a, uint64_t b) {
    return a / gcd (a, b) * b;
}

void readHeader(uint64_t& file_size, std::string& file_name, uint64_t& name_size,  uint8_t& control_bytes_count, std::ifstream& arch) {
    std::string to_be_decoded = "";
    uint64_t mnogitel = 1;
    uint8_t number;
    file_size = 0;
    for (size_t i = 0; i < kHeaderFileSize * 4; i++) {
        char ch;
        arch.get(ch);
        to_be_decoded += ch;
        if ((i + 1) % 4 == 0) {
            number = atoi(decoder(to_be_decoded, 2).c_str());
            file_size += number * mnogitel;
            mnogitel = mnogitel * 10;
            to_be_decoded = "";
        }
    }

    to_be_decoded = "";
    name_size = 0;
    mnogitel = 1;
    for (size_t i = 0; i < kHeaderNameLength * 4; i++) {
        char ch;
        arch.get(ch);
        to_be_decoded += ch;
        if ((i + 1) % 4 == 0) {
            number = atoi(decoder(to_be_decoded, 2).c_str());
            name_size += number * mnogitel;
            mnogitel = mnogitel * 10;
            to_be_decoded = "";
        }
    }

    file_name = "";
    for (size_t i = 0; i < name_size; i++) {
        char ch;
        arch.get(ch);
        to_be_decoded += ch;
        if ((i + 1) % 4 == 0) {
            file_name += (decoder(to_be_decoded, 2)).c_str();
            to_be_decoded = "";
        }
    }

    mnogitel = 1;
    control_bytes_count = 0;
    for (size_t i = 0; i < kHeaderControlBitsCount * 4; i++) {
        char ch;
        arch.get(ch);
        to_be_decoded += ch;
        if ((i + 1) % 4 == 0) {
            number = atoi((decoder(to_be_decoded, 2)).c_str());
            control_bytes_count += number * mnogitel;
            mnogitel = mnogitel * 10;
            to_be_decoded = "";
        }

    }
}

void createArchive(const std::string& archive_name, const std::vector<std::string>& add_files, uint8_t control_bytes_count) {
    uint64_t infobites_count = (1 << control_bytes_count) - control_bytes_count - 1;

    std::ofstream arch(archive_name, std::ios::binary);

    uint64_t pack_size = lcm(infobites_count, 8) / 8;
    for (size_t i = 0; i < add_files.size(); i++)  {
        std::ifstream readfile(add_files[i], std::ios::binary);
        readfile.seekg(0, std::ios_base::end);
        uint64_t file_size = readfile.tellg();
        readfile.seekg(0, std::ios_base::beg);

        uint64_t new_name_size = add_files[i].size() * 4;

        // размер обычного файла в обратном порядке
        uint64_t file_size_copy = file_size;
        for (size_t j = 0; j < kHeaderFileSize; j++) {
            uint8_t razrad = file_size_copy % 10;
            converter(std::to_string(razrad), 1, 2, arch);
            file_size_copy /= 10;
        }

        //  длина кодированного имени в обратном порядке
        for (size_t j = 0; j < kHeaderNameLength; j++) {
            uint8_t razrad = new_name_size % 10;
            converter(std::to_string(razrad), 1, 2, arch);
            new_name_size /= 10;
        }

        std::string to_be_coded = "";
        for (size_t j = 0; j < add_files[i].size(); j++) {
            to_be_coded += add_files[i][j];
            converter(to_be_coded, 1, 2, arch); //  записываем кодированное имя файла
            to_be_coded = "";
        }
        to_be_coded = "";

        uint8_t control_bites_count_copy = control_bytes_count; // < 256 - 3 symbols
        for (size_t j = 0; j < kHeaderControlBitsCount; j++) {
            uint8_t razrad = control_bites_count_copy % 10;
            converter(std::to_string(razrad), 1, 2, arch);
            control_bites_count_copy /= 10;
        }

        for (size_t j = 0 ; j < (file_size / pack_size) * pack_size; j++) {
            char ch;
            readfile.get(ch);
            to_be_coded += ch;
            if ((j + 1) % pack_size == 0) {
                converter(to_be_coded, pack_size, control_bytes_count, arch);
                to_be_coded = "";

            }
        }
        to_be_coded = "";
        for (size_t j = 0 ; j < file_size % pack_size; j++) {
            char ch;
            readfile.get(ch);
            to_be_coded += ch;
            converter(to_be_coded, 1, 2, arch);
            to_be_coded = "";
        }
    }

    arch.close();
}

// Структура кодированного файла
// 13 байт - размер обычного файла (в обр порядке)
// 4 байта - длина кодированного имени (в обр порядке)
// кодированное имя файла
// 3 байта - колическтво контрольных битов
// кодированный файл
bool inList(const std::vector<std::string>& list, const std::string& file_name) {
    if (list.size() == 0) {
        return true;
    }
    for (size_t i = 0; i < list.size(); i++) {
        if (list[i] == file_name) {
            return true;
        }
    }
    return false;
}

void extractArchive(const std::string& archive_name, const std::vector<std::string>& extract_files) {
    std::ifstream arch(archive_name, std::ios::binary);
    std::string to_be_decoded = "";
    std::cout << "Extract\n";

    arch.seekg(0, std::ios_base::end);
    uint64_t arch_size = arch.tellg();
    arch.seekg(0, std::ios_base::beg);
    uint64_t cursor_position = 0;

    while (cursor_position < arch_size) {
        uint64_t file_size = 0;
        uint64_t name_size = 0;
        std::string file_name = "";
        uint8_t control_bytes_count = 0;
        readHeader(file_size, file_name, name_size, control_bytes_count, arch);

        uint64_t infobites_count = (1 << control_bytes_count) - control_bytes_count - 1;
        uint64_t pack_size = lcm(infobites_count, 8) / 8;
        uint64_t new_file_size = (file_size / pack_size) * pack_size * (1 << control_bytes_count) / infobites_count + (file_size % pack_size) * 4;

        //  файл в списке - обрабатываем
        if (inList(extract_files, file_name)) {
            std::cout << "File found" <<'\n';
            std::ofstream out(file_name, std::ios::binary);
            for (int j = 0; j < (file_size / pack_size) * pack_size * (1 << control_bytes_count) / infobites_count; j++) {
                char ch;
                arch.get(ch);
                to_be_decoded += ch;
                if ((j + 1) % (pack_size * (1 << control_bytes_count) / infobites_count) == 0) {
                    out << decoder(to_be_decoded, control_bytes_count);
                    to_be_decoded = "";
                }
            }

            to_be_decoded = "";
            for (int j = 0; j < 4 * (file_size % pack_size); j++) {
                char ch;
                arch.get(ch);
                to_be_decoded += ch;
                if ((j + 1) % 4 == 0) {
                    out << decoder(to_be_decoded, 2);
                    to_be_decoded = "";
                }

            }
            out.close();
            cursor_position = arch.tellg();
        } else {
            cursor_position += ((kHeaderFileSize + kHeaderNameLength) * 4 + name_size + kHeaderControlBitsCount * 4 + new_file_size);
            arch.seekg(cursor_position, std::ios_base::beg);
        }
    }
}

void printList(const std::string& archive_name) {
    std::ifstream arch(archive_name, std::ios::binary);
    std::string to_be_decoded = "";
    std::cout <<"List of files:" << '\n';

    arch.seekg(0, std::ios_base::end);
    uint64_t arch_size = arch.tellg();
    arch.seekg(0, std::ios_base::beg);
    uint64_t cursor_position = 0;

    while (cursor_position < arch_size) {
        uint64_t file_size = 0;
        uint64_t name_size = 0;
        std::string file_name = "";
        uint8_t control_bytes_count = 0;
        readHeader(file_size, file_name, name_size, control_bytes_count, arch);

        uint64_t infobites_count = (1 << control_bytes_count) - control_bytes_count - 1;
        uint64_t pack_size = lcm(infobites_count, 8) / 8;
        uint64_t new_file_size = (file_size / pack_size) * pack_size * (1 << control_bytes_count) / infobites_count + (file_size % pack_size) * 4;

        std::cout << file_name << '\n';
        cursor_position += (kHeaderFileSize + kHeaderNameLength) * 4 + name_size + kHeaderControlBitsCount * 4 + new_file_size;
        arch.seekg(cursor_position, std::ios_base::beg);
    }
}

void addFile(const std::string& archive_name, const std::string& file_name, uint8_t control_bytes_count) {
    std::ofstream arch(archive_name, std::ios::binary | std::ios::in | std::ios::out);
    arch.seekp(0, std::ios_base::end);
    uint64_t archive_size = arch.tellp();

    std::ifstream readfile(file_name, std::ios::binary);
    readfile.seekg(0, std::ios_base::end);
    uint64_t file_size = readfile.tellg();
    readfile.seekg(0, std::ios_base::beg);

    uint64_t infobites_count = (1 << control_bytes_count) - control_bytes_count - 1;
    uint64_t pack_size = lcm(infobites_count, 8) / 8;
    uint64_t new_name_size = file_name.size() * 4;

    // размер обычного файла в обратном порядке
    uint64_t file_size_copy = file_size;
    for (size_t j = 0; j < kHeaderFileSize; j++) {
        uint8_t razrad = file_size_copy % 10;
        converter(std::to_string(razrad), 1, 2, arch);
        file_size_copy /= 10;
    }

    for (size_t j = 0; j < kHeaderNameLength; j++) {
        uint8_t razrad = new_name_size % 10;
        converter(std::to_string(razrad), 1, 2, arch);
        new_name_size /= 10;
    }
    std::string to_be_coded = "";
    for (int j = 0; j < file_name.size(); j++) {
        to_be_coded += file_name[j];
        converter(to_be_coded, 1, 2, arch); //  записываем кодированное имя файла
        to_be_coded = "";
    }
    to_be_coded = "";

    uint8_t control_bites_count_copy = control_bytes_count; // < 256 - 3 symbols
    for (size_t j = 0; j < kHeaderControlBitsCount; j++) {
        uint8_t razrad = control_bites_count_copy % 10;
        converter(std::to_string(razrad), 1, 2, arch);
        control_bites_count_copy /= 10;
    }

    for (int j = 0 ; j < (file_size / pack_size) * pack_size; j++) {
        char ch;
        readfile.get(ch);
        to_be_coded += ch;
        if ((j + 1) % pack_size == 0) {
            converter(to_be_coded, pack_size, control_bytes_count, arch);
            to_be_coded = "";
        }
    }
    to_be_coded = "";

    for (int j = 0 ; j < file_size % pack_size; j++) {
        char ch;
        readfile.get(ch);
        to_be_coded += ch;
        converter(to_be_coded, 1, 2, arch);
        to_be_coded = "";
    }

    arch.close();
}

void deleteFile(const std::string& archive_name, const std::string& delete_file_name) {
    std::string temp = std::filesystem::temp_directory_path().generic_string() + archive_name;
    //std::string temp = archive_name + "abcder";
    std::ofstream archivecopy(temp, std::ios::binary);
    std::ifstream arch(archive_name, std::ios::binary);
    std::string to_be_decoded = "";

    arch.seekg(0, std::ios_base::end);
    uint64_t arch_size = arch.tellg();
    arch.seekg(0, std::ios_base::beg);
    uint64_t cursor_position = 0;

    while (cursor_position < arch_size) {
        uint64_t file_size = 0;
        uint64_t name_size = 0;
        std::string file_name = "";
        uint8_t control_bytes_count = 0;
        readHeader(file_size, file_name, name_size, control_bytes_count, arch);

        uint64_t infobites_count = (1 << control_bytes_count) - control_bytes_count - 1;
        uint64_t pack_size = lcm(infobites_count, 8) / 8;
        uint64_t new_file_size = (file_size / pack_size) * pack_size * (1 << control_bytes_count) / infobites_count + (file_size % pack_size) * 4;

        uint64_t file_size_in_archive = (kHeaderNameLength + kHeaderFileSize) * 4 + name_size + kHeaderControlBitsCount * 4 + new_file_size;
        if (file_name != delete_file_name) {
            arch.seekg(cursor_position, std::ios_base::beg);
            char ch;
            for (size_t j = 0; j < file_size_in_archive; j++) {
                arch.get(ch);
                archivecopy << ch;
            }
            cursor_position += file_size_in_archive;
        } else {
            cursor_position += file_size_in_archive;
            arch.seekg(cursor_position, std::ios_base::beg);
        }
    }
    archivecopy.close();

    std::ofstream new_arch(archive_name, std::ios::binary);
    std::ifstream archive_copy(temp, std::ios::binary);
    char ch;
    while (archive_copy.get(ch)) {
        new_arch << ch;
    }
    new_arch.close();
}

void mergeArchives(const std::string& first_archive_name, const std::string& second_archive_name, const std::string& result_archive_name) {
    std::ofstream new_arch(result_archive_name, std::ios::binary);

    std::ifstream first_arch(first_archive_name, std::ios::binary);
    char ch;
    while (first_arch.get(ch)) {
        new_arch << ch;
    }

    std::ifstream second_arch(second_archive_name, std::ios::binary);
    while (second_arch.get(ch)) {
        new_arch << ch;
    }

    new_arch.close();
}
