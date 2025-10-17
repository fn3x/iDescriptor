#ifndef DEVICEMENUWIDGET_H
#define DEVICEMENUWIDGET_H
#include "deviceinfowidget.h"
#include "fileexplorerwidget.h"
#include "gallerywidget.h"
#include "iDescriptor.h"
#include "installedappswidget.h"
#include <QStackedWidget>
#include <QWidget>

class DeviceMenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceMenuWidget(iDescriptorDevice *device,
                              QWidget *parent = nullptr);
    void switchToTab(const QString &tabName);
    void init();
    // ~DeviceMenuWidget();
private:
    QStackedWidget *stackedWidget; // Pointer to the stacked widget
    iDescriptorDevice *device;     // Pointer to the iDescriptor device
    DeviceInfoWidget *m_deviceInfoWidget;
    InstalledAppsWidget *m_installedAppsWidget;
    GalleryWidget *m_galleryWidget;
    FileExplorerWidget *m_fileExplorerWidget;
signals:
};

#endif // DEVICEMENUWIDGET_H
