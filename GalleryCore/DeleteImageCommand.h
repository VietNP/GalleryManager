#ifndef DELETE_IMAGE_COMMAND_H
#define DELETE_IMAGE_COMMAND_H

#include "ImageCommand.h"
#include "Image.h"
#include <vector>

class DeleteImageCommand : public ImageCommand {
private:
    Image m_deletedImage;
    int m_targetId;

public:
    DeleteImageCommand(std::vector<Image>& images, int id);

    void execute() override;
    void undo() override;

    std::string getName() const override;
    std::string getPayloadRedo() const override;
    std::string getPayloadUndo() const override;
};

#endif // DELETE_IMAGE_COMMAND_H