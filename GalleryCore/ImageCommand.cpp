#include "ImageCommand.h"

// ==========================================
// 1. AddImageCommand
// ==========================================
AddImageCommand::AddImageCommand(std::vector<Image>& imgs, const Image& img)
    : ImageCommand(imgs), imageToAdd(img) {
}

void AddImageCommand::execute() {
    images.push_back(imageToAdd);
}

void AddImageCommand::undo() {
    auto it = findImageById(imageToAdd.id);
    if (it != images.end()) {
        images.erase(it);
    }
}

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

// ==========================================
// 3. DeleteImageCommand
// ==========================================
DeleteImageCommand::DeleteImageCommand(std::vector<Image>& imgs, int id)
    : ImageCommand(imgs), imageId(id) {
}

void DeleteImageCommand::execute() {
    auto it = findImageById(imageId);
    if (it != images.end()) {
        deletedIndex = static_cast<int>(std::distance(images.begin(), it));
        deletedImage = *it;
        images.erase(it);
    }
}

void DeleteImageCommand::undo() {
    if (deletedIndex >= 0 && deletedIndex <= static_cast<int>(images.size())) {
        images.insert(images.begin() + deletedIndex, deletedImage);
    }
}