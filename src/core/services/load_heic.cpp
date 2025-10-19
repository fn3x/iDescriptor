#include "../../iDescriptor.h"
#include <QByteArray>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <libheif/heif.h>

QPixmap load_heic(const QByteArray &imageData)
{
    heif_context *ctx = heif_context_alloc();
    if (!ctx) {
        qWarning() << "Failed to allocate heif_context";
        return QPixmap();
    }

    heif_error err = heif_context_read_from_memory(ctx, imageData.constData(),
                                                   imageData.size(), nullptr);
    if (err.code != heif_error_Ok) {
        qWarning() << "Failed to read HEIC from memory:" << err.message;
        heif_context_free(ctx);
        return QPixmap();
    }

    heif_image_handle *handle;
    err = heif_context_get_primary_image_handle(ctx, &handle);
    if (err.code != heif_error_Ok) {
        qWarning() << "Failed to get primary image handle:" << err.message;
        heif_context_free(ctx);
        return QPixmap();
    }

    heif_image *img;
    err = heif_decode_image(handle, &img, heif_colorspace_RGB,
                            heif_chroma_interleaved_RGB, nullptr);
    if (err.code != heif_error_Ok) {
        qWarning() << "Failed to decode HEIC image:" << err.message;
        heif_image_handle_release(handle);
        heif_context_free(ctx);
        return QPixmap();
    }

    int width = heif_image_get_width(img, heif_channel_interleaved);
    int height = heif_image_get_height(img, heif_channel_interleaved);
    int stride;
    /*
     FIXME: use heif_image_get_plane_readonly2 in future, on ubuntu 24 it's not
     available yet
    */
    const uint8_t *data =
        heif_image_get_plane_readonly(img, heif_channel_interleaved, &stride);

    if (!data) {
        qWarning() << "Failed to get image plane data";
        heif_image_release(img);
        heif_image_handle_release(handle);
        heif_context_free(ctx);
        return QPixmap();
    }

    QImage qimg(data, width, height, stride, QImage::Format_RGB888);
    QPixmap result = QPixmap::fromImage(qimg);

    heif_image_release(img);
    heif_image_handle_release(handle);
    heif_context_free(ctx);

    return result;
}
