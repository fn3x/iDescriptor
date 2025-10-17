#ifndef EXPORTMANAGER_H
#define EXPORTMANAGER_H

#include "iDescriptor.h"
#include <QFuture>
#include <QFutureWatcher>
#include <QMap>
#include <QMutex>
#include <QObject>
#include <QString>
#include <QUuid>
#include <atomic>
#include <memory>
#include <optional>

// Forward declaration
class ExportProgressDialog;

struct ExportItem {
    QString sourcePathOnDevice;
    QString suggestedFileName;

    ExportItem() = default;
    ExportItem(const QString &sourcePath, const QString &fileName)
        : sourcePathOnDevice(sourcePath), suggestedFileName(fileName)
    {
    }
};

struct ExportResult {
    QString sourceFilePath;
    QString outputFilePath;
    bool success = false;
    QString errorMessage;
    qint64 bytesTransferred = 0;
};

struct ExportJobSummary {
    QUuid jobId;
    int totalItems = 0;
    int successfulItems = 0;
    int failedItems = 0;
    qint64 totalBytesTransferred = 0;
    QString destinationPath;
    bool wasCancelled = false;
};

class ExportManager : public QObject
{
    Q_OBJECT

public:
    // Singleton access method
    static ExportManager *sharedInstance();

    // Delete copy and assignment operators
    ExportManager(const ExportManager &) = delete;
    ExportManager &operator=(const ExportManager &) = delete;

    QUuid startExport(iDescriptorDevice *device, const QList<ExportItem> &items,
                      const QString &destinationPath,
                      std::optional<afc_client_t> altAfc = std::nullopt);

    void cancelExport(const QUuid &jobId);

    bool isExporting() const;

    bool isJobRunning(const QUuid &jobId) const;

signals:

    void exportStarted(const QUuid &jobId, int totalItems,
                       const QString &destinationPath);

    void exportProgress(const QUuid &jobId, int currentItem, int totalItems,
                        const QString &currentFileName);

    void fileTransferProgress(const QUuid &jobId, const QString &fileName,
                              qint64 bytesTransferred, qint64 totalFileSize);

    void itemExported(const QUuid &jobId, const ExportResult &result);

    void exportFinished(const QUuid &jobId, const ExportJobSummary &summary);

    void exportCancelled(const QUuid &jobId);

private:
    // Private constructor for singleton pattern
    explicit ExportManager(QObject *parent = nullptr);
    ~ExportManager();

    struct ExportJob {
        QUuid jobId;
        iDescriptorDevice *device = nullptr;
        QList<ExportItem> items;
        QString destinationPath;
        std::optional<afc_client_t> altAfc;
        std::atomic<bool> cancelRequested{false};
        QFuture<void> future;
        QFutureWatcher<void> *watcher = nullptr;
    };

    void executeExportJob(ExportJob *job);

    ExportResult exportSingleItem(iDescriptorDevice *device,
                                  const ExportItem &item,
                                  const QString &destinationDir,
                                  std::optional<afc_client_t> altAfc,
                                  std::atomic<bool> &cancelRequested,
                                  const QUuid &jobId);

    QString generateUniqueOutputPath(const QString &basePath) const;

    QString extractFileName(const QString &devicePath) const;

    void cleanupJob(const QUuid &jobId);

    // Thread-safe storage for active jobs
    mutable QMutex m_jobsMutex;
    QMap<QUuid, ExportJob *> m_activeJobs;

    // Manager owns the dialog
    ExportProgressDialog *m_exportProgressDialog;
};

#endif // EXPORTMANAGER_H