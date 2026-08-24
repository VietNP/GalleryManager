#pragma once
#include <vector>
#include <stack>
#include <memory>
#include "Image.h"
#include "ImageCommand.h"
#include "DatabaseRepository.h" // Thêm bao hàm repository

class GalleryManager {
private:
    std::vector<Image> images;
    std::stack<std::shared_ptr<ImageCommand>> undoStack;
    std::stack<std::shared_ptr<ImageCommand>> redoStack;

    DatabaseRepository m_dbRepo; // Thêm đối tượng làm việc với Database

public:
    // Khởi tạo connection (User: root, Password: để trống)
    GalleryManager()
        : m_dbRepo("localhost", "root", "", "gallery_db", 3306) {
    }

    void executeCommand(std::shared_ptr<ImageCommand> cmd);
    bool undo();
    bool redo();

    void addImage(const Image& img);
    void editImage(int id, const Image& newImgData);
    void deleteImage(int id);

    RawImageData loadImageData(const std::string& path);
    int compareImages(int id1, int id2);
    std::vector<DuplicatePair> findDuplicates(int threshold);

    // Lấy toàn bộ danh sách ảnh trong bộ nhớ
    const std::vector<Image>& getImages() const { return images; }

    // Lấy thông tin 1 ảnh theo ID (trả về con trỏ nullptr nếu không tìm thấy)
    const Image* getImageById(int id) const {
        for (const auto& img : images) {
            if (img.id == id) return &img;
        }
        return nullptr;
    }

    // Thêm hàm này vào phần public của GalleryManager.h
    std::vector<Image>& getImagesMutable() { return images; }
};