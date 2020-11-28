# efgl
## About
This engine started as an abstraction over OpenGL, with some useful classes such as a Camera and Window along with it. Now, I am working on making it into more of a fully-fledged rendering engine, which began with adding a Scene Graph. Next, I will implement clustered forward rendering by following various guides around the internet. After that, my plan is to implement a physically based shading model. Any other rendering techniques, such as shadows/post-processing are still TBD.

### Progress So Far

<img src="progress/subdiv_demo.PNG" width="250" height="250" />

Debug screenshot of subdividing the between near and far plane on the sponza model.

<img src="progress/first_model_render.PNG" width="250" height="250" />   

First rendered model.  

<img src="progress/gooch_shading.PNG" width="250" height="250" />

Gooch shading model implementation. See reference for Stanford dragon model below.

Morgan McGuire, Computer Graphics Archive, July 2017 (https://casual-effects.com/data)

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
### Profiling
To do profiling with this library, define the `PROFILING` macro. This will write to a `[something]results.json` file in the `efgl/Sandbox` directory. Simply drag & drop this file into [`about:tracing`](chrome://tracing) to see the profiling results. To profile a function, type `PROFILE_FUNCTION()` at thbeginning, and wrap the function call between a call to `Instrumentor::Get().BeginSession[session name]` and `Instrumentor::Get().EndSession()`.
## Acknowledgements
* Thank you to [The Cherno](https://github.com/TheCherno) for his awesome Youtube series that got me into OpenGL, which can be found [here](https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2). He also came up with the `premake5.lua` script except for the minor changes I've made. ALSO he introduced me to the profiling technique using [`about:tracing`](chrome://tracing) which is seriously cool.
* This project currently relies on [GLFW](https://www.glfw.org/), which is an open-source library for OpenGL.    
* This project also relies on [glad](https://github.com/Dav1dde/glad), which is a OpenGL Loader-Generator.
* I am using [ASSIMP](https://www.assimp.org/) as my model loading library.
