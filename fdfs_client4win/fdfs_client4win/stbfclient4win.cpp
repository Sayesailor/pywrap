#include "stbfclient4win.h"
#include "FastDFS_Client_Win.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void get_fn_ext(const char *fn, unsigned char *buf, int buflen) {
    memset(buf, 0, buflen);
    const char *p = fn + strlen(fn) - 1;
    int fn_ext_len = 0;
    for (; p >= fn; --p) {
        if (*p == '.') break;
        fn_ext_len++;
    }
    if (fn_ext_len < strlen(fn)) {
        memcpy(buf, p + 1, fn_ext_len);
    }
}

__declspec(dllexport) void stbf_test() {}

__declspec(dllexport) int stbf_setconf(const char *ip) {
    ServerAddress addr[2];
    addr[0].nPort = 22122;
    strncpy_s(addr[0].szIP, sizeof(addr[0].szIP), ip, strlen(ip));
    return FDFSC_Initialize(&addr[0], 1, 0);
}

__declspec(dllexport) int stbf_upload(const char *local_filename, TCHAR *strFdfsId) {
    int ret = enumFailure_FDFS;
    FILE *fp = NULL;
    unsigned char *buf = NULL;
    do {
        if (fopen_s(&fp, local_filename, "rb") != 0) {
            ret = enumFileNotExists_FDFS;
            break;
        }

        fseek(fp, 0L, SEEK_END);
        int flen = ftell(fp);
        rewind(fp);
        buf = (unsigned char *)malloc(flen);
        if (buf != NULL && flen == fread(buf, 1, flen, fp)) {
            fclose(fp);
            fp = NULL;

            TCHAR strFileExtName[512];
            get_fn_ext(local_filename, (unsigned char *)strFileExtName, 512);
            TCHAR strGroupName[FDFS_GROUP_NAME_MAX_LEN + 1] = {0};
            TCHAR strRemoteFileName[FDFS_REMOTE_FILE_NAME_MAX_LEN + 1] = {0};

            ret = FDFSC_UploadFile(buf, flen, strFileExtName, strGroupName, strRemoteFileName);
            sprintf_s(strFdfsId, FDFS_GROUP_NAME_MAX_LEN + 1 + FDFS_REMOTE_FILE_NAME_MAX_LEN + 1, "%s/%s", strGroupName, strRemoteFileName);

        } else {
            ret = enumReadFileError;
        }
    } while (0);

    if (buf) {
        free(buf);
        buf = NULL;
    }

    if (fp) {
        fclose(fp);
        fp = NULL;
    }
    return ret;
}

__declspec(dllexport) int stbf_download(const char *fdfs_id, const char *local_path) {
    char *path_name = NULL;
    const char *local_filename = NULL;
    BYTE *buf = NULL;
    FILE *fp = NULL;
    UINT32 ret = enumFailure_FDFS;
    do {
        local_filename = strrchr(fdfs_id, '/');
        if (local_filename != NULL) {
            local_filename++;
        } else {
            local_filename = (char *)(fdfs_id);
        }
        path_name = (char *)malloc(MAX_PATH);
        sprintf_s(path_name, MAX_PATH, "%s/%s", local_path, local_filename);

        UINT64 buf_size = MAXINT;
        buf = (BYTE *)malloc(buf_size);
        if (!buf) {
            ret = enumMallocMemFailed;
            break;
        }
        if ((ret = FDFSC_DownloadFileByID(fdfs_id, buf, &buf_size)) == enumSuccess_FDFS) {
            if (buf_size <= MAXINT) {
                if (fopen_s(&fp, path_name, "wb") != 0) {
                    ret = enumFileNotExists_FDFS;
                    break;
                }
                if (buf_size == fwrite(buf, 1, buf_size, fp)) {
                    ret = enumSuccess_FDFS;
                    break;
                }
            }
            ret = enumWriteFileError;
        }
    } while (0);

    if (fp) fclose(fp);
    if (buf) free(buf);
    if (path_name) free(path_name);

    return ret;
}

__declspec(dllexport) int stbf_delete(const char *fdfs_id) { return 0; }

__declspec(dllexport) void stbf_release() { FDFSC_UnInitialize(); }
