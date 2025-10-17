#include "devicemenuwidget.h"
#include "iDescriptor.h"
#include "qprocessindicator.h"
#include <QDebug>
#include <QStackedWidget>
#include <QVBoxLayout>

DeviceMenuWidget::DeviceMenuWidget(iDescriptorDevice *device, QWidget *parent)
    : QWidget{parent}, device(device)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setContentsMargins(0, 0, 0, 0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(stackedWidget);

    QProcessIndicator *loadingIndicator = new QProcessIndicator();
    loadingIndicator->setType(QProcessIndicator::line_rotate);
    loadingIndicator->setFixedSize(64, 32);

    QWidget *loadingWidget = new QWidget();
    QVBoxLayout *loadingLayout = new QVBoxLayout(loadingWidget);
    loadingLayout->setAlignment(Qt::AlignCenter);
    loadingLayout->addWidget(loadingIndicator, 0, Qt::AlignCenter);
    loadingIndicator->start();
    stackedWidget->addWidget(loadingWidget);
    stackedWidget->setCurrentIndex(0);

    QTimer::singleShot(100, this, &DeviceMenuWidget::init);
}

void DeviceMenuWidget::init()
{

    // Create and add widgets to the stacked widget
    m_deviceInfoWidget = new DeviceInfoWidget(device, this);
    m_installedAppsWidget = new InstalledAppsWidget(device, this);
    m_galleryWidget = new GalleryWidget(device, this);
    m_fileExplorerWidget = new FileExplorerWidget(device, this);

    // Set minimum heights
    m_galleryWidget->setMinimumHeight(300);
    m_fileExplorerWidget->setMinimumHeight(300);

    stackedWidget->addWidget(m_deviceInfoWidget);    // Index 0 - Info
    stackedWidget->addWidget(m_installedAppsWidget); // Index 1 - Apps
    stackedWidget->addWidget(m_galleryWidget);       // Index 2 - Gallery
    stackedWidget->addWidget(m_fileExplorerWidget);  // Index 3 - Files

    // Set default to Info tab
    stackedWidget->setCurrentWidget(m_deviceInfoWidget);

    // Connect to current changed signal for lazy loading
    connect(stackedWidget, &QStackedWidget::currentChanged, this,
            [this](int index) {
                if (index == 2) { // Gallery tab
                    qDebug() << "Switched to Gallery tab";
                    m_galleryWidget->load();
                }
            });

    QWidget *loadingWidget = stackedWidget->widget(0);
    stackedWidget->removeWidget(loadingWidget);
    loadingWidget->deleteLater();
}

void DeviceMenuWidget::switchToTab(const QString &tabName)
{
    if (tabName == "Info") {
        stackedWidget->setCurrentWidget(m_deviceInfoWidget);
    } else if (tabName == "Apps") {
        stackedWidget->setCurrentWidget(m_installedAppsWidget);
    } else if (tabName == "Gallery") {
        stackedWidget->setCurrentWidget(m_galleryWidget);
    } else if (tabName == "Files") {
        stackedWidget->setCurrentWidget(m_fileExplorerWidget);
    } else {
        qDebug() << "Tab not found:" << tabName;
    }
}
