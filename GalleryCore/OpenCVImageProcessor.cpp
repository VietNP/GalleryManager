#include "OpenCVImageProcessor.h"
#include <opencv2/opencv.hpp>
#include <iostream>

uint64_t OpenCVImageProcessor::calculatePHash(const std::string& filepath) {
    cv::Mat img = cv::imread(filepath, cv::IMREAD_GRAYSCALE);
    if (img.empty()) return 0;

    cv::Mat resized;
    cv::resize(img, resized, cv::Size(32, 32));

    cv::Mat floatImg, dctImg;
    resized.convertTo(floatImg, CV_32F);
    cv::dct(floatImg, dctImg);

    cv::Mat top8x8 = dctImg(cv::Rect(0, 0, 8, 8));
    double sum = cv::sum(top8x8)[0] - top8x8.at<float>(0, 0);
    double avg = sum / 63.0;

    uint64_t hash = 0;
    int bitIndex = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (r == 0 && c == 0) continue;
            if (top8x8.at<float>(r, c) > avg) {
                hash |= (1ULL << bitIndex);
            }
            bitIndex++;
        }
    }
    return hash;
}

bool OpenCVImageProcessor::isSimilar(uint64_t hash1, uint64_t hash2, int maxDistance) {
    uint64_t x = hash1 ^ hash2;
    int distance = 0;
    while (x > 0) {
        distance += x & 1;
        x >>= 1;
    }
    return distance <= maxDistance;
}

std::unordered_map<uint64_t, std::vector<Image>> OpenCVImageProcessor::findSimilarGroups(
    const std::vector<Image>& images,
    int maxDistance
) {
    std::unordered_map<uint64_t, std::vector<Image>> groups;
    for (const auto& img : images) {
        uint64_t hash = (img.pHash != 0) ? img.pHash : 0;
        groups[hash].push_back(img);
    }
    return groups;
}