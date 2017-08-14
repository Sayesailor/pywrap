g++ py3wrap.cc -std=c++11 -fPIC -shared -o libpy3wrap.so -I$PYTHON3_INC -I$FASTDFS_PRO_INC -L/usr/lib64 -L$FASTDFS_PRO_LIB -lfdfsclient
