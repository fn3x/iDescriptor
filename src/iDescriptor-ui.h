#pragma once
#include <QGraphicsView>
#include <QMainWindow>
#include <QMouseEvent>
#include <QWidget>

#define COLOR_GREEN QColor(0, 180, 0)    // Green
#define COLOR_ORANGE QColor(255, 140, 0) // Orange
#define COLOR_RED QColor(255, 0, 0)      // Red

// A custom QGraphicsView that keeps the content fitted with aspect ratio on
// resize
class ResponsiveGraphicsView : public QGraphicsView
{
public:
    ResponsiveGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr)
        : QGraphicsView(scene, parent)
    {
    }

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        if (scene() && !scene()->items().isEmpty()) {
            fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
        }
        QGraphicsView::resizeEvent(event);
    }
};

class ClickableWidget : public QWidget
{
    Q_OBJECT
public:
    using QWidget::QWidget;

signals:
    void clicked();

protected:
    // On mouse release, if the click is inside the widget, emit the clicked
    // signal
    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton &&
            rect().contains(event->pos())) {
            emit clicked();
        }
        QWidget::mouseReleaseEvent(event);
    }
};

#ifdef Q_OS_MAC
void setupMacOSWindow(QMainWindow *window);
#endif

enum class iDescriptorTool {
    Airplayer,
    RealtimeScreen,
    EnterRecoveryMode,
    MountDevImage,
    VirtualLocation,
    Restart,
    Shutdown,
    RecoveryMode,
    QueryMobileGestalt,
    DeveloperDiskImages,
    WirelessFileImport,
    MountIphone,
    CableInfoWidget,
    TouchIdTest,
    FaceIdTest,
    UnmountDevImage,
    Unknown,
    iFuse
};