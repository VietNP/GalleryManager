#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <cstdint>

struct Image {
    int id = 0;
    std::string filename;
    std::string filepath;
    uint64_t pHash = 0;

    // Metadata bổ sung
    std::string title;
    std::string author;
    std::string location;
    std::string dateCreated;
};

// Struct lưu kết quả so sánh 2 ảnh giống nhau
struct DuplicatePair {
    Image image1;
    Image image2;
    int distance; // Khoảng cách Hamming (càng nhỏ càng giống nhau)
};

// Struct lưu buffer dữ liệu ảnh raw để hiển thị giao diện
struct RawImageData {
    int width = 0;
    int height = 0;
    int channels = 0;
    std::vector<unsigned char> pixelBuffer;
    bool success = false;
};

#endif // IMAGE_H