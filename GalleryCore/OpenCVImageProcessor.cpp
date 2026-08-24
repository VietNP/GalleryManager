#include "OpenCVImageProcessor.h"
#include <opencv2/opencv.hpp>

uint64_t OpenCVImageProcessor::computePHash(const std::string& path) {
    // 1. Đọc ảnh dưới dạng ảnh xám (Grayscale)
    cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);
    if (img.empty()) return 0;

    // 2. Co dãn ảnh về kích thước cố định 32x32
    cv::Mat resized;
    cv::resize(img, resized, cv::Size(32, 32));

    // 3. Chuyển kiểu dữ liệu sang float để thực hiện phép biến đổi DCT
    cv::Mat floatImg;
    resized.convertTo(floatImg, CV_32F);

    // 4. Thực hiện biến đổi Cosine rời rạc (DCT)
    cv::Mat dctImg;
    cv::dct(floatImg, dctImg);

    // 5. Cắt lấy ma trận 8x8 ở góc trên bên trái (chứa thông tin tần số thấp)
    cv::Mat top8x8 = dctImg(cv::Rect(0, 0, 8, 8));

    // 6. Tính giá trị trung bình các điểm ảnh trong vùng 8x8
    double sum = 0.0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            sum += top8x8.at<float>(r, c);
        }
    }
    double mean = sum / 64.0;

    // 7. Tạo chuỗi hash 64-bit dựa trên so sánh từng pixel với giá trị trung bình
    uint64_t hash = 0;
    int bitIndex = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (top8x8.at<float>(r, c) > mean) {
                hash |= (1ULL << bitIndex);
            }
            bitIndex++;
        }
    }
    return hash;
}

int OpenCVImageProcessor::hammingDistance(uint64_t hash1, uint64_t hash2) {
    uint64_t x = hash1 ^ hash2;
    int dist = 0;
    while (x > 0) {
        dist += (x & 1);
        x >>= 1;
    }
    return dist;
}