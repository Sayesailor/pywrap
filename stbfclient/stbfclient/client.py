import os
import sys 

class FDFSException(Exception):
    pass

if sys.version_info.major == 2:
    from .libpywrap import *
else:
    from .libpy3wrap import *

def up_file(local_file_path):
    fdfs_new_name = stbf_up(local_file_path)
    if isinstance(fdfs_new_name, bool) and not fdfs_new_name:
        raise FDFSException("up file failed %s" % local_file_path)
    if isinstance(fdfs_new_name, str):
        return fdfs_new_name
    return fdfs_new_name.decode()

def down_file(fdfs_file_name, save_path):
    stbf_down(fdfs_file_name, save_path)
    return os.path.join(save_path, os.path.basename(fdfs_file_name))

def down_file_and_makedir(fdfs_file_name, save_path):
    save_path_new = os.path.join(save_path, os.path.dirname(fdfs_file_name))
    if not os.path.exists(save_path_new):
        print('test')
        os.makedirs(save_path_new)
    print(save_path_new)
    down_file(fdfs_file_name, save_path_new)
    return os.path.join(save_path, fdfs_file_name)

def set_conf(path):
    stbf_stcnf(path)
