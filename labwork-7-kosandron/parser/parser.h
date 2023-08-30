#pragma once

#include <cstdio>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct tag_flags {
    bool tag_after_preservation;
    bool file_after_preservation;
    bool read_only;
    bool grouping_identity;
    bool compression;
    bool encryption;
    bool unsynchronisation;
    bool data_length_indicator;
};

class Frame {  // main parent frame
protected:
    uint64_t size = 0;
    std::string name;
    tag_flags flags;
public:
    Frame(const std::string& name, std::ifstream& input);
    void setFrameSize(std::ifstream& input);
    uint64_t frameSize() const {
        return size;
    }

    void setFrameName(const std::string& name) {
        this->name = name;
    }
    const std::string frameName() const {
        return name;
    }

    void readFrameFlags(std::ifstream& input);
    void printFrameFlags() const;

    void readFrameHeader(std::ifstream& input);
};

class Header{
public:
    std::string format;
    unsigned short int version1, version2;
    bool unsynchronisation, extended_header, experimental_indicator, footer_present;
    unsigned int tag_size = 0;
public:
    void readHeader(std::ifstream& in);
};

class OwnerIdentifierFrames : public Frame {
protected:
    std::string owner_identifier;
    uint32_t counter;
public:
    OwnerIdentifierFrames(const std::string& name, std::ifstream& input);
    const std::string getOwnerIdentifier() const;
};

class TextEncodingFrames : public Frame {
protected:
    char text_encoding;
public:
    TextEncodingFrames(const std::string& name, std::ifstream& input);
    char getTextEncoding() const;
};

class LanguageFrames : public TextEncodingFrames {
protected:
    char language[3];
public:
    LanguageFrames(const std::string& name, std::ifstream& input);
    const std::string getLanguage() const;
};

class TimeStampFormatFrames : public Frame {
protected:
    char time_stamp_format;
public:
    TimeStampFormatFrames(const std::string& name, std::ifstream& input);
    char getTimeStampFormat() const;
};

class UFID : public OwnerIdentifierFrames {
private:
    std::string identifier;
public:
    UFID(const std::string& name, std::ifstream& input);
    const std::string getIdentifier() const;
    void print() const;
};

class textFrame : public TextEncodingFrames {
private:
    std::vector<std::string> lines;
public:
    textFrame(const std::string& name, std::ifstream& input);
    const std::vector<std::string>& getString() const;
    void print() const;
};

class TXXX : public TextEncodingFrames {
private:
    std::string description;
    std::string value;
public:
    TXXX(const std::string& name, std::ifstream& input);
    void setValue(const std::string& value);
    const std::string getValue() const;
    const std::string getDescription() const;
    void print() const;
};

class URLframe : public Frame {
private:
    std::string URL;
public:
    URLframe(const std::string& name, std::ifstream& input);
    void setURL(const std::string& value);
    const std::string getURL() const;
    void print() const;
};

class WXXX : public TXXX {
private:
    std::string description;
    std::string value;
public:
    WXXX(const std::string& name, std::ifstream& input);
    void setValue(const std::string& value);
    const std::string getValue() const;
    const std::string getDescription() const;
    void print() const;
};

class ETCO : public TimeStampFormatFrames {
private:
    char type_of_event;
    uint32_t time_stamp = 0;
public:
    ETCO(const std::string& name, std::ifstream& input);
    uint32_t getTime() const;
    void print() const;
};

class USLT : public LanguageFrames {
private:
    std::string content_descriptor;
    std::string lyrics;
public:
    USLT(const std::string& name, std::ifstream& input);
    void setLyrics(const std::string& value);
    const std::string getLyrics() const;
    const std::string getDescription() const;
    void print() const;
};

class COMM : public LanguageFrames {
private:
    std::string short_content_decriptor;
    std::string the_actual_text;
public:
    COMM(const std::string& name, std::ifstream& input);
    void setText(const std::string& value);
    const std::string getText() const;
    void print() const;
};

class SYLT : public LanguageFrames {
private:
    char time_stamp_format;
    char content_type;
    std::string content_descriptor;
public:
    SYLT(const std::string& name, std::ifstream& input);
    char getTimeStampFormat() const;
    char getContentType() const;
    void setContentDescriptor(const std::string& value);
    const std::string getContentDescriptor() const;
    void print() const;
};

class RVA2 : public Frame {
private:
    std::string identificator;
    char type_of_channel;
    double volume_adjustment;
    char bits_representing_peak;
    std::string peak_volume;  // не знаю, что это
public:
    RVA2(const std::string& name, std::ifstream& input);
    double getVolumeAdjustment() const;
    const std::string getTypeOfChannel() const;
    const std::string getPeakVolume() const;
    void print() const;
};

class EQU2 : public Frame {
private:
    std::string identification;
    char interpolation_method;
    uint16_t frequency = 0;
    double volume_adjustment = 0;
public:
    EQU2(const std::string& name, std::ifstream& input);
    double getVolumeAdjustment() const;
    const std::string getIdentification() const;
    uint16_t getFrequency() const;
    const std::string getInterpretationMethod() const;
    void print() const;
};

class PCNT : public Frame {
private:
    uint64_t counter = 0;
public:
    PCNT(const std::string& name, std::ifstream& input);
    uint64_t getCounter() const;
    void print() const;
};

class POPM : public Frame {
private:
    std::string email_to_user;
    uint8_t rating;
    uint64_t counter = 0;
public:
    POPM(const std::string& name, std::ifstream& input);
    uint64_t getCounter() const;
    uint8_t getRating() const;
    const std::string getEmail() const;
    void print() const;
};

class RBUF: public Frame {
private:
    uint32_t buffer_size = 0;
    bool embedded_info_flag;
    uint64_t offset_to_next_tag = 0;
public:
    RBUF(const std::string& name, std::ifstream& input);
    uint64_t getOffsetToNextFlag() const;
    bool embeddedInfoFlag() const;
    uint32_t getBufferSize() const;
    void print() const;
};

class POSS: public TimeStampFormatFrames {
private:
    uint32_t position = 0;
public:
    POSS(const std::string& name, std::ifstream& input);
    uint32_t getPosition() const;
    void print() const;
};

class USER: public TextEncodingFrames {
private:
    char language[3];
    std::string the_actual_text;
public:
    USER(const std::string& name, std::ifstream& input);
    const std::string getLanguage() const;
    const std::string getText() const;
    void print() const;
};

class OWNE : public TextEncodingFrames {
private:
    std::string price_paid;
    std::string date_of_purch;
    std::string seller;
public:
    OWNE(const std::string& name, std::ifstream& input);
    const std::string getPricePaid() const;
    const std::string getDateOfPurch() const;
    const std::string getSeller() const;
    void print() const;
};

class LINK : public Frame {
private:
    char identifier[4];
    std::string URL;
    std::vector<std::string> ID;
public:
    LINK(const std::string& name, std::ifstream& input);
    char* getIdentifier();
    const std::string getURL() const;
    const std::vector<std::string>& getID() const;
    void print() const;
};

class COMR : public TextEncodingFrames {
private:
    std::string price;
    std::string valid_until;
    std::string contact_URL;
    char recived_as;
    std::string name_of_seller;
    std::string description;
    std::string picture_MIME_type;
    std::vector <char> seller_logo;
public:
    COMR(const std::string& name, std::ifstream& input);
    const std::string getPrice() const;
    const std::string getContactURL() const;
    const std::string getNameOfSeller() const;
    const std::string getDescription() const;
    const std::string getRecivedAs() const;
    void print() const;
};

class ENCR : public OwnerIdentifierFrames {
private:
    char method_symbol;
    std::string enscryption_data;
public:
    ENCR(const std::string& name, std::ifstream& input);
    char getMethodSymbol() const;
    const std::string getEnscryptionData() const;
    void print() const;
};

class GRID : public OwnerIdentifierFrames {
private:
    char group_symbol;
    std::string group_depend_data;
public:
    GRID(const std::string& name, std::ifstream& input);
    char getGroupSymbol() const;
    const std::string getGroupDependData() const;
    void print() const;
};

class PRIV : public OwnerIdentifierFrames {
private:
    std::string private_data;
public:
    PRIV(const std::string& name, std::ifstream& input);
    const std::string getPrivateData() const;
    void print() const;
};

class SEEK : public Frame {
private:
    uint32_t minimum_offset_to_next_tag = 0;
public:
    SEEK(const std::string& name, std::ifstream& input);
    uint32_t getMinimumOffsetToNextTag() const;
    void print() const;
};

//void readHeader(std::ifstream& in);
void readInfo(const std::filesystem::path& file_name);
