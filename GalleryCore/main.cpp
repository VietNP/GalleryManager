#include "GalleryManager.h"
#include "DatabaseRepository.h"
#include <iostream>

int main() {
    DatabaseRepository db("localhost", "root", "your_password", "gallery_db");
    GalleryManager gallery;

    // 1. Load dữ liệu từ DB lên RAM
    auto loadedImages = db.loadAll();
    for (const auto& img : loadedImages) {
        gallery.addOrInsert(img);
    }

    // 2. Thao tác trên RAM
    Image newImg{ 101, "test.jpg", "/path/to/test.jpg" };
    gallery.addOrInsert(newImg);

    // 3. Sync ngược lại CSDL khi hoàn tất
    db.saveAll(gallery.getImages());

    std::cout << "Hoan tat khoi tao va cap nhat du lieu!\n";
    return 0;
}