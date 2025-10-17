#ifndef DEVDISKIMAGEHELPER_H
#define DEVDISKIMAGEHELPER_H

#include "iDescriptor.h"
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

class QProcessIndicator;

class DevDiskImageHelper : public QDialog
{
    Q_OBJECT
public:
    explicit DevDiskImageHelper(iDescriptorDevice *device,
                                QWidget *parent = nullptr);

    // Start the mounting process
    void start();

signals:
    void mountingCompleted(bool success);
    void downloadStarted();
    void downloadCompleted(bool success);

private slots:
    void checkAndMount();
    void onMountButtonClicked();
    void onRetryButtonClicked();
    void onImageDownloadFinished(const QString &version, bool success,
                                 const QString &errorMessage);

private:
    void setupUI();
    void showStatus(const QString &message, bool isError = false);
    void showMountUI();
    void showRetryUI(const QString &errorMessage);
    void finishWithSuccess();
    void finishWithError(const QString &errorMessage);

    iDescriptorDevice *m_device;

    QLabel *m_statusLabel;
    QProcessIndicator *m_loadingIndicator;
    QPushButton *m_mountButton;
    QPushButton *m_retryButton;
    QPushButton *m_cancelButton;

    bool m_isDownloading;
    bool m_isMounting;
    QString m_downloadingVersion;
};

#endif // DEVDISKIMAGEHELPER_H
