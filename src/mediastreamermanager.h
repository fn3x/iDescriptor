#ifndef MEDIASTREAMERMANAGER_H
#define MEDIASTREAMERMANAGER_H

#include "iDescriptor.h"
#include "mediastreamer.h"
#include <QMap>
#include <QMutex>
#include <QObject>
#include <QUrl>

/**
 * @brief Singleton manager for MediaStreamer instances
 *
 * This class manages MediaStreamer instances to avoid creating multiple
 * streamers for the same file. It automatically cleans up unused streamers
 * and provides thread-safe access.
 */
class MediaStreamerManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Get the singleton instance
     * @return The MediaStreamerManager instance
     */
    static MediaStreamerManager *sharedInstance();

    /**
     * @brief Get or create a streamer for the specified file
     * @param device The iOS device
     * @param filePath The file path on the device
     * @return URL to stream the file, or empty URL if failed
     */
    QUrl getStreamUrl(iDescriptorDevice *device, const QString &filePath);

    /**
     * @brief Release a streamer for the specified file
     * @param filePath The file path to release
     */
    void releaseStreamer(const QString &filePath);

    /**
     * @brief Clean up all inactive streamers
     */
    void cleanup();

private:
    ~MediaStreamerManager();

private slots:
    void onStreamerDestroyed();

private:
    struct StreamerInfo {
        MediaStreamer *streamer;
        iDescriptorDevice *device;
        int refCount;
    };

    static MediaStreamerManager *s_instance;
    static QMutex s_instanceMutex;

    QMap<QString, StreamerInfo> m_streamers;
    QMutex m_streamersMutex;
};

#endif // MEDIASTREAMERMANAGER_H