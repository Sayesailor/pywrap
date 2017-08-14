#ifndef FDFS_EXPORT_FUNC_H_
#define FDFS_EXPORT_FUNC_H_

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) void stbf_test();
__declspec(dllexport) int stbf_setconf(const char* ip);
__declspec(dllexport) int stbf_upload(const char* local_filename, char* strFdfsId);
__declspec(dllexport) int stbf_download(const char* fdfs_id, const char* local_path);
__declspec(dllexport) int stbf_delete(const char* fdfs_id);
__declspec(dllexport) void stbf_release();

#ifdef __cplusplus
}
#endif

#endif