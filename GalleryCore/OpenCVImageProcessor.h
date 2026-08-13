#ifndef OPENCV_IMAGE_PROCESSOR_H
#define OPENCV_IMAGE_PROCESSOR_H

#include "Image.h"
#include <vector>
#include <unordered_map>

class OpenCVImageProcessor {
public:
    uint64_t calculatePHash(const std::string& filepath);
    bool isSimilar(uint64_t hash1, uint64_t hash2, int maxDistance = 5);

    // Tìm các nhóm ảnh tương đồng (trùng tư thế, góc chụp)
    std::unordered_map<uint64_t, std::vector<Image>> findSimilarGroups(
        const std::vector<Image>& images,
        int maxDistance = 5
    );
};

#endif // OPENCV_IMAGE_PROCESSOR_H