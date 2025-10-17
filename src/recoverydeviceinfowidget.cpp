#include "recoverydeviceinfowidget.h"
#include "iDescriptor-ui.h"
#include "iDescriptor.h"
#include "libirecovery.h"
#include <QDebug>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

RecoveryDeviceInfoWidget::RecoveryDeviceInfoWidget(
    const iDescriptorRecoveryDevice *info, QWidget *parent)
    : QWidget{parent}
{
    ecid = info->ecid; // Assuming ecid is unique for each device

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);

    // Device Information Group
    QGroupBox *deviceInfoGroup = new QGroupBox("Device Information");
    QVBoxLayout *infoLayout = new QVBoxLayout(deviceInfoGroup);
    infoLayout->setSpacing(8);
    infoLayout->setContentsMargins(16, 16, 16, 16);

    // Device name with larger font
    QLabel *deviceNameLabel =
        new QLabel(QString::fromStdString(info->displayName));
    QFont nameFont = deviceNameLabel->font();
    nameFont.setPointSize(nameFont.pointSize() + 2);
    nameFont.setWeight(QFont::DemiBold);
    deviceNameLabel->setFont(nameFont);
    infoLayout->addWidget(deviceNameLabel);

    // Add spacing
    infoLayout->addSpacing(8);

    // Mode info
    QString modeText = QString::fromStdString(parse_recovery_mode(info->mode));
    QLabel *modeLabel = new QLabel("Mode: " + modeText);
    infoLayout->addWidget(modeLabel);

    // ECID info
    QLabel *ecidLabel = new QLabel("ECID: " + QString::number(info->ecid));
    infoLayout->addWidget(ecidLabel);

    // CPID info
    QLabel *cpidLabel = new QLabel("CPID: " + QString::number(info->cpid));
    infoLayout->addWidget(cpidLabel);

    mainLayout->addWidget(deviceInfoGroup);

    // Actions Group
    QGroupBox *actionsGroup = new QGroupBox("Actions");
    QVBoxLayout *actionsLayout = new QVBoxLayout(actionsGroup);
    actionsLayout->setSpacing(12);
    actionsLayout->setContentsMargins(16, 16, 16, 16);

    // Info label
    QLabel *infoLabel = new QLabel(
        "Exit recovery mode and restart the device into normal mode.");
    infoLabel->setWordWrap(true);
    QFont infoFont = infoLabel->font();
    infoFont.setPointSize(infoFont.pointSize() - 1);
    infoLabel->setFont(infoFont);
    QPalette infoPalette = infoLabel->palette();
    infoPalette.setColor(QPalette::WindowText,
                         infoPalette.color(QPalette::WindowText).lighter(120));
    infoLabel->setPalette(infoPalette);
    actionsLayout->addWidget(infoLabel);

    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    QPushButton *exitRecoveryMode = new QPushButton("Exit Recovery Mode");
    exitRecoveryMode->setMinimumWidth(160);
    exitRecoveryMode->setMinimumHeight(32);

    connect(exitRecoveryMode, &QPushButton::clicked, this, [this, info]() {
        irecv_client_t client = NULL;
        irecv_error_t ierr = irecv_open_with_ecid_and_attempts(
            &client, info->ecid, RECOVERY_CLIENT_CONNECTION_TRIES);
        irecv_error_t error = IRECV_E_SUCCESS;
        if (ierr != IRECV_E_SUCCESS) {
            QMessageBox::critical(
                this, "Connection Error",
                QString("Failed to open device with ECID %1:\n%2")
                    .arg(info->ecid)
                    .arg(irecv_strerror(ierr)));
            return;
        }

        if (client == NULL) {
            QMessageBox::critical(this, "Error",
                                  "Client is NULL after successful open");
            return;
        }

        error = irecv_setenv(client, "auto-boot", "true");
        if (error != IRECV_E_SUCCESS) {
            QMessageBox::critical(
                this, "Error",
                QString("Failed to set environment variable 'auto-boot':\n%1")
                    .arg(irecv_strerror(error)));
            irecv_close(client);
            return;
        }

        error = irecv_saveenv(client);
        if (error != IRECV_E_SUCCESS) {
            QMessageBox::critical(
                this, "Error",
                QString("Failed to save environment variables:\n%1")
                    .arg(irecv_strerror(error)));
            irecv_close(client);
            return;
        }

        error = irecv_reboot(client);
        if (error != IRECV_E_SUCCESS) {
            QMessageBox::critical(this, "Error",
                                  QString("Failed to send reboot command:\n%1")
                                      .arg(irecv_strerror(error)));
            irecv_close(client);
            return;
        }

        irecv_close(client);

        auto *msgBox =
            new QMessageBox(QMessageBox::Information, "Success",
                            "Device is exiting recovery mode and restarting...",
                            QMessageBox::Ok, QApplication::activeWindow());
        msgBox->setAttribute(Qt::WA_DeleteOnClose);
        msgBox->open();
    });

    buttonLayout->addWidget(exitRecoveryMode);
    buttonLayout->addStretch();
    actionsLayout->addLayout(buttonLayout);

    mainLayout->addWidget(actionsGroup);
    mainLayout->addStretch();
}
