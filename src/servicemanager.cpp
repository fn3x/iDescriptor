#include "servicemanager.h"

afc_error_t
ServiceManager::safeAfcReadDirectory(iDescriptorDevice *device,
                                     const char *path, char ***dirs,
                                     std::optional<afc_client_t> altAfc)
{
    return executeAfcOperation(
        device,
        [path, dirs](afc_client_t client) {
            return afc_read_directory(client, path, dirs);
        },
        altAfc);
}

afc_error_t
ServiceManager::safeAfcGetFileInfo(iDescriptorDevice *device, const char *path,
                                   char ***info,
                                   std::optional<afc_client_t> altAfc)
{
    return executeAfcOperation(
        device,
        [path, info](afc_client_t client) {
            return afc_get_file_info(client, path, info);
        },
        altAfc);
}

afc_error_t ServiceManager::safeAfcFileOpen(iDescriptorDevice *device,
                                            const char *path,
                                            afc_file_mode_t mode,
                                            uint64_t *handle,
                                            std::optional<afc_client_t> altAfc)
{
    return executeAfcOperation(
        device,
        [path, mode, handle](afc_client_t client) {
            return afc_file_open(client, path, mode, handle);
        },
        altAfc);
}

afc_error_t ServiceManager::safeAfcFileRead(iDescriptorDevice *device,
                                            uint64_t handle, char *data,
                                            uint32_t length,
                                            uint32_t *bytes_read,
                                            std::optional<afc_client_t> altAfc)
{
    return executeAfcOperation(
        device,
        [handle, data, length, bytes_read](afc_client_t client) {
            return afc_file_read(client, handle, data, length, bytes_read);
        },
        altAfc);
}

afc_error_t ServiceManager::safeAfcFileWrite(iDescriptorDevice *device,
                                             uint64_t handle, const char *data,
                                             uint32_t length,
                                             uint32_t *bytes_written,
                                             std::optional<afc_client_t> altAfc)
{
    return executeAfcOperation(
        device,
        [handle, data, length, bytes_written](afc_client_t client) {
            return afc_file_write(client, handle, data, length, bytes_written);
        },
        altAfc);
}

afc_error_t ServiceManager::safeAfcFileClose(iDescriptorDevice *device,
                                             uint64_t handle,
                                             std::optional<afc_client_t> altAfc)
{
    return executeAfcOperation(
        device,
        [handle](afc_client_t client) {
            return afc_file_close(client, handle);
        },
        altAfc);
}

afc_error_t ServiceManager::safeAfcFileSeek(iDescriptorDevice *device,
                                            uint64_t handle, int64_t offset,
                                            int whence,
                                            std::optional<afc_client_t> altAfc)
{
    return executeAfcOperation(
        device,
        [handle, offset, whence](afc_client_t client) {
            return afc_file_seek(client, handle, offset, whence);
        },
        altAfc);
}

QByteArray
ServiceManager::safeReadAfcFileToByteArray(iDescriptorDevice *device,
                                           const char *path,
                                           std::optional<afc_client_t> altAfc)
{
    return executeOperation<QByteArray>(
        device,
        [path](afc_client_t client) -> QByteArray {
            return read_afc_file_to_byte_array(client, path);
        },
        altAfc);
}

AFCFileTree ServiceManager::safeGetFileTree(iDescriptorDevice *device,
                                            const std::string &path,
                                            std::optional<afc_client_t> altAfc)
{
    return executeOperation<AFCFileTree>(
        device,
        [path](afc_client_t client) -> AFCFileTree {
            return get_file_tree(client, path.c_str());
        },
        altAfc);
}