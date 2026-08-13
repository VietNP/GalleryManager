#include "GalleryManager.h"
#include "ImageCommands.h"

void GalleryManager::executeCommand(std::shared_ptr<Command> cmd) {
    cmd->execute();
    undoStack.push(cmd);

    while (!redoStack.empty()) {
        redoStack.pop();
    }
}

bool GalleryManager::undo() {
    if (undoStack.empty()) return false;

    auto cmd = undoStack.top();
    undoStack.pop();
    cmd->undo();
    redoStack.push(cmd);
    return true;
}

bool GalleryManager::redo() {
    if (redoStack.empty()) return false;

    auto cmd = redoStack.top();
    redoStack.pop();
    cmd->execute();
    undoStack.push(cmd);
    return true;
}

void GalleryManager::editImage(int id, const Image& newImgData) {
    auto cmd = std::make_shared<EditImageCommand>(images, id, newImgData);
    executeCommand(cmd);
}

void GalleryManager::deleteImage(int id) {
    auto cmd = std::make_shared<DeleteImageCommand>(images, id);
    executeCommand(cmd);
}