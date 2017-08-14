# -*- encoding:utf8 -*-

try:
    from setuptools import setup, find_packages, Extension
except ImportError:
    from distutils.core import setup, find_packages
import sys

if sys.version_info.major == 2:
    print('python 2.x is unsupported, please update to python 3.5+')
    pywrap = './stbfclient/libpywrap.so'
else:
    pywrap = './stbfclient/libpy3wrap.so'

setup(
    name='stbfclient',
    version='1.0',
    author='SeetaTech',
    author_email='anbo.dai@seetatech.com',
    package_data={'': ['*.*']},
    # py_modules=['manage'],
    # scripts=['run_master', 'run_worker', 'deploy'],
    packages=find_packages(),
    data_files=[('stbfclient/lib', [pywrap, './stbfclient/lib/libfastcommon.so', './stbfclient/lib/libfdfsclient.so'])],
    zip_safe=False,
    classifiers=[
        'Programming Language :: Python',
        'Operating System :: Unix',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 2.7',
    ],
)
