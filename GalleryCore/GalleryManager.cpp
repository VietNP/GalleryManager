#include "GalleryManager.h"
#include "OpenCVImageProcessor.h"
#include <opencv2/opencv.hpp>

void GalleryManager::executeCommand(std::shared_ptr<ImageCommand> cmd) {
    if (!cmd) return;
    cmd->execute();
    undoStack.push(cmd);
    while (!redoStack.empty()) redoStack.pop();

    // Ghi log vào Database khi một Action được thực thi
    m_dbRepo.logAction(cmd->getName(), cmd->getPayloadRedo(), cmd->getPayloadUndo());
}

bool GalleryManager::undo() {
    if (undoStack.empty()) return false;
    auto cmd = undoStack.top();
    undoStack.pop();
    cmd->undo();
    redoStack.push(cmd);

    // Ghi log trạng thái UNDO vào Database
    m_dbRepo.logAction("UNDO_" + cmd->getName(), cmd->getPayloadUndo(), cmd->getPayloadRedo());
    return true;
}

bool GalleryManager::redo() {
    if (redoStack.empty()) return false;
    auto cmd = redoStack.top();
    redoStack.pop();
    cmd->execute();
    undoStack.push(cmd);

    // Ghi log trạng thái REDO vào Database
    m_dbRepo.logAction("REDO_" + cmd->getName(), cmd->getPayloadRedo(), cmd->getPayloadUndo());
    return true;
}

void GalleryManager::addImage(const Image& img) {
    Image newImg = img;
    if (newImg.pHash == 0 && !newImg.filepath.empty()) {
        newImg.pHash = OpenCVImageProcessor::computePHash(newImg.filepath);
    }
    images.push_back(newImg);
}

void GalleryManager::editImage(int id, const Image& newImgData) {
    for (auto& img : images) {
        if (img.id == id) {
            img = newImgData;
            break;
        }
    }
}

void GalleryManager::deleteImage(int id) {
    for (auto it = images.begin(); it != images.end(); ++it) {
        if (it->id == id) {
            images.erase(it);
            break;
        }
    }
}

RawImageData GalleryManager::loadImageData(const std::string& path) {
    RawImageData result;
    cv::Mat mat = cv::imread(path);
    if (mat.empty()) {
        result.success = false;
        return result;
    }

    cv::Mat rgb;
    if (mat.channels() == 3) {
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
    }
    else if (mat.channels() == 1) {
        cv::cvtColor(mat, rgb, cv::COLOR_GRAY2RGB);
    }
    else {
        result.success = false;
        return result;
    }

    result.width = rgb.cols;
    result.height = rgb.rows;
    result.channels = 3;
    result.pixelBuffer.assign(rgb.data, rgb.data + (rgb.total() * rgb.elemSize()));
    result.success = true;

    return result;
}

int GalleryManager::compareImages(int id1, int id2) {
    const Image* img1 = nullptr;
    const Image* img2 = nullptr;

    for (const auto& img : images) {
        if (img.id == id1) img1 = &img;
        if (img.id == id2) img2 = &img;
    }

    if (!img1 || !img2) return -1;

    return OpenCVImageProcessor::hammingDistance(img1->pHash, img2->pHash);
}

std::vector<DuplicatePair> GalleryManager::findDuplicates(int threshold) {
    std::vector<DuplicatePair> duplicates;

    size_t n = images.size();
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            if (images[i].pHash == 0 || images[j].pHash == 0) continue;

            int dist = OpenCVImageProcessor::hammingDistance(images[i].pHash, images[j].pHash);

            if (dist <= threshold) {
                duplicates.push_back({ images[i], images[j], dist });
            }
        }
    }

    return duplicates;
}
