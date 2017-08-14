g++ pywrap.cc -std=c++11 -fPIC -shared -o libpywrap.so -I$PYTHON2_INC -I$FASTDFS_PRO_INC -L/usr/lib64 -L$FASTDFS_PRO_LIB -lfdfsclient
