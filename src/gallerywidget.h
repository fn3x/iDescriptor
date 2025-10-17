#ifndef GALLERYWIDGET_H
#define GALLERYWIDGET_H

#include "iDescriptor.h"
#include <QWidget>

QT_BEGIN_NAMESPACE
class QListView;
class QComboBox;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QStackedWidget;
class QLabel;
class QStandardItem;
QT_END_NAMESPACE

class PhotoModel;
class ExportManager;
class ExportProgressDialog;

class GalleryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GalleryWidget(iDescriptorDevice *device,
                           QWidget *parent = nullptr);
    void load();

private slots:
    void onSortOrderChanged();
    void onFilterChanged();
    void onExportSelected();
    void onExportAll();
    void onAlbumSelected(const QString &albumPath);
    void onBackToAlbums();

private:
    void setupUI();
    void setupControlsLayout();
    void setupAlbumSelectionView();
    void setupPhotoGalleryView();
    void loadAlbumList();
    void setControlsEnabled(bool enabled);
    QString selectExportDirectory();
    QIcon loadAlbumThumbnail(const QString &albumPath);
    void loadAlbumThumbnailAsync(const QString &albumPath, QStandardItem *item);
    void onPhotoContextMenu(const QPoint &pos);

    iDescriptorDevice *m_device;
    bool m_loaded = false;
    QString m_currentAlbumPath;

    // UI components
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_controlsLayout;
    QStackedWidget *m_stackedWidget;

    // Album selection view
    QWidget *m_albumSelectionWidget;
    QListView *m_albumListView;

    // Photo gallery view
    QWidget *m_photoGalleryWidget;
    QListView *m_listView;
    PhotoModel *m_model;

    // Control widgets
    QComboBox *m_sortComboBox;
    QComboBox *m_filterComboBox;
    QPushButton *m_exportSelectedButton;
    QPushButton *m_exportAllButton;
    QPushButton *m_backButton;

    // Export manager
    ExportManager *m_exportManager;
    ExportProgressDialog *m_exportProgressDialog;
};

#endif // GALLERYWIDGET_H
