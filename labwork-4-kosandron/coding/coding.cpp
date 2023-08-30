#include "coding.h"

void converter(const std::string& message, uint64_t pack_size, uint8_t control_bytes_count, std::ofstream& out) {
    std::vector <uint8_t> code(8 * message.size());

    int to_be_converted = 0;
    for (size_t i = 0; i < message.size(); i++) {
        uint8_t ch = message[i];
        for (int j = 7; j >= 0; j--) {
            code[i * 8 + j] = ch % 2;
            ch >>= 1;

        }
    }

    uint64_t infobites_count = (1 << control_bytes_count) - control_bytes_count - 1;
    uint64_t new_message_size_bites = 8 *(pack_size * (1 << control_bytes_count) / infobites_count);
    uint8_t printcode[new_message_size_bites];
    uint8_t convert[infobites_count];

    uint64_t printed_coded_bits = 0;
    uint64_t hamcode_length = (1 << control_bytes_count);
    for (size_t i = 0; i < message.size() * 8; i++) {
        convert[i % infobites_count] = code[i];
        if ((i + 1) % infobites_count == 0) {
            uint8_t printed[hamcode_length];
            coder(convert, infobites_count, control_bytes_count, printed);

            for (int b = 0; b < (1 << control_bytes_count); b++) {
                printcode[printed_coded_bits + b] = printed[b];
            }
            printed_coded_bits += hamcode_length;
        }
    }

    uint8_t ch = 0;
    for (int i = 0; i < new_message_size_bites; i+= 8) {
        uint8_t t = 128;
        for (size_t j = 0; j < 8; j++) {
            ch += printcode[i + j] * t;
            t /= 2;
        }
        out << ch;
        ch = 0;
    }
}

void coder(const uint8_t* line, uint64_t line_size, uint8_t control_bites_count, uint8_t* code) {
    uint64_t ham_size = (1 << control_bites_count);
    int power = 0;
    int t = 0;
    for (size_t i = 0; i < ham_size; i++) { //  запись кода
        if (i == (1 << power) - 1) {
            code[i] = 0;
            power++;
        } else {
            code[i] = line[t];
            t++;
        }
    }

    for (int i = 0; i < control_bites_count; i++) { //  расчет контрольных битов
        uint64_t hamcount = 0;
        uint64_t index = (1 << i);
        uint64_t j = index - 1;
        while (j < ham_size) {
            for (size_t z = j; z < j + index; z++) {
                hamcount += code[z];
            }
            j += 2 * index;
        }

        code[index - 1] = hamcount % 2;
    }

    uint64_t hamcount = 0;
    for (size_t i = 0; i < ham_size; i++) { //  запись кода
        if (i == (1 << power) - 1) {
            power++;
        } else {
            hamcount += code[i];
        }
    }
    code[ham_size - 1] = hamcount % 2;
}

uint64_t errorIndex(uint8_t control_bites_count, const std::vector<uint8_t>& code, uint64_t bite_position) {
    uint64_t error = 0;
    uint64_t hamcode_length = (1 << control_bites_count);
    for (size_t z = 0; z < control_bites_count; z++) { //  перерасчет контрольных битов
        uint64_t hamcount = 0;
        uint64_t index = (1 << z);
        uint64_t j = index - 1;
        while (j < hamcode_length) {
            for (size_t t = j; t < j + index; t++) {
                hamcount += code[t + bite_position];
            }
            j += 2 * index;
        }

        if ((hamcount - code[bite_position + index - 1])  % 2 != code[bite_position + index - 1]) {
            error += index;
        }
    }

    return error;
}

std::string decoder(const std::string& message, uint8_t control_bites_count) {
    std::vector <uint8_t> code(8 * message.size());
    for (size_t i = 0; i < message.size(); i++) {
        uint8_t ch = message[i];
        for (int j = 7; j >= 0; j--) {
            code[i * 8 + j] = ch % 2;
            ch >>= 1;

        }
    }

    uint8_t ch = 0;
    uint8_t count_printed_info_bits = -1;
    std::string answer = ""; //  результат работы декодировщика
    uint64_t hamcode_length = (1 << control_bites_count);
    for(size_t i = 0; i < 8 * message.size(); i += hamcode_length) {
        uint64_t error = errorIndex(control_bites_count, code, i);
        if (error > 0) {
            code[i + error - 1] = (code[i + error - 1] + 1) % 2;
            error = 0;
            error = errorIndex(control_bites_count, code, i);
            uint64_t hamcount = 0;
            int r = 0;
            for (size_t z = 0; z < hamcode_length - 1; z++) {
                if (z % 2 == 1) {
                    hamcount += code[z + i];
                }
            }
            hamcount = hamcount % 2;

            if (error > 0 or hamcount != code[i + hamcode_length - 1]) {
                std::cout << "Error cannot be fixed!" << '\n';
            }
        }

        uint8_t power = 0;
        uint64_t control_byte_index = 0;

        for (size_t j = 0; j < hamcode_length; j++) {
            if (control_byte_index == j) {
                power++;
                control_byte_index = (1 << power) - 1;
            } else {
                count_printed_info_bits++;
                ch += code[i + j] *  (1 << static_cast<uint8_t>(7 - count_printed_info_bits % 8));
                if ((count_printed_info_bits + 1) % 8 == 0) {
                    answer += ch;
                    ch = 0;
                }
            }
        }
    }

    return answer;
}