#ifndef VIRTUAL_LOCATION_H
#define VIRTUAL_LOCATION_H

#include "iDescriptor.h"
#include <QLineEdit>
#include <QPushButton>
#include <QQuickWidget>
#include <QTimer>
#include <QWidget>

class VirtualLocation : public QWidget
{
    Q_OBJECT

public:
    explicit VirtualLocation(iDescriptorDevice *device,
                             QWidget *parent = nullptr);

signals:
    void locationChanged(double latitude, double longitude);

public slots:
    void updateInputsFromMap(double latitude, double longitude);

private slots:
    void onQuickWidgetStatusChanged(QQuickWidget::Status status);
    void onInputChanged();
    void onMapCenterChanged();
    void onApplyClicked();
    void updateMapFromInputs();

private:
    QQuickWidget *m_quickWidget;
    QLineEdit *m_latitudeEdit;
    QLineEdit *m_longitudeEdit;
    QPushButton *m_applyButton;
    QTimer m_updateTimer;
    bool m_updatingFromInput = false;
    iDescriptorDevice *m_device;
};

#endif // VIRTUAL_LOCATION_H