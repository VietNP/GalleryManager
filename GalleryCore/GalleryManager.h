#ifndef GALLERY_MANAGER_H
#define GALLERY_MANAGER_H

#include "Image.h"
#include "ImageCommand.h"
#include <vector>
#include <stack>
#include <memory>

class GalleryManager {
private:
    std::vector<Image> images;
    std::stack<std::shared_ptr<ImageCommand>> undoStack;
    std::stack<std::shared_ptr<ImageCommand>> redoStack;

public:
    void executeCommand(std::shared_ptr<ImageCommand> cmd);
    bool undo();
    bool redo();

    void addImage(const Image& img); // Thêm hàm này
    void editImage(int id, const Image& newImgData);
    void deleteImage(int id);

    const std::vector<Image>& getImages() const { return images; }
};

#endif // GALLERY_MANAGER_H