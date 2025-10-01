#ifndef CABLEINFOWIDGET_H
#define CABLEINFOWIDGET_H

#include "iDescriptor.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <plist/plist.h>

class CableInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CableInfoWidget(iDescriptorDevice *device,
                             QWidget *parent = nullptr);

private slots:
    void initCableInfo();

private:
    void setupUI();
    void analyzeCableInfo();
    void updateUI();
    void createInfoRow(QGridLayout *layout, int row, const QString &label,
                       const QString &value, const QString &style = "");

    // Cable information structure
    struct CableInfo {
        bool isConnected = false;
        bool isGenuine = false;
        bool isTypeC = false;
        QString manufacturer;
        QString modelNumber;
        QString accessoryName;
        QString serialNumber;
        QString interfaceModuleSerial;
        uint64_t currentLimit = 0;
        uint64_t chargingVoltage = 0;
        QString connectionType;
        QString triStarClass;
        QStringList supportedTransports;
        QStringList activeTransports;
    };

    // UI components
    QVBoxLayout *m_mainLayout;
    QLabel *m_statusLabel;
    QLabel *m_iconLabel;
    QWidget *m_infoWidget;
    QGridLayout *m_infoLayout;

    // Data
    iDescriptorDevice *m_device;
    CableInfo m_cableInfo;
    plist_t m_response;
};

#endif // CABLEINFOWIDGET_H