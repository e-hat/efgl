import requests
import io
from zipfile import ZipFile
import subprocess
import os

def setup_dirs():
    subprocess.run('mkdir ../vendor/')
    subprocess.run('mkdir ../lib/')
    subprocess.run('mkdir ../include/')
    subprocess.run('mkdir ../include/GLFW')


def install_glfw():
    setup_dirs()
    glfw_version = 'glfw-3.3.2.bin.WIN64'

    request = requests.get('https://github.com/glfw/glfw/releases/download/3.3.2/glfw-3.3.2.bin.WIN64.zip')
    glfw_zfile = ZipFile(io.BytesIO(request.content))
    glfw_zfile.extract(os.path.join(glfw_version, '/lib-vc2019/glfw3.lib'), '../lib/')
    glfw_zfile.extract(os.path.join(glfw_version, 'glfw-3.3.2.bin.WIN64/include/GLFW/glfw3.h'), 
                                                '../include/GLFW/')
    glfw_zfile.close()

    subprocess.run('mv ../lib/' + glfw_version + '/lib-vc2019/glfw3.lib ../lib/')
    subprocess.run('rm -r ../lib/' + glfw_version)
    subprocess.run('mv ../include/GLFW/' + glfw_version + '/include/GLFW/glfw3.h ../include/GLFW')
    subprocess.run('rm -r ../include/GLFW/' + glfw_version)


