#include "ImageCommand.h"

// ==========================================
// 2. EditImageCommand
// ==========================================
EditImageCommand::EditImageCommand(std::vector<Image>& imgs, int id, const Image& updatedImg)
    : ImageCommand(imgs), imageId(id), newImage(updatedImg) {
}

void EditImageCommand::execute() {
    auto it = findImageById(imageId);
    if (it != images.end()) {
        oldImage = *it;
        *it = newImage;
    }
}

void EditImageCommand::undo() {
    auto it = findImageById(imageId);
    if (it != images.end()) {
        *it = oldImage;
    }
}