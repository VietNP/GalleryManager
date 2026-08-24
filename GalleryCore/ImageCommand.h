#ifndef IMAGE_COMMAND_H
#define IMAGE_COMMAND_H

#include "Image.h"
#include <vector>
#include <algorithm>

// ==========================================
// LỚP CHA ABSTRACT (Base Command)
// ==========================================

class ImageCommand {
protected:
    std::vector<Image>& images; // Dùng chung cho tất cả lệnh

    // Hàm phụ dùng chung để tìm ảnh theo ID
    auto findImageById(int id) {
        return std::find_if(images.begin(), images.end(), [id](const Image& img) {
            return img.id == id;
            });
    }

public:
    ImageCommand(std::vector<Image>& imgs) : images(imgs) {}
    virtual ~ImageCommand() = default;

    virtual void execute() = 0;
    virtual void undo() = 0;

    virtual std::string getName() const = 0;
    virtual std::string getPayloadRedo() const = 0;
    virtual std::string getPayloadUndo() const = 0;
};

// ==========================================
// CÁC LỚP CON CỤ THỂ (Concrete Commands)
// ==========================================


// 2. Sửa ảnh
class EditImageCommand : public ImageCommand {
private:
    int imageId;
    Image oldImage;
    Image newImage;

public:
    EditImageCommand(std::vector<Image>& imgs, int id, const Image& updatedImg);
    void execute() override;
    void undo() override;
};

#endif // IMAGE_COMMAND_H