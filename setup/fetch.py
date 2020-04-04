import requests
import io
from zipfile import ZipFile
import subprocess


def install_glfw():
    subprocess.call('mkdir ../efgl/vendor/GLFW')
    subprocess.call('mkdir ../efgl/vendor/GLFW/include')

    glfw_version = 'glfw-3.3.2.bin.WIN64'
    lib_dest = '../efgl/vendor/bin'
    header_dest = '../efgl/vendor/GLFW/include'

    request = requests.get('https://github.com/glfw/glfw/releases/download/3.3.2/glfw-3.3.2.bin.WIN64.zip')
    glfw_zfile = ZipFile(io.BytesIO(request.content))
    glfw_zfile.extract(glfw_version + '/lib-vc2019/glfw3.lib', lib_dest)
    glfw_zfile.extract(glfw_version + '/include/GLFW/glfw3.h', )
    glfw_zfile.close()

    subprocess.call('mv ' + lib_dest + glfw_version + '/lib-vc2019/glfw3.lib ' + lib_dest)
    subprocess.call('rm -r ' + lib_dest + glfw_version)
    subprocess.call('mv ' + header_dest + glfw_version + '/include/GLFW/glfw3.h ' + header_dest)
    subprocess.call('rm -r ' + header_dest + glfw_version)


