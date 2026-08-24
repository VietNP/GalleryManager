#ifndef ADD_IMAGE_COMMAND_H
#define ADD_IMAGE_COMMAND_H

#include "ImageCommand.h"
#include "Image.h"
#include <vector>

class AddImageCommand : public ImageCommand {
private:
    Image m_addedImage;

public:
    AddImageCommand(std::vector<Image>& images, const Image& img);

    void execute() override;
    void undo() override;

    std::string getName() const override;
    std::string getPayloadRedo() const override;
    std::string getPayloadUndo() const override;
};

#endif // ADD_IMAGE_COMMAND_H#pragma once
