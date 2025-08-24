#ifndef TOOLBOXWIDGET_H
#define TOOLBOXWIDGET_H

#include "devdiskimageswidget.h"
#include "iDescriptor.h"
#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

class ToolboxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ToolboxWidget(QWidget *parent = nullptr);

private slots:
    void onDeviceAdded();
    void onDeviceRemoved();
    void onDeviceSelectionChanged();
    void onToolboxClicked(const QString &toolName);

private:
    void setupUI();
    void updateDeviceList();
    void updateToolboxStates();
    QWidget *createToolbox(const QString &title, const QString &description,
                           const QString &iconName, bool requiresDevice);
    bool eventFilter(QObject *obj, QEvent *event) override;
    QComboBox *m_deviceCombo;
    QLabel *m_deviceLabel;
    QScrollArea *m_scrollArea;
    QWidget *m_contentWidget;
    QGridLayout *m_gridLayout;
    QList<QWidget *> m_toolboxes;
    QList<bool> m_requiresDevice;
    iDescriptorDevice *m_currentDevice;
    std::string m_uuid;
    DevDiskImagesWidget *m_devDiskImagesWidget;

signals:
};

#endif // TOOLBOXWIDGET_H
