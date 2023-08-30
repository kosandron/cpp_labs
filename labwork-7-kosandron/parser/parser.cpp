#include "parser.h"

const std::string flag(bool value) {
    if (value) {
        return "on";
    }
    return "off";
}


void Header::readHeader(std::ifstream& in) {
    char ch;
    for (int i = 0; i < 3; i++) {
        in.get(ch);
        format += ch;
    }

    in.get(ch);
    uint8_t byte_variant = ch;
    for (int i = 0; i < 8; i++) {
        version1 += (byte_variant % 2) * (1 << i);
        byte_variant >>= 1;
    }
    in.get(ch);
    byte_variant = ch;
    for (int i = 0; i < 8; i++) {
        version2 += (byte_variant % 2) * (1 << i);
        byte_variant >>= 1;
    }
    std::cout << "Format and version: " << format << " " << version1 << " " << version2 << '\n';

    //flags
    in.get(ch);
    byte_variant = ch;
    byte_variant >>= 4;
    footer_present = true ? byte_variant % 2 == 1 : false;
    byte_variant >>= 1;

    experimental_indicator = true ? byte_variant % 2 == 1 : false;
    byte_variant >>= 1;

    extended_header = true ? byte_variant % 2 == 1 : false;
    byte_variant >>= 1;

    unsynchronisation = true ? byte_variant % 2 == 1 : false;
    byte_variant >>= 1;

    std::cout << "Unsynchronisation: " << flag(unsynchronisation) << '\n';
    std::cout << "Extended header: " << flag(extended_header) << '\n';
    std::cout << "Experimental indicator: " << flag(experimental_indicator) << '\n';
    std::cout << "Footer present: " << flag(footer_present) << '\n';

    // tag size - in BITES
    for (int i = 0; i < 4; i++) {
        in.get(ch);
        byte_variant = ch;
        for (int j = 0; j < 7; j++) {
            tag_size += (byte_variant % 2) * (1 << j + (3 - i) * 7);
            byte_variant >>= 1;
        }

    }
    std::cout << tag_size << '\n';
}

void Frame::readFrameFlags(std::ifstream& input) {
    char flags[2];
    input.read(flags, 2);

    flags[0] >>= 4;
    this->flags.read_only = flags[0] % 2;

    flags[0] >>= 1;
    this->flags.file_after_preservation = flags[0] % 2;

    flags[0] >>= 1;
    this->flags.tag_after_preservation = flags[0] % 2;


    this->flags.data_length_indicator = flags[1] % 2;

    flags[1] >>= 1;
    this->flags.unsynchronisation = flags[1] % 2;

    flags[1] >>= 1;
    this->flags.encryption = flags[1] % 2;

    flags[1] >>= 1;
    this->flags.compression = flags[1] % 2;

    flags[1] >>= 3;
    this->flags.grouping_identity = flags[1] % 2;
}

void Frame::setFrameSize(std::ifstream& input) {
    size = 0;
    char frame_size[4];
    input.read(frame_size, 4);
    for (int i = 0; i < 4; i++) {
        for (int j = 6; j >=0; j--) {
            if (bool((1<<j) & frame_size[i])) {
                size |= 1;
            }
            size <<= 1;
        }
    }
    size = size / 2;
}
Frame::Frame(const std::string& name, std::ifstream& input) {
    this->name = name;
    setFrameSize(input);
    readFrameFlags(input);
}

OwnerIdentifierFrames::OwnerIdentifierFrames(const std::string &name, std::ifstream &input) : Frame(name, input) {
    char ch;
    uint32_t counter = 0;
    input.get(ch);
    while (ch != '\0') {
        owner_identifier += ch;
        counter++;
        input.get(ch);
    }
    counter++;
}
const std::string OwnerIdentifierFrames::getOwnerIdentifier() const {
    return owner_identifier;
}

TextEncodingFrames::TextEncodingFrames(const std::string &name, std::ifstream &input) : Frame(name, input) {
    input.get(text_encoding);
}
char TextEncodingFrames::getTextEncoding() const {
    return text_encoding;
}

LanguageFrames::LanguageFrames(const std::string &name, std::ifstream &input) : TextEncodingFrames(name, input) {
    input.read(language, 3);
}
const std::string LanguageFrames::getLanguage() const {
    std::string s;
    s += language[0];
    s += language[1];
    s += language[2];
    return s;
}

TimeStampFormatFrames::TimeStampFormatFrames(const std::string &name, std::ifstream &input) : Frame(name, input) {
    input.get(time_stamp_format);
}
char TimeStampFormatFrames::getTimeStampFormat() const {
    return time_stamp_format;
}

UFID::UFID(const std::string& name, std::ifstream& input) : OwnerIdentifierFrames(name, input) {
    char ch;
    for (size_t i = counter; i < size; i++) {
        input.get(ch);
        identifier += ch;
    }
}
const std::string UFID::getIdentifier() const {
    return identifier;
}
void UFID::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Owner identifier: " << owner_identifier << '\n';
    std::cout << "Identifier: " << identifier << '\n';
}

textFrame::textFrame(const std::string& name, std::ifstream& input) : TextEncodingFrames(name, input) {
    //std::cout << name;
    char ch;
    input.get(ch);
    uint32_t counter = 2;
    std::string line;
    while (counter != size) {
        if (ch == '\0') {
            lines.push_back(line);
            line = "";
        } else {
            line += ch;
        }
        input.get(ch);
        counter++;
    }
    lines.push_back(line);
}
const std::vector<std::string>& textFrame::getString() const {
    return lines;
}
void textFrame::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Text encoding: " << text_encoding << '\n';
    std::cout << "Information (lines): " << '\n';
    for (size_t i = 0; i < lines.size(); i++) {
        std::cout << i + 1 << ") " << lines[i] << '\n';
    }
}

TXXX::TXXX(const std::string& name, std::ifstream& input) : TextEncodingFrames(name, input) {
    //std::cout << name;
    char ch;
    bool flag_begin_of_value = false;
    for (size_t i = 0; i < size - 1; i++) {
        input.get(ch);
        if (ch == '\0') {
            flag_begin_of_value = true;
            continue;
        }
        if (!flag_begin_of_value) {
            description += ch;
        } else {
            value += ch;
        }
    }
}
void TXXX::setValue(const std::string& value) {
    this->value = value;
}
const std::string TXXX::getValue() const {
    return value;
}
const std::string TXXX::getDescription() const {
    return description;
}
void TXXX::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Text encoding: " << text_encoding << '\n';
    std::cout << "Description: " << description << '\n';
    std::cout << "Value: " << value << '\n';
    std::cout << '\n';
}

URLframe::URLframe(const std::string& name, std::ifstream& input) : Frame(name, input) {
    char ch;
    for (size_t i = 0; i < size; i++) {
        input.get(ch);
        URL += ch;
    }
}
void URLframe::setURL(const std::string& value) {
    URL = value;
}
const std::string URLframe::getURL() const {
    return URL;
}
void URLframe::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "URL: " << URL << '\n';
}

WXXX::WXXX(const std::string& name, std::ifstream& input) : TXXX(name, input){}
void WXXX::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Text encoding: " << text_encoding << '\n';
    std::cout << "URL: " << value << '\n';
}

ETCO::ETCO(const std::string& name, std::ifstream& input) : TimeStampFormatFrames(name, input) {
    input.get(type_of_event);
    char ch;
    for (int i = 0; i < 4; i++) {
        input.get(ch);
        time_stamp += (1<<(3 - i)) * uint8_t(ch);
    }
}
uint32_t ETCO::getTime() const {
    return time_stamp;
}
void ETCO::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Text encoding: " << time_stamp << '\n';
}

USLT::USLT(const std::string& name, std::ifstream& input) : LanguageFrames(name, input) {
    char ch;
    bool flag_begin_of_lyrics = false;
    for (size_t i = 0; i < size - 4; i++) {
        input.get(ch);
        if (ch == '\0') {
            flag_begin_of_lyrics = true;
            continue;
        }
        if (!flag_begin_of_lyrics) {
            content_descriptor += ch;
        } else {
            lyrics += ch;
        }
    }
}
void USLT::setLyrics(const std::string& value) {
    lyrics = value;
}
const std::string USLT::getLyrics() const {
    return lyrics;
}
const std::string USLT::getDescription() const {
    return content_descriptor;
}
void USLT::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Text encoding: " << text_encoding << '\n';
    std::cout << "Language: " << language[0] << language[1] << language[2] << '\n';
    std::cout << "Content descriptor: " << content_descriptor << '\n';
    std::cout << "Lyrics: " << lyrics << '\n';
}

SYLT::SYLT(const std::string& name, std::ifstream& input) : LanguageFrames(name, input) {
    input.get(time_stamp_format);
    input.get(content_type);
    char ch;
    for (size_t i = 0; i < size - 7; i++) {
        input.get(ch);
        content_descriptor += ch;
    }
    input.get(ch);  // &00
}
char SYLT::getTimeStampFormat() const {
    return time_stamp_format;
}
char SYLT::getContentType() const {
    return content_type;
}
void SYLT::setContentDescriptor(const std::string& value) {
    content_descriptor = value;
}
const std::string SYLT::getContentDescriptor() const {
    return content_descriptor;
}
void SYLT::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Text encoding: " << text_encoding << '\n';
    std::cout << "Language: " << language[0] << language[1] << language[2] << '\n';
    std::cout << "Time stamp format: " << time_stamp_format << '\n';
    std::cout << "Content type: " << content_type << '\n';
    std::cout << "Content descriptor: " << content_descriptor << '\n';
}

COMM::COMM(const std::string& name, std::ifstream& input) : LanguageFrames(name, input) {
    char ch;
    bool flag_begin_of_actual_text = false;
    for (size_t i = 0; i < size - 4; i++) {
        input.get(ch);
        if (ch == '\0') {
            flag_begin_of_actual_text = true;
            continue;
        }
        if (!flag_begin_of_actual_text) {
            short_content_decriptor += ch;
        } else {
            the_actual_text += ch;
        }
    }
}
void COMM::setText(const std::string& value) {
    the_actual_text = value;
}
const std::string COMM::getText() const {
    return the_actual_text;
}
void COMM::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Text encoding: " << text_encoding << '\n';
    std::cout << "Language: " << language[0] << language[1] << language[2] << '\n';
    std::cout << "good";
    std::cout << "Content descriptor: " << short_content_decriptor << '\n';
    std::cout << "The actual text: " << the_actual_text << '\n';
}

RVA2::RVA2(const std::string& name, std::ifstream& input) : Frame(name, input) {
    char ch;
    input.get(ch);
    while (ch != '\0') {
        identificator += ch;
        input.get(ch);
        //counter++;
    }
    input.get(type_of_channel);
    input.get(ch);
    if (ch > 63) {
        volume_adjustment = -(ch << 1) / 2;
        input.get(ch);
        volume_adjustment -= ch / 256;
    } else {
        volume_adjustment = (ch << 1) / 2;
        input.get(ch);
        volume_adjustment += ch / 256;
    }
    input.get(bits_representing_peak);
    for(size_t i = 0; i < bits_representing_peak; i++) {
        input.get(ch);
        peak_volume += ch;
    }
}
double RVA2::getVolumeAdjustment() const {
    return volume_adjustment;
}
const std::string RVA2::getTypeOfChannel() const {
    if (type_of_channel == 0) {
        return "Other";
    } else if (type_of_channel == 1) {
        return "Master volume";
    } else if (type_of_channel == 2) {
        return "Front right";
    } else if (type_of_channel == 3) {
        return "Front left";
    } else if (type_of_channel == 4) {
        return "Back right";
    } else if (type_of_channel == 5) {
        return "Back left";
    } else if (type_of_channel == 6) {
        return "Front centre";
    } else if (type_of_channel == 7) {
        return "Left centre";
    } else if (type_of_channel == 8) {
        return "Subwoofer";
    }
}
const std::string RVA2::getPeakVolume() const {
    return peak_volume;
}
void RVA2::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Identification: " << identificator << '\n';
    std::cout << "Type of channel: " << RVA2::getTypeOfChannel() << '\n';
    std::cout << "Volume adjustment: " << volume_adjustment << "dB" << '\n';
    std::cout << "Bits representing peak: " << bits_representing_peak << '\n';
    std::cout << "Peak volume: " << peak_volume << '\n';
}

EQU2::EQU2(const std::string& name, std::ifstream& input) : Frame(name, input) {
    input.get(interpolation_method);
    char ch;
    input.get(ch);
    while (ch != '\0') {
        identification += ch;
        input.get(ch);
    }

    input.get(ch);
    frequency = uint8_t(ch) << 8;
    input.get(ch);
    frequency += uint8_t(ch);
    input.get(ch);
    if (ch > 63) {
        volume_adjustment = -(ch << 1) / 2;
        input.get(ch);
        volume_adjustment -= ch / 256;
    } else {
        volume_adjustment = (ch << 1) / 2;
        input.get(ch);
        volume_adjustment += ch / 256;
    }
}
double EQU2::getVolumeAdjustment() const {
    return volume_adjustment;
}
const std::string EQU2::getIdentification() const {
    return identification;
}
uint16_t EQU2::getFrequency() const {
    return frequency;
}
const std::string EQU2::getInterpretationMethod() const {
    if (interpolation_method == 0) {
        return "Band";
    } else if (interpolation_method == 1) {
        return "Linear";
    }
}
void EQU2::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Identification: " << identification << '\n';
    std::cout << "Interpretation method: " << EQU2::getInterpretationMethod() << '\n';
    std::cout << "Frequency: " << frequency << "Hz" << '\n';
    std::cout << "Volume adjustment: " << volume_adjustment << "dB" << '\n';
}

PCNT::PCNT(const std::string& name, std::ifstream& input) : Frame(name, input) {
    char ch;
    for (size_t i = 0; i < size; i++) {
        counter <<= 8;
        input.get(ch);
        counter += ch;
    }
}
uint64_t PCNT::getCounter() const {
    return counter;
}
void PCNT::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Counter: " << counter << '\n';
}

POPM::POPM(const std::string& name, std::ifstream& input) : Frame(name, input) {
    char ch;
    uint64_t read_counter = 1;
    input.get(ch);
    while (ch != '\0') {
        email_to_user += ch;
        input.get(ch);
        read_counter++;
    }
    input.get(ch);
    rating = uint8_t(ch);
    read_counter++;
    for (size_t i = read_counter; i < size; i++) {
        counter <<= 8;
        input.get(ch);
        counter += ch;
    }
}
uint64_t POPM::getCounter() const {
    return counter;
}
uint8_t POPM::getRating() const {
    return rating;
}
const std::string POPM::getEmail() const {
    return email_to_user;
}
void POPM::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Email to user: " << email_to_user << '\n';
    std::cout << "Rating: " << (int)rating << '\n';
    std::cout << "Counter: " << counter << '\n';
}

RBUF::RBUF(const std::string& name, std::ifstream& input) : Frame(name, input) {
    char ch;
    for (int i = 0; i < 3; i++) {
        buffer_size <<= 8;
        input.get(ch);
        buffer_size += ch;
    }
    input.get(ch);
    if (ch == 1) {
        embedded_info_flag = true;
    }
    for (int i = 0; i < 4; i++) {
        offset_to_next_tag <<= 8;
        input.get(ch);
        buffer_size += ch;
    }
}
uint32_t RBUF::getBufferSize() const {
    return buffer_size;
}
uint64_t RBUF::getOffsetToNextFlag() const {
    return offset_to_next_tag;
}
bool RBUF::embeddedInfoFlag() const {
    return embedded_info_flag;
}
void RBUF::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Buffer size: " << buffer_size << '\n';
    std::cout << "Embedded info flag: " << embedded_info_flag << '\n';
    std::cout << "Offset to next tag: " << offset_to_next_tag << '\n';
}

POSS::POSS(const std::string& name, std::ifstream& input) : TimeStampFormatFrames(name, input) {
    char ch;
    for (int i = 0; i < 4; i++) {
        position <<= 8;
        input.get(ch);
        position += ch;
    }
}
uint32_t POSS::getPosition() const {
    return position;
}
void POSS::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Time stamp format: " << time_stamp_format << '\n';
    std::cout << "Position: " << position << '\n';
}

USER::USER(const std::string& name, std::ifstream& input) : TextEncodingFrames(name, input) {
    input.read(language, 3);
    char ch;
    for (size_t i = 0; i < size - 4; i++) {
        input.get(ch);
        the_actual_text += ch;
    }
}
const std::string USER::getText() const {
    return the_actual_text;
}
const std::string USER::getLanguage() const {
    std::string s;
    s += language[0];
    s += language[1];
    s += language[2];
    return s;
}
void USER::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Text encoding: " << text_encoding << '\n';
    std::cout << "Language: " << language[0] << language[1] << language[2] << '\n';
    std::cout << "The actual text: " << the_actual_text << '\n';
}

LINK::LINK(const std::string& name, std::ifstream& input) : Frame(name, input) {
    char ch;
    input.read(identifier, 4);
    uint32_t counter = 4;
    input.get(ch);
    while (ch != '\0') {
        URL += ch;
        counter++;
        input.get(ch);
    }
    counter++;

    std::string id;
    input.get(ch);
    counter++;
    while (counter != size) {
        if (ch == '\0') {
            ID.push_back(id);
            id = "";
        } else {
            id += ch;
        }
        input.get(ch);
        counter++;
    }
    ID.push_back(id);
}
char* LINK::getIdentifier() {
    return identifier;
}
const std::string LINK::getURL() const {
    return URL;
}
const std::vector<std::string>& LINK::getID() const {
    return ID;
}
void LINK::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Identifier: " << identifier << '\n';
    std::cout << "URL: " << URL << '\n';
    std::cout << "ID and additional data: " << '\n';
    for (size_t i = 0; i < ID.size(); i++) {
        std::cout << ID[i] << '\n';
    }
}

OWNE::OWNE(const std::string& name, std::ifstream& input) : TextEncodingFrames(name, input) {
    char ch;
    uint32_t counter = 1;
    input.get(ch);
    while (ch != '\0') {
        price_paid += ch;
        counter++;
        input.get(ch);
    }
    counter++;

    for (int i = 0; i < 8; i++) {
        input.get(ch);
        date_of_purch += ch;
    }
    counter += 8;

    for (size_t i = counter; i < size; i++) {
        input.get(ch);
        seller += ch;
    }
}
const std::string OWNE::getPricePaid() const {
    return price_paid;
}
const std::string OWNE::getDateOfPurch() const {
    return date_of_purch;
}
const std::string OWNE::getSeller() const {
    return seller;
}
void OWNE::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Text encoding: " << text_encoding << '\n';
    std::cout << "Price paid: " << price_paid << '\n';
    std::cout << "Date of purch: " << date_of_purch << '\n';
    std::cout << "Seller: " << seller << '\n';
}

ENCR::ENCR(const std::string& name, std::ifstream& input) : OwnerIdentifierFrames(name, input) {
    char ch;
    input.get(method_symbol);
    counter++;

    for (size_t i = counter; i < size; i++) {
        input.get(ch);
        enscryption_data += ch;
    }
}
char ENCR::getMethodSymbol() const  {
    return method_symbol;
}
const std::string ENCR::getEnscryptionData() const {
    return enscryption_data;
}
void ENCR::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Owner identifier: " << owner_identifier << '\n';
    std::cout << "Method symbol: " << method_symbol << '\n';
    std::cout << "Encryption data: " << enscryption_data << '\n';
}

GRID::GRID(const std::string& name, std::ifstream& input) : OwnerIdentifierFrames(name, input) {
    char ch;
    input.get(group_symbol);
    counter++;

    for (size_t i = counter; i < size; i++) {
        input.get(ch);
        group_depend_data += ch;
    }
}
char GRID::getGroupSymbol() const {
    return group_symbol;
}
const std::string GRID::getGroupDependData() const {
    return group_depend_data;
}
void GRID::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Owner identifier: " << owner_identifier << '\n';
    std::cout << "Group symbol: " << group_symbol << '\n';
    std::cout << "Group dependent data: " << group_depend_data << '\n';
}

PRIV::PRIV(const std::string& name, std::ifstream& input) : OwnerIdentifierFrames(name, input) {
    char ch;
    for (size_t i = counter; i < size; i++) {
        input.get(ch);
        private_data += ch;
    }
}
const std::string PRIV::getPrivateData() const {
    return private_data;
}
void::PRIV::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Owner identifier: " << owner_identifier << '\n';
    std::cout << "The private data: " << private_data << '\n';
}

SEEK::SEEK(const std::string& name, std::ifstream& input) : Frame(name, input) {
    char ch;
    for (int i = 0; i < 4; i++) {
        input.get(ch);
        minimum_offset_to_next_tag <<= 8;
        minimum_offset_to_next_tag += ch;
    }
}
uint32_t SEEK::getMinimumOffsetToNextTag() const {
    return minimum_offset_to_next_tag;
}
void SEEK::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Minimum offset to next flag: " << minimum_offset_to_next_tag << '\n';
}

COMR::COMR(const std::string& name, std::ifstream& input) : TextEncodingFrames(name, input) {
    char ch;
    uint64_t counter = 1;
    input.get(ch);
    while (ch != '\0') {
        price += ch;
        counter++;
        input.get(ch);
    }
    counter++;

    for (int i = 0; i < 8; i++) {
        input.get(ch);
        counter++;
        valid_until += ch;
    }
    while (ch != '\0') {
        contact_URL += ch;
        counter++;
        input.get(ch);
    }
    counter++;

    input.get(recived_as);
    while (ch != '\0') {
        name_of_seller += ch;
        counter++;
        input.get(ch);
    }
    counter++;
    while (ch != '\0') {
        description += ch;
        counter++;
        input.get(ch);
    }
    counter++;

    while (ch != '\0') {
        picture_MIME_type += ch;
        counter++;
        input.get(ch);
    }
    counter++;

    for (size_t i = counter; i < size; i++) {
        input.get(ch);
        seller_logo.push_back(ch);
    }
}

const std::string COMR::getPrice() const{
    return price;
}

const std::string COMR::getContactURL() const{
    return contact_URL;
}

const std::string COMR::getNameOfSeller() const{
    return name_of_seller;
}

const std::string COMR::getDescription() const{
    return description;
}

const std::string COMR::getRecivedAs() const {
    if (uint8_t(recived_as) == 0) {
        return "Other";
    } else if (uint8_t(recived_as) == 1) {
        return "Standard CD album with other songs";
    } else if (uint8_t(recived_as) == 2) {
        return "Compressed audio on CD";
    } else if (uint8_t(recived_as) == 3) {
        return "File over the Internet";
    } else if (uint8_t(recived_as) == 4) {
        return "Stream over the Internet";
    } else if (uint8_t(recived_as) == 5) {
        return "As note sheets";
    } else if (uint8_t(recived_as) == 6) {
        return "As note sheets in a book with other sheets";
    } else if (uint8_t(recived_as) == 7) {
        return "Music on other media";
    } else if (uint8_t(recived_as) == 8) {
        return "Non-musical merchandise";
    }
}

void COMR::print() const {
    std::cout << "Frame: " << name << '\n';
    std::cout << "Text encoding: " << text_encoding << '\n';
    std::cout << "Price: " << price << '\n';
    std::cout << "Valid until: " << valid_until << '\n';
    std::cout << "Contact URL: " << contact_URL << '\n';
    std::cout << "Recived as: " << getRecivedAs() << '\n';
    std::cout << "Name of seller: " << name_of_seller << '\n';
    std::cout << "Description: " << description << '\n';
    std::ofstream picture;
    picture.open("Logo." + picture_MIME_type.substr(6, picture_MIME_type.npos), std::ios::binary);
    for (size_t i = 0; i < seller_logo.size(); i++) {
        picture << seller_logo[i];
    }
    picture.close();
    std::cout << "Seller logo saved in \"Logo." << picture_MIME_type.substr(6, picture_MIME_type.npos) << '\"' << '\n';
}

void readInfo(const std::filesystem::path& file_name) {
    Header main_header;
    std::cout << file_name << '\n';
    std::ifstream input(file_name, std::ios::binary);
    main_header.readHeader(input);

    if (main_header.extended_header) { //  расширенный хедер есть
        uint32_t extended_header_size = 0;
        char ch;
        uint8_t byte_variant = ch;
        for (int i = 0; i < 4; i++) {
            input >> ch;
            byte_variant = ch;
            for (int j = 0; j < 7; j++) {
                extended_header_size += (byte_variant % 2) * (1 << j + (3 - i) * 7);
                byte_variant >>= 1;
            }
        }
        //std::cout << extended_header_size << '\n';
        input.seekg(extended_header_size - 4);
        for (size_t i = 0; i < extended_header_size - 4; i++) {
            input >> ch;
        }
    }

    char a_frame_name[4];
    while (true) {
        input.read(a_frame_name, 4);
        std::string frame_name = "";
        for (int k = 0; k < 4; k++) {
            frame_name += a_frame_name[k];
        }
        if (frame_name[0] == '0') {
            return;
        }

        if (frame_name == "TXXX") {
            TXXX new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name[0] == 'T') {
            textFrame new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "WXXX") {
            WXXX new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name[0] == 'W') {
            URLframe new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "USLT") {
            USLT new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "UFID") {
            UFID new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "ETCO") {
            ETCO new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "SYLT") {
            SYLT new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "COMM") {
            COMM new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "RVA2") {
            RVA2 new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "EQU2") {
            EQU2 new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "PCNT") {
            PCNT new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "POPM") {
            POPM new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "RBUF") {
            RBUF new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "LINK") {
            LINK new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "POSS") {
            POSS new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "USER") {
            USER new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "OWNE") {
            OWNE new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "COMR") {
            COMR new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "ENCR") {
            ENCR new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "GRID") {
            GRID new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "PRIV") {
            PRIV new_frame(frame_name, input);
            new_frame.print();
        } else if (frame_name == "SEEK") {
            SEEK new_frame(frame_name, input);
            new_frame.print();
        } else {
            Frame new_frame(frame_name, input);
            input.seekg(new_frame.frameSize());
            break;
        }
    }
}
