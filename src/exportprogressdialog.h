#ifndef EXPORTPROGRESSDIALOG_H
#define EXPORTPROGRESSDIALOG_H

#include <QDateTime>
#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>
#include <QUuid>
#include <QVBoxLayout>

// Forward declarations
class ExportManager;
struct ExportResult;
struct ExportJobSummary;

class ExportProgressDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor with ExportManager parameter since it's owned by manager
    explicit ExportProgressDialog(ExportManager *exportManager,
                                  QWidget *parent = nullptr);

    void showForJob(const QUuid &jobId);

protected:
    void changeEvent(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onExportStarted(const QUuid &jobId, int totalItems,
                         const QString &destinationPath);
    void onExportProgress(const QUuid &jobId, int currentItem, int totalItems,
                          const QString &currentFileName);
    void onFileTransferProgress(const QUuid &jobId, const QString &fileName,
                                qint64 bytesTransferred, qint64 totalFileSize);
    void onItemExported(const QUuid &jobId, const ExportResult &result);
    void onExportFinished(const QUuid &jobId, const ExportJobSummary &summary);
    void onExportCancelled(const QUuid &jobId);
    void onCancelClicked();
    void onOpenDirectoryClicked();
    void updateTransferRate();

private:
    void setupUI();
    void updateColors();
    QString formatFileSize(qint64 bytes) const;
    QString formatTransferRate(qint64 bytesPerSecond) const;
    QString formatTimeRemaining(int secondsRemaining) const;

    ExportManager *m_exportManager;
    QUuid m_currentJobId;

    QVBoxLayout *m_mainLayout;
    QLabel *m_titleLabel;
    QLabel *m_statusLabel;
    QLabel *m_currentFileLabel;
    QProgressBar *m_progressBar;
    QLabel *m_statsLabel;
    QLabel *m_transferRateLabel;
    QLabel *m_timeRemainingLabel;
    QPushButton *m_cancelButton;
    QPushButton *m_closeButton;
    QPushButton *m_openDirButton;

    QString m_destinationPath;
    int m_totalItems = 0;
    int m_completedItems = 0;
    qint64 m_totalBytesTransferred = 0;
    QTimer *m_transferRateTimer;
    qint64 m_lastBytesTransferred = 0;
    QDateTime m_startTime;
    QDateTime m_lastUpdateTime;

    bool m_jobCompleted = false;
    bool m_jobCancelled = false;
};

#endif // EXPORTPROGRESSDIALOG_H
