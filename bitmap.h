#pragma once
#include <cstdint>
#include <string>

using namespace std;

#pragma pack(push, 1)

struct BitmapFileHeader {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offbits;
};

struct BitmapInfoHeader {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitcount;
    uint32_t compression;
    uint32_t sizeimage;
    int32_t xpelspermeter;
    int32_t ypelspermeter;
    uint32_t colorsused;
    uint32_t colorsimportant;
};

struct BitmapHeader {
    BitmapFileHeader bitmapFileHeader;
    BitmapInfoHeader bitmapInfoHeader;
};
#pragma pack(pop)

class Bitmap {
    public:
        Bitmap();
        ~Bitmap();
        bool Load(string& file);
        bool Save(string& file);
        uint32_t Width();
        uint32_t Height();
        uint16_t BitCount();
        uint32_t SizeImage();
        uint8_t *Data();

    private:
        BitmapHeader m_header;
        uint8_t* m_data;
        uint32_t m_size;
        uint8_t* m_offset_data;
        size_t m_offset_size;
};
