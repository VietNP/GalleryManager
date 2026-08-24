#include "DeleteImageCommand.h"
#include <algorithm>

DeleteImageCommand::DeleteImageCommand(std::vector<Image>& images, int id)
    : ImageCommand(images), m_targetId(id)
{
}

void DeleteImageCommand::execute() {
    auto it = std::find_if(images.begin(), images.end(),
        [this](const Image& img) { return img.id == m_targetId; });

    if (it != images.end()) {
        m_deletedImage = *it; // Lưu trữ dữ liệu ảnh trước khi xóa để hoàn tác
        images.erase(it);
    }
}

void DeleteImageCommand::undo() {
    images.push_back(m_deletedImage); // Khôi phục ảnh trở lại danh sách
}

std::string DeleteImageCommand::getName() const {
    return "DELETE_IMAGE";
}

std::string DeleteImageCommand::getPayloadRedo() const {
    return "{\"id\": " + std::to_string(m_targetId) + "}";
}

std::string DeleteImageCommand::getPayloadUndo() const {
    return "{\"id\": " + std::to_string(m_deletedImage.id) +
        ", \"filepath\": \"" + m_deletedImage.filepath + "\"}";
}