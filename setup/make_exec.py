import subprocess

subprocess.call('py -m pyinstaller -F setup.py')
subprocess.call('mv -f dist/setup.exe .')
subprocess.call('rm -rf dist')
subprocess.call('rm -rf build')
subprocess.call('rm -rf __pycache__/')
subprocess.call('rm -f setup.spec')