#ifndef OPENCV_IMAGE_PROCESSOR_H
#define OPENCV_IMAGE_PROCESSOR_H

// Tự động khai báo liên kết với các module OpenCV của vcpkg
#ifdef _DEBUG
#pragma comment(lib, "opencv_core4d.lib")
#pragma comment(lib, "opencv_imgproc4d.lib")
#pragma comment(lib, "opencv_imgcodecs4d.lib")
#else
#pragma comment(lib, "opencv_core4.lib")
#pragma comment(lib, "opencv_imgproc4.lib")
#pragma comment(lib, "opencv_imgcodecs4.lib")
#endif

#include <string>
#include <cstdint>

class OpenCVImageProcessor {
public:
    // Tính mã pHash 64-bit bằng thuật toán Discrete Cosine Transform (DCT)
    static uint64_t computePHash(const std::string& path);

    // Tính khoảng cách Hamming giữa 2 mã pHash (trả về 0 nếu 2 ảnh giống hệt nhau)
    static int hammingDistance(uint64_t hash1, uint64_t hash2);
};

#endif // OPENCV_IMAGE_PROCESSOR_H