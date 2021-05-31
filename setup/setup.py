"""
runs premake based on which OS you're on, just to reduce headache
if you have a different setup than these commands assume, try running 
../vendor/bin/linux/premake5 --help
"""
import os
import platform

path_prefix = '../vendor/bin/'
cmd = ''

if platform.system() == 'Linux':
    cmd = '{}linux/premake5 --os=linux gmake'.format(path_prefix)
elif platform.system() == 'Windows':
    cmd = '{}windows/premake5 --os=windows vs2019'.format(path_prefix)

os.system(cmd) 
