#ifndef GALLERY_MANAGER_H
#define GALLERY_MANAGER_H

#include "Image.h"
#include "Command.h"
#include <vector>
#include <stack>
#include <memory>

class GalleryManager {
private:
    std::vector<Image> images;
    std::stack<std::shared_ptr<Command>> undoStack;
    std::stack<std::shared_ptr<Command>> redoStack;

public:
    void executeCommand(std::shared_ptr<Command> cmd);
    bool undo();
    bool redo();

    void editImage(int id, const Image& newImgData);
    void deleteImage(int id);

    const std::vector<Image>& getImages() const { return images; }
};

#endif // GALLERY_MANAGER_H