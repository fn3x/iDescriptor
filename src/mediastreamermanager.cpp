#include "mediastreamermanager.h"
#include "mediastreamer.h"
#include <QDebug>
#include <QMutexLocker>

MediaStreamerManager::~MediaStreamerManager() { cleanup(); }

MediaStreamerManager *MediaStreamerManager::sharedInstance()
{
    static MediaStreamerManager instance;
    return &instance;
}

QUrl MediaStreamerManager::getStreamUrl(iDescriptorDevice *device,
                                        const QString &filePath)
{

    // Check if we already have a streamer for this file
    auto it = m_streamers.find(filePath);
    if (it != m_streamers.end()) {
        // Verify the streamer is still valid and listening
        if (it->streamer && it->streamer->isListening()) {
            it->refCount++;
            qDebug() << "MediaStreamerManager: Reusing existing streamer for"
                     << filePath << "refCount:" << it->refCount;
            return it->streamer->getUrl();
        } else {
            // Clean up invalid streamer
            qDebug() << "MediaStreamerManager: Cleaning up invalid streamer for"
                     << filePath;
            if (it->streamer) {
                it->streamer->deleteLater();
            }
            m_streamers.erase(it);
        }
    }

    // Create new streamer
    auto *streamer = new MediaStreamer(device, filePath, this);
    if (!streamer->isListening()) {
        qWarning() << "MediaStreamerManager: Failed to create streamer for"
                   << filePath;
        streamer->deleteLater();
        return QUrl();
    }

    // Store the streamer info
    StreamerInfo info;
    info.streamer = streamer;
    info.device = device;
    info.refCount = 1;
    m_streamers[filePath] = info;

    // Connect to destruction signal for cleanup
    connect(streamer, &QObject::destroyed, this,
            &MediaStreamerManager::onStreamerDestroyed);

    qDebug() << "MediaStreamerManager: Created new streamer for" << filePath
             << "at" << streamer->getUrl().toString();

    return streamer->getUrl();
}

void MediaStreamerManager::releaseStreamer(const QString &filePath)
{

    auto it = m_streamers.find(filePath);
    if (it != m_streamers.end()) {
        it->refCount--;
        qDebug() << "MediaStreamerManager: Released streamer for" << filePath
                 << "refCount:" << it->refCount;

        // If no more references, mark for cleanup but don't delete immediately
        // This allows for quick reuse if the same file is opened again soon
        if (it->refCount <= 0) {
            qDebug() << "MediaStreamerManager: Streamer for" << filePath
                     << "ready for cleanup";
        }
    }
}

void MediaStreamerManager::cleanup()
{

    auto it = m_streamers.begin();
    while (it != m_streamers.end()) {
        if (it->refCount <= 0) {
            qDebug() << "MediaStreamerManager: Cleaning up streamer for"
                     << it.key();
            if (it->streamer) {
                it->streamer->deleteLater();
            }
            it = m_streamers.erase(it);
        } else {
            ++it;
        }
    }
}

void MediaStreamerManager::onStreamerDestroyed()
{
    // Find and remove the destroyed streamer
    auto it = m_streamers.begin();
    while (it != m_streamers.end()) {
        if (it->streamer == sender()) {
            qDebug() << "MediaStreamerManager: Streamer destroyed for"
                     << it.key();
            it = m_streamers.erase(it);
            break;
        } else {
            ++it;
        }
    }
}