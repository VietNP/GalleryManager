#include "GalleryUI.h"
#include "DeleteImageCommand.h"
#include "AddImageCommand.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QDateTime>
#include <QImage>
#include <QPixmap>
#include <qformlayout.h>

GalleryUI::GalleryUI(QWidget* parent) : QMainWindow(parent) {
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // Cột 1 (Bên trái): Danh sách & Nút bấm
    QVBoxLayout* leftLayout = new QVBoxLayout();
    m_imageList = new QListWidget(this);
    m_btnAdd = new QPushButton("Thêm ảnh...", this);
    m_btnDelete = new QPushButton("Xóa ảnh", this);
    m_btnUndo = new QPushButton("Undo", this);
    m_btnRedo = new QPushButton("Redo", this);

    leftLayout->addWidget(m_imageList);
    leftLayout->addWidget(m_btnAdd);
    leftLayout->addWidget(m_btnDelete);
    leftLayout->addWidget(m_btnUndo);
    leftLayout->addWidget(m_btnRedo);

    // Cột 2 (Giữa): Preview ảnh
    m_imagePreviewLabel = new QLabel("Chọn một ảnh để xem", this);
    m_imagePreviewLabel->setAlignment(Qt::AlignCenter);
    m_imagePreviewLabel->setMinimumSize(350, 350);
    m_imagePreviewLabel->setStyleSheet("border: 1px solid #ccc; background: #f9f9f9;");

    // Cột 3 (Bên phải): Khung Metadata (QFormLayout)
    QVBoxLayout* rightLayout = new QVBoxLayout();
    QLabel* metaHeader = new QLabel("<b>THÔNG TIN METADATA</b>", this);

    QFormLayout* formLayout = new QFormLayout();
    m_lblTitle = new QLabel("-", this);
    m_lblName = new QLabel("-", this);
    m_lblAuthor = new QLabel("-", this);
    m_lblLocation = new QLabel("-", this);
    m_lblDate = new QLabel("-", this);

    formLayout->addRow("Tiêu đề (Title):", m_lblTitle);
    formLayout->addRow("Tên File (Name):", m_lblName);
    formLayout->addRow("Tác giả (Author):", m_lblAuthor);
    formLayout->addRow("Địa điểm (Location):", m_lblLocation);
    formLayout->addRow("Thời gian (Date):", m_lblDate);

    rightLayout->addWidget(metaHeader);
    rightLayout->addLayout(formLayout);
    rightLayout->addStretch();

    // Ghép 3 cột vào Main Layout
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addWidget(m_imagePreviewLabel, 2);
    mainLayout->addLayout(rightLayout, 1);

    setCentralWidget(centralWidget);
    resize(900, 500);

    // Connections
    connect(m_imageList, &QListWidget::currentRowChanged, this, &GalleryUI::handleSelectionChanged);
    connect(m_btnAdd, &QPushButton::clicked, this, &GalleryUI::handleAddImage);
    connect(m_btnDelete, &QPushButton::clicked, this, &GalleryUI::handleDeleteImage);
    connect(m_btnUndo, &QPushButton::clicked, this, &GalleryUI::handleUndo);
    connect(m_btnRedo, &QPushButton::clicked, this, &GalleryUI::handleRedo);

    handleShowImages();
}

void GalleryUI::handleShowImages() {
    m_imageList->clear();
    m_imagePreviewLabel->clear();
    m_imagePreviewLabel->setText("Chọn một ảnh để xem");

    m_lblTitle->setText("-");
    m_lblName->setText("-");
    m_lblAuthor->setText("-");
    m_lblLocation->setText("-");
    m_lblDate->setText("-");

    const auto& images = m_manager.getImages();
    for (const auto& img : images) {
        m_imageList->addItem(QString::fromStdString(img.filename));
    }
}

void GalleryUI::handleSelectionChanged() {
    int currentRow = m_imageList->currentRow();
    const auto& images = m_manager.getImages();

    if (currentRow < 0 || currentRow >= (int)images.size()) return;

    const Image& selectedImg = images[currentRow];

    // 1. Cập nhật thông tin Metadata lên UI
    m_lblTitle->setText(QString::fromStdString(selectedImg.title.empty() ? "(Chưa có)" : selectedImg.title));
    m_lblName->setText(QString::fromStdString(selectedImg.filename));
    m_lblAuthor->setText(QString::fromStdString(selectedImg.author.empty() ? "(Chưa có)" : selectedImg.author));
    m_lblLocation->setText(QString::fromStdString(selectedImg.location.empty() ? "(Chưa có)" : selectedImg.location));
    m_lblDate->setText(QString::fromStdString(selectedImg.dateCreated.empty() ? "(Chưa có)" : selectedImg.dateCreated));

    // 2. Load và render dữ liệu điểm ảnh
    RawImageData raw = m_manager.loadImageData(selectedImg.filepath);
    if (!raw.success) {
        m_imagePreviewLabel->setText("Lỗi: Không thể tải ảnh!");
        return;
    }

    QImage qImg(raw.pixelBuffer.data(), raw.width, raw.height,
        raw.width * raw.channels, QImage::Format_RGB888);

    QPixmap pixmap = QPixmap::fromImage(qImg);
    m_imagePreviewLabel->setPixmap(pixmap.scaled(m_imagePreviewLabel->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation));
}

void GalleryUI::handleAddImage() {
    QString filePath = QFileDialog::getOpenFileName(this, "Chọn ảnh", "", "Image Files (*.png *.jpg *.jpeg *.bmp)");
    if (filePath.isEmpty()) return;

    QFileInfo fileInfo(filePath);

    int nextId = 1;
    const auto& currentImages = m_manager.getImages();
    if (!currentImages.empty()) {
        nextId = currentImages.back().id + 1;
    }

    Image newImg;
    newImg.id = nextId;
    newImg.filename = fileInfo.fileName().toStdString();
    newImg.filepath = filePath.toStdString();

    // Gán dữ liệu mặc định/giả lập metadata (có thể mở rộng popup nhập tay sau)
    newImg.title = fileInfo.baseName().toStdString();
    newImg.author = "Admin";
    newImg.location = "Local Device";
    newImg.dateCreated = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString();

    m_manager.addImage(newImg);

    auto cmd = std::make_shared<AddImageCommand>(m_manager.getImagesMutable(), newImg);
    m_manager.executeCommand(cmd);

    handleShowImages();
    m_imageList->setCurrentRow(m_imageList->count() - 1);
}

void GalleryUI::handleDeleteImage() {
    int currentRow = m_imageList->currentRow();
    if (currentRow < 0) return;

    int targetId = m_manager.getImages()[currentRow].id;
    auto cmd = std::make_shared<DeleteImageCommand>(m_manager.getImagesMutable(), targetId);
    m_manager.executeCommand(cmd);

    handleShowImages();
}

void GalleryUI::handleUndo() {
    if (m_manager.undo()) handleShowImages();
}

void GalleryUI::handleRedo() {
    if (m_manager.redo()) handleShowImages();
}