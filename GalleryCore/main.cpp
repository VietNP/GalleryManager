#include "GalleryManager.h"
#include "DatabaseRepository.h"
#include <iostream>

int main() {
    // Thay "your_password" bằng mật khẩu MySQL thực tế trên máy bạn
    DatabaseRepository db("localhost", "root", "your_password", "gallery_db");
    GalleryManager gallery;

    // 1. Load dữ liệu từ DB lên RAM
    auto loadedImages = db.loadAll();
    for (const auto& img : loadedImages) {
        gallery.addImage(img); // Đổi từ addOrInsert() thành addImage()
    }

    // 2. Thao tác trên RAM
    Image newImg{ 101, "test.jpg", "/path/to/test.jpg" };
    gallery.addImage(newImg); // Đổi từ addOrInsert() thành addImage()

    // 3. Sync ngược lại CSDL khi hoàn tất
    db.saveAll(gallery.getImages());

    std::cout << "Hoan tat khoi tao va cap nhat du lieu!\n";
    return 0;
}