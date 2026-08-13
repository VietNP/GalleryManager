#include "ImageCommands.h"
#include <algorithm>

// --- EditImageCommand ---
EditImageCommand::EditImageCommand(std::vector<Image>& imgs, int id, const Image& updatedImg)
    : images(imgs), imageId(id), newImage(updatedImg) {
}

void EditImageCommand::execute() {
    auto it = std::find_if(images.begin(), images.end(), [this](const Image& img) {
        return img.id == imageId;
        });
    if (it != images.end()) {
        oldImage = *it;
        *it = newImage;
    }
}

void EditImageCommand::undo() {
    auto it = std::find_if(images.begin(), images.end(), [this](const Image& img) {
        return img.id == imageId;
        });
    if (it != images.end()) {
        *it = oldImage;
    }
}

// --- DeleteImageCommand ---
DeleteImageCommand::DeleteImageCommand(std::vector<Image>& imgs, int id)
    : images(imgs), imageId(id) {
}

void DeleteImageCommand::execute() {
    auto it = std::find_if(images.begin(), images.end(), [this](const Image& img) {
        return img.id == imageId;
        });
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