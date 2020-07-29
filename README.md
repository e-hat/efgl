# efgl
## About
Currently, this is just a C++ abstraction of the OpenGL API (I plan to expand to other rendering APIs at some point, such as Vulkan). It will be useful for me to have this around for various projects I might want to do in the future.
### Long-term Goals
I want to collect a lot of graphical effects and techniques here in the `efgl/Sandbox/src/examples` folder, along with animation techniques and other stuff having to do with real-time rendering. Once I have those, I want to revisit my [JumboEngine project](https://github.com/deadbird11/efgl/JumboEngine) and begin building a rendering engine there.

## Getting Started
### Requirements
* [Microsoft Visual Studio 2019](https://visualstudio.microsoft.com/vs/), although I plan to make this cross-platform eventually    
### Installation
Open a command prompt in the directory where want to clone the repo. Enter `git clone --recursive https://github.com/deadbird11/efgl.git`, then `cd efgl`. Type the command `cd setup/`, followed by `./setup.exe`. In the `efgl/` directory, there should now be a file named `efgl.sln`.   
### Building `efgl` as a static library
First, open `efgl/efgl.sln` in Visual Studio. Click on the `efgl` __project__ (not solution) in the solution explorer on the left.      
   
<img src="img/solutionexplorer.png" alt="step1" width="209" height="191">      
    
Press <kbd>⌘B</kbd> to build the selected project. Now, in the main `eflg/` directory, your .lib file should be in a directory named something like `efgl/bin/efgl/Debug-windows-x86_64/`.  
### Using the `efgl` Sandbox
First, open `efgl/efgl.sln` in Visual Studio. Then, you can simply use the Sandbox project as you would any other project as it comes with the linking and include directories already set up. Add your source files in `/efgl/Sandbox/src`, which will come with a simple `main.cpp` already there.      
### Using `efgl` in another project
For this, you will need to follow the steps in the section titled "Building `efgl` as a static library." Once you have the .lib file, add it as a dependency to your project. For the headers, I recommend adding `efgl/efgl/src`, `efgl/vendor/GLFW/include`, `efgl/vendor/Glad/include`, and `efgl/vendor/stb` to your list of include directories. In those folders, you will also find `glfw3.lib`, which you should link your project against, and `glad.c`, which you should include in your project as source code. 
### Using `xample` to save experiments
If you are working in `Sandbox/src/main.cpp` and you are doing something interesting that you would like to save in the `Sandbox/src/examples` folder in a "snapshot" fashion, simply navigate to `efgl/Sandbox/src` on a terminal and type the command `xample <name of example>`. This will save a snapshot of the current `src/resources`, `src/shaders`, and `src/main.cpp`, and it will rename the `main` function to whatever the title of your example is. The source code for `xample` can be found [here](https://github.com/deadbird11/xample) if you would like to build it yourself.
## Acknowledgements
* Thank you to [The Cherno](https://github.com/TheCherno) for his awesome Youtube series that got me into OpenGL, which can be found [here](https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2). He also came up with the `premake5.lua` script except for the minor changes I've made.
* This project currently relies on [GLFW](https://www.glfw.org/), which is an open-source library for OpenGL.    
* This project also relies on [glad](https://github.com/Dav1dde/glad), which is a OpenGL Loader-Generator.
