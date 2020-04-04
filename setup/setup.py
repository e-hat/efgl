import subprocess
subprocess.call('pip install requirements.txt')

import fetch
fetch.install_glfw()

subprocess.call('vendor/bin/premake5 premake5.lua')