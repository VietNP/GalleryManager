#include <QApplication>
#include "GalleryUI.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    GalleryUI w;
    w.show(); // Hiển thị cửa sổ GUI
    return app.exec();
}