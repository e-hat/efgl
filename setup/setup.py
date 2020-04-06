import requests
import io
from zipfile import ZipFile
import subprocess


def install_glfw():
    # TODO: this needs to change for cross platform
    glfw_version = 'glfw-3.3.2.bin.WIN64'
    lib_dest = '../efgl/vendor/bin/'
    header_dest = '../efgl/vendor/GLFW/include/'

    # creating destinations
    subprocess.call('mkdir ' + lib_dest)
    subprocess.call('mkdir ' + header_dest)

    # downloading GLFW
    request = requests.get('https://github.com/glfw/glfw/releases/download/3.3.2/glfw-3.3.2.bin.WIN64.zip')
    glfw_zfile = ZipFile(io.BytesIO(request.content))
    # extracting to respective directories
    # TODO: vc2019 --> check what this means
    glfw_zfile.extract(glfw_version + '/lib-vc2019/glfw3.lib', lib_dest)
    glfw_zfile.extract(glfw_version + '/include/GLFW/glfw3.h', header_dest)
    glfw_zfile.close()
    
    # cleaning up leftover folders and whatnot, maybe there's a cleaner way
    subprocess.call('mv ' + lib_dest + glfw_version + '/lib-vc2019/glfw3.lib ' + lib_dest)
    subprocess.call('rm -r ' + lib_dest + glfw_version)
    subprocess.call('mv ' + header_dest + glfw_version + '/include/GLFW/glfw3.h ' + header_dest)
    subprocess.call('rm -r ' + header_dest + glfw_version)
    

install_glfw()
# maybe make this an input from the user
subprocess.call('../vendor/bin/premake5 vs2019') 