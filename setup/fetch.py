import requests
import io
from zipfile import ZipFile
import subprocess

def setup_dirs():
    subprocess.call('mkdir ./lib/')
    subprocess.call('mkdir ./include/GLFW')


def install_glfw():
    setup_dirs()
    glfw_version = 'glfw-3.3.2.bin.WIN64'

    request = requests.get('https://github.com/glfw/glfw/releases/download/3.3.2/glfw-3.3.2.bin.WIN64.zip')
    glfw_zfile = ZipFile(io.BytesIO(request.content))
    glfw_zfile.extract(glfw_version + '/lib-vc2019/glfw3.lib', './lib/')
    glfw_zfile.extract(glfw_version + '/include/GLFW/glfw3.h', './include/GLFW/')
    glfw_zfile.close()

    subprocess.call('mv ./lib/' + glfw_version + '/lib-vc2019/glfw3.lib ./lib/')
    subprocess.call('rm -r ./lib/' + glfw_version)
    subprocess.call('mv ./include/GLFW/' + glfw_version + '/include/GLFW/glfw3.h ./include/GLFW')
    subprocess.call('rm -r ./include/GLFW/' + glfw_version)


