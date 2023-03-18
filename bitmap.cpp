#include "bitmap.h"
#include <fstream>
#include <iostream>

Bitmap::Bitmap() : m_data(nullptr), m_size(0), m_offset_data(nullptr), m_offset_size(0) {}

Bitmap::~Bitmap() {
    delete[] m_data;
    delete[] m_offset_data;
}

bool Bitmap::Load(string& file) {
    ifstream f(file, ios::binary);
    if (!f) {
        cerr << "Error: Failed to open file " << file << endl;
        return false;
    }
    f.read(reinterpret_cast<char*>(&m_header), sizeof(m_header));
    if (m_header.bitmapFileHeader.type != 0x4D42) {
        cerr << "Error: Invalid bitmap file format in " << file << endl;
        return false;
    }

    m_data = new uint8_t[m_header.bitmapInfoHeader.sizeimage];
    m_size = m_header.bitmapInfoHeader.sizeimage;
    m_offset_size = m_header.bitmapFileHeader.offbits - sizeof(m_header);
    m_offset_data = new uint8_t[m_offset_size];
    //f.seekg(m_header.bitmapFileHeader.offbits, ios_base::beg);
    f.read(reinterpret_cast<char*>(m_offset_data), m_offset_size);
    f.read(reinterpret_cast<char*>(m_data), m_header.bitmapInfoHeader.sizeimage);
    f.close();
    return true;
}

bool Bitmap::Save(string& file) {
    ofstream f(file, ios::out | ios::binary);
    if (!f) {
        cerr << "Error: Failed to create file " << file << endl;
        return false;
    }

    f.write(reinterpret_cast<char*>(&m_header), sizeof(m_header));
    f.write(reinterpret_cast<char*>(m_offset_data), m_offset_size);
    f.write(reinterpret_cast<char*>(m_data), m_header.bitmapInfoHeader.sizeimage);
    f.close();
    return true;
}

uint32_t Bitmap::Width() {
    return m_header.bitmapInfoHeader.width;
}

uint32_t Bitmap::Height() {
    return m_header.bitmapInfoHeader.height;
}

uint16_t Bitmap::BitCount() {
    return m_header.bitmapInfoHeader.bitcount;
}

uint32_t Bitmap::SizeImage() {
    return m_header.bitmapInfoHeader.sizeimage;
}

uint8_t* Bitmap::Data() {
    return m_data;
}
