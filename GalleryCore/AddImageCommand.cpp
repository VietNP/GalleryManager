#include "AddImageCommand.h"
#include <algorithm>

AddImageCommand::AddImageCommand(std::vector<Image>& images, const Image& img)
    : ImageCommand(images), m_addedImage(img)
{
}

void AddImageCommand::execute() {
    images.push_back(m_addedImage);
}

void AddImageCommand::undo() {
    auto it = std::find_if(images.begin(), images.end(),
        [this](const Image& img) { return img.id == m_addedImage.id; });

    if (it != images.end()) {
        images.erase(it);
    }
}

std::string AddImageCommand::getName() const {
    return "ADD_IMAGE";
}

std::string AddImageCommand::getPayloadRedo() const {
    return "{\"id\": " + std::to_string(m_addedImage.id) +
        ", \"filepath\": \"" + m_addedImage.filepath + "\"}";
}

std::string AddImageCommand::getPayloadUndo() const {
    return "{\"id\": " + std::to_string(m_addedImage.id) + "}";
}