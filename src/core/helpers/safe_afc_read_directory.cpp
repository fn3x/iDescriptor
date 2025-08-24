#include <QDebug>
#include <libimobiledevice/afc.h>
#include <libimobiledevice/libimobiledevice.h>

afc_error_t safe_afc_read_directory(afc_client_t afcClient, idevice_t device,
                                    const char *path, char ***dirs)
{
    afc_error_t res = afc_read_directory(afcClient, path, dirs);
    // maybe the afc client is not valid anymore, so we try to reinitialize it
    if (res != AFC_E_SUCCESS) {
        qDebug() << "AFC read directory error: " << res;
        afc_client_free(afcClient);
        afc_client_new(device, NULL, &afcClient);
        res = afc_read_directory(afcClient, path, dirs);
        if (res != AFC_E_SUCCESS) {
            qDebug() << "Failed to re-read directory after AFC client reset: "
                     << res;
        }
    }

    return res;
}
