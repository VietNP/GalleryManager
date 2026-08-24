#ifndef GALLERY_UI_H
#define GALLERY_UI_H

#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "GalleryManager.h"

class GalleryUI : public QMainWindow {
    Q_OBJECT

private:
    GalleryManager m_manager;
    QListWidget* m_imageList;
    QLabel* m_imagePreviewLabel;

    // Các Label hiển thị Metadata
    QLabel* m_lblTitle;
    QLabel* m_lblName;
    QLabel* m_lblAuthor;
    QLabel* m_lblLocation;
    QLabel* m_lblDate;

    QPushButton* m_btnAdd;    // Nút Thêm ảnh
    QPushButton* m_btnDelete;
    QPushButton* m_btnUndo;
    QPushButton* m_btnRedo;

public:
    GalleryUI(QWidget* parent = nullptr);
    ~GalleryUI() = default;

private slots:
    void handleShowImages();
    void handleSelectionChanged();
    void handleAddImage();   // Slot xử lý sự kiện thêm ảnh
    void handleDeleteImage();
    void handleUndo();
    void handleRedo();
};

#endif // GALLERY_UI_H