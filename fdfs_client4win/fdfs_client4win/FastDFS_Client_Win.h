#ifndef FASTDFS_CLIENT_WIN_H_
#define FASTDFS_CLIENT_WIN_H_

#include <Windows.h>
#include <minwindef.h>
#include <basetsd.h>
#include <crtdefs.h>
#include <TCHAR.h>

#define IP_ADDRESS_SIZE 16
#define FDFS_GROUP_NAME_MAX_LEN 16
#define FDFS_FILE_PREFIX_MAX_LEN 16
#define FDFS_GROUP_NAME_MAX_LEN 16
#define FDFS_STORAGE_ID_MAX_SIZE 16
#define FDFS_REMOTE_FILE_NAME_MAX_LEN 128
#define FDFS_FILE_EXT_NAME_MAX_LEN 6
#define FDFS_MAX_SERVERS_EACH_GROUP 32
#define FDFS_MAX_GROUPS 512
#define FDFS_PROTO_PKG_LEN_SIZE 8
#define FDFS_VERSION_SIZE 6
#define FDFS_DOMAIN_NAME_MAX_SIZE 128

typedef enum enumTemplateResult_FDFS {
    enumSuccess_FDFS = 0,            // successed
    enumFailure_FDFS = 1,            // failed
    enumNotInit_FDFS = 2,            // not init
    enumInvalidParameters_FDFS = 3,  // param error
    enumNetworkError_FDFS = 4,       // network error
    enumFileNotExists_FDFS = 5,      // file not exist
    enumNoEnoughSpace_FDFS = 6,      // no space
    enumReadFileError = 7,           // read file failed
    enumWriteFileError = 8,          // write file failed
    enumMallocMemFailed = 9          // no memory
};

typedef struct __ServerAddress__ {
    TCHAR szIP[16];
    UINT32 nPort;
} ServerAddress;

typedef struct {
    TCHAR szGroupName[FDFS_GROUP_NAME_MAX_LEN + 8];  // for 8 bytes alignment
    UINT64 nTotalMB;                                 // total disk storage in MB
    UINT64 nFreeMB;                                  // free disk storage in MB
    UINT64 nTrunkFreeMB;                             // trunk free disk storage in MB
    UINT64 nCount;                                   // server count
    UINT64 nStoragePort;                             // storage server port
    UINT64 nStorageHttpPort;                         // storage server http port
    UINT64 nActiveCount;                             // active server count
    UINT64 nCurrentWriteServer;                      // current server index to upload file
    UINT64 nStorePathCount;                          // store base path count of each storage server
    UINT64 nSubdirCountPerPath;
    UINT64 nCurrentTrunkFileID;  // current trunk file id
} FDFSGroupStat;

typedef struct {
    UINT64 nTotalUploadCount;
    UINT64 nSuccessUploadCount;
    UINT64 nTotalAppendCount;
    UINT64 nSuccessAppendCount;
    UINT64 nTotalModifyCount;
    UINT64 nSuccessModifyCount;
    UINT64 nTotalTruncateCount;
    UINT64 nSuccessTruncateCount;
    UINT64 nTotalSetMetaCount;
    UINT64 nSuccessSetMetaCount;
    UINT64 nTotalDeleteCount;
    UINT64 nSuccessDeleteCount;
    UINT64 nTotalDownloadCount;
    UINT64 nSuccessDownloadCount;
    UINT64 nTotalGetMetaCount;
    UINT64 nSuccessGetMetaCount;
    UINT64 nTotalCreateLinkCount;
    UINT64 nSuccessCreateLinkCount;
    UINT64 nTotalDeleteLinkCount;
    UINT64 nSuccessDeleteLinkCount;
    UINT64 nTotalUploadBytes;
    UINT64 nSuccessUploadBytes;
    UINT64 nTotalAppendBytes;
    UINT64 nSuccessAppendBytes;
    UINT64 nTotalModifyBytes;
    UINT64 nSuccessModifyBytes;
    UINT64 nTotalDownloadBytes;
    UINT64 nSuccessDownloadBytes;
    UINT64 nTotalSyncInBytes;
    UINT64 nSuccessSyncInBytes;
    UINT64 nTotalSyncOutBytes;
    UINT64 nSuccessSyncOutBytes;
    UINT64 nTotalFileOpenCount;
    UINT64 nSuccessFileOpenCount;
    UINT64 nTotalFileReadCount;
    UINT64 nSuccessFileReadCount;
    UINT64 nTotalFileWriteCount;
    UINT64 nSuccessFileWriteCount;

    /* last update timestamp as source server,
       current server' timestamp
    */
    time_t lastSourceUpdate;

    /* last update timestamp as dest server,
       current server' timestamp
    */
    time_t lastSyncUpdate;

    /* last syned timestamp,
       source server's timestamp
    */
    time_t lastSyncedTimestamp;

    /* last heart beat time */
    time_t lastHeartBeatTime;

    struct {
        UINT32 nAllocCount;
        volatile UINT32 nCurrentCount;
        UINT32 nMaxCount;
    } connection;
} FDFSStorageInfo;

typedef struct {
    BOOL bIfTrunkServer;
    BYTE byStatus;
    TCHAR szID[FDFS_STORAGE_ID_MAX_SIZE];
    TCHAR szIpAddr[IP_ADDRESS_SIZE];
    TCHAR szSrcId[FDFS_STORAGE_ID_MAX_SIZE];
    TCHAR szDomainName[FDFS_DOMAIN_NAME_MAX_SIZE];
    TCHAR szVersion[FDFS_VERSION_SIZE];
    UINT64 nTotalMb;
    UINT64 nFreeMb;
    UINT64 nUploadPriority;
    time_t joinTime;
    time_t upTime;
    UINT64 nStorePathCount;
    UINT64 nSubdirCountPerPath;
    UINT64 nStoragePort;
    UINT64 nStorageHttpPort;
    UINT64 nCurrentWritePath;
    FDFSStorageInfo stat;
} FDFSStorageStat;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * init FastDFS client
 *
 * param:
 * pAddr		[in]	tracker server addr
 * nAddrCount	[in]	tracker server addr length
 *
 * return:
 * enumSuccess_FDFS:successed
 * enumInvalidParameters_FDFS:invalid parameter
 * enumFailure_FDFS:init failed
 */
UINT32 __stdcall FDFSC_Initialize(ServerAddress *pAddr, UINT32 nAddrCount, UINT32 nLogLevel);

/**
 * uninitialize FastDFS client
 *
 * param:none
 *
 * return:none
 */
void __stdcall FDFSC_UnInitialize();

/**
 * upload file
 *
 * param:
 * pbyFileBuff			[in]	file data buffer
 * nFileSize			[in]	file data size
 * pszFileExtName		[in]	file ext name
 * pszGroupName			[out]   group name
 * pszRemoteFileName	[out]	file name
 *
 * return:
 * enumSuccess_FDFS:successed
 * enumInvalidParameters_FDFS:invalid param
 * enumFailure_FDFS:failed
 */
UINT32 __stdcall FDFSC_UploadFile(const BYTE *pbyFileBuff, UINT32 nFileSize, const TCHAR *pszFileExtName, TCHAR *pszGroupName, TCHAR *pszRemoteFileName);

/**
 * upload file
 *
 * param:
 * pbyFileBuff			[in]	file data buffer
 * nFileSize			[in]	file data size
 * pszFileExtName		[in]	file ext name
 * pszFileID			[out]	file id
 *
 * return:
 * enumSuccess_FDFS:successed
 * enumInvalidParameters_FDFS:invalid param
 * enumFailure_FDFS:failed
 */
UINT32 __stdcall FDFSC_UploadFileByID(const BYTE *pbyFileBuff, UINT32 nFileSize, const TCHAR *pszFileExtName, TCHAR *pszFileID);

/**
 * upload slave file
 *
 * param:
 * pbyFileBuff			[in]	slave file data buffer
 * nFileSize			[in]	slave file data size
 * pszMasterGroupName	[in]	master group
 * pszMasterFileName	[in]	master file name
 * pszPrefixName		[in]	slave Prefix
 * pszFileExtName		[in]	slave file ext name
 * pszGroupName			[out]   slave file group
 * pszRemoteFileName	[out]	slave file name
 *
 * return:
 * enumSuccess_FDFS:successed
 * enumInvalidParameters_FDFS:invalid param
 * enumFailure_FDFS:failed
 */
UINT32 __stdcall FDFSC_UploadSlaveFile(const BYTE *pbyFileBuff, UINT32 nFileSize, const TCHAR *pszMasterGroupName, const TCHAR *pszMasterFileName,
                                       const TCHAR *pszPrefixName, const TCHAR *pszFileExtName, TCHAR *pszGroupName, TCHAR *pszRemoteFileName);

/**
 * upload slave file
 *
 * param:
 * pbyFileBuff			[in]	slave file data buffer
 * nFileSize			[in]	slave file data size
 * pszMasterGroupName	[in]	master file group
 * pszMasterFileName	[in]	master file name
 * pszPrefixName		[in]	slave file Prefix
 * pszFileExtName		[in]	slave file ext name
 * pszFileID			[out]	slave file id
 *
 * return:
 * enumSuccess_FDFS:successed
 * enumInvalidParameters_FDFS:invalid param
 * enumFailure_FDFS:failed
 */
UINT32 __stdcall FDFSC_UploadSlaveFileByID(const BYTE *pbyFileBuff, UINT32 nFileSize, const TCHAR *pszMasterGroupName, const TCHAR *pszMasterFileName,
                                           const TCHAR *pszPrefixName, const TCHAR *pszFileExtName, TCHAR *pszFileID);

/**
 * download file
 *
 * param:
 * pszGroupName			[in]	group name
 * pszRemoteFileName	[in]	file name
 * pbyFileBuff			[out]	file data buffer
 * nFileSize			[out]	file data size
 *
 * return:
 * enumSuccess_FDFS:successed
 * enumInvalidParameters_FDFS:invalid param
 * enumFailure_FDFS:failed
 */
UINT32 __stdcall FDFSC_DownloadFile(const TCHAR *pszGroupName, const TCHAR *pszRemoteFileName, BYTE *pbyFileBuff, UINT64 *nFileSize);

/**
 * download file
 *
 * param:
 * pszFileID			[in]	file id
 * pbyFileBuff			[out]	file data buffer
 * nFileSize			[out]	file data size
 *
 * return:
 * enumSuccess_FDFS:successed
 * enumInvalidParameters_FDFS:invalid param
 * enumFailure_FDFS:failed
 */
UINT32 __stdcall FDFSC_DownloadFileByID(const TCHAR *pszFileID, BYTE *pbyFileBuff, UINT64 *nFileSize);

/**
 * download file
 *
 * param:
 * pszTrackerIPList		[in]	Tracker ip list
 * pszFileID			[in]	file id
 * pbyFileBuff			[out]	file data buffer
 * nFileSize			[out]	file data size
 *
 * return:
 * enumSuccess_FDFS:successed
 * enumInvalidParameters_FDFS:invalid param
 * enumFailure_FDFS:failed
 *
 * note
 * can be called without initialized
 */
UINT32 __stdcall FDFSC_DownloadFileByIDEx(const TCHAR *pszTrackerIPList, const TCHAR *pszFileID, BYTE *pbyFileBuff, UINT32 *nFileSize);

/**
 * delete file
 *
 * param:
 * pszGroupName			[in]	group
 * pszRemoteFileName	[in]	file name
 *
 * return:
 * enumSuccess_FDFS:successed
 * enumInvalidParameters_FDFS:invalid param
 * enumFailure_FDFS:failed
 */
UINT32 __stdcall FDFSC_DeleteFile(const TCHAR *pszGroupName, const TCHAR *pszRemoteFileName);

/**
 * delete file
 *
 * param:
 * pszFileID			[in]	file id
 *
 * return:
 * enumSuccess_FDFS:successed
 * enumInvalidParameters_FDFS:invalid param
 * enumFailure_FDFS:failed
 */
UINT32 __stdcall FDFSC_DeleteFileByID(const TCHAR *pszFileID);

/**
 * delete file
 *
 * param:
 * pszTrackerIPList		[in]	Tracker ip list
 * pszFileID			[in]	file id
 *
 * return:
 * enumSuccess_FDFS:successed
 * enumInvalidParameters_FDFS:invalid param
 * enumFailure_FDFS:failed
 *
 * note
 * can be called without initialized
 */
UINT32 __stdcall FDFSC_DeleteFileByIDEx(const TCHAR *pszTrackerIPList, const TCHAR *pszFileID);

/**
 * request Group information
 *
 * param:
 * pTrackerAddr			[in]	request Tracker server addr
 * pStat				[in,out]	FDFSGroupStat structure array for save result
 * nLen					[in]	array length,FDFS_MAX_GROUPS
 * pnStatCount			[out]	get count for result of FDFSGroupStat
 *
 * return:
 * enumSuccess_FDFS:successed
 * enumInvalidParameters_FDFS:invalid param
 * enumFailure_FDFS:failed
 * enumNetworkError_FDFS:network error
 * enumNotInit_FDFS:uninitialized
 */
UINT32 __stdcall FDFSC_TrackerListGroups(ServerAddress *pTrackerAddr, FDFSGroupStat *pStat, UINT32 nLen, UINT32 *pnStatCount);

/**
 * request Storage information in Group
 *
 * param:
 * pTrackerAddr			[in]	request Tracker server addr
 * pszGroupName			[in]	group
 * pStat				[in,out]	FDFSGroupStat structure array for save result
 * nLen					[in]	array length,FDFS_MAX_GROUPS
 * pnStatCount			[out]	get count for result of FDFSGroupStat
 *
 * return:
 * enumSuccess_FDFS:successed
 * enumInvalidParameters_FDFS:invalid param
 * enumFailure_FDFS:failed
 * enumNetworkError_FDFS:network error
 * enumNotInit_FDFS:uninitialized
 */
UINT32 __stdcall FDFSC_TrackerListStorages(ServerAddress *pTrackerAddr, TCHAR *pszGroupName, FDFSStorageStat *pStat, UINT32 nLen, UINT32 *pnStatCount);

/**
 * check if same Storage for same group Tracker
 *
 * param:
 * pszTrackerIPList		[in]	Tracker ip list
 *
 * return:
 * enumSuccess_FDFS:same configure
 * enumInvalidParameters_FDFS:invalid param
 * enumFailure_FDFS:failed
 *
 * note
 * can be called without initialized
 */
UINT32 __stdcall FDFSC_CheckConfiguration(const TCHAR *pszTrackerIPList);

#ifdef __cplusplus
}
#endif

#endif