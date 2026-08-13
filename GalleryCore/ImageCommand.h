#ifndef IMAGE_COMMANDS_H
#define IMAGE_COMMANDS_H

#include "Command.h"
#include "Image.h"
#include <vector>

class EditImageCommand : public Command {
private:
    std::vector<Image>& images;
    int imageId;
    Image oldImage;
    Image newImage;

public:
    EditImageCommand(std::vector<Image>& imgs, int id, const Image& updatedImg);
    void execute() override;
    void undo() override;
};

class DeleteImageCommand : public Command {
private:
    std::vector<Image>& images;
    int imageId;
    Image deletedImage;
    int deletedIndex{ -1 };

public:
    DeleteImageCommand(std::vector<Image>& imgs, int id);
    void execute() override;
    void undo() override;
};

#endif // IMAGE_COMMANDS_H