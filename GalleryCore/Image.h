#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <cstdint>

struct Image {
    int id{ 0 };
    std::string filename;
    std::string filepath;
    std::string hash;        // FNV-1a hoặc Binary Hash
    uint64_t pHash{ 0 };       // Perceptual Hash (OpenCV)
};

#endif // IMAGE_H