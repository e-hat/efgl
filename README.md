# efgl
## About
This engine is still very much in progress, as most of the major features have not yet been built. The current roadmap is as follows:

* Create a clustered forward rendering pipeline for efficiency and and graphical fidelity.
* Implement a physically-based shading model.
* Post-processing, including SSAO. 
* Shadows, which I intend to use PCSS for, although, like many things in this project, this will require MUCH research.

This project began as an abstraction over OpenGL where I played around with various simple lighting techniques, which explains the outsized commit history.

### Progress So Far

<img src="progress/phong_demo.PNG" width="250" height="250" />

Simple phong lighting model with forward rendering, which will be adapted to work with clustered forward rendering.

<img src="progress/subdiv_demo.PNG" width="250" height="250" />

Debug screenshot of subdividing between near and far plane into clusters on the sponza model. This idea will be used in the compute shader for culling lights into clusters. 

<img src="progress/first_model_render.PNG" width="250" height="250" />   

First rendered model.  

<img src="progress/gooch_shading.PNG" width="250" height="250" />

Gooch shading model implementation. See reference for Stanford dragon model below.

Morgan McGuire, Computer Graphics Archive, July 2017 (https://casual-effects.com/data)

## Getting Started
### Requirements
* [Microsoft Visual Studio 2019](https://visualstudio.microsoft.com/vs/), although I plan to make this cross-platform eventually. I've never tested on other platforms so I can't say what would happen exactly.
### Installation
Open a command prompt in the directory where want to clone the repo. Enter `git clone --recursive https://github.com/deadbird11/efgl.git`, then `cd efgl`. Type the command `cd setup/`, followed by `./setup.exe`. In the `efgl/` directory, there should now be a file named `efgl.sln`.   
### Profiling
To do profiling with this library, define the `PROFILING` macro. This will write to a `[something]results.json` file in the `efgl/Sandbox` directory. Simply drag & drop this file into [`about:tracing`](chrome://tracing) to see the profiling results. To profile a function, type `PROFILE_FUNCTION()` at thbeginning, and wrap the function call between a call to `Instrumentor::Get().BeginSession[session name]` and `Instrumentor::Get().EndSession()`.
## Acknowledgements
* Thank you to [The Cherno](https://github.com/TheCherno) for his awesome Youtube series that got me into OpenGL, which can be found [here](https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2). He also came up with the `premake5.lua` script except for the minor changes I've made, which are slowly accumulating to make it a completely different document.
* This project currently relies on [GLFW](https://www.glfw.org/), which is an open-source library for OpenGL.    
* This project also relies on [glad](https://github.com/Dav1dde/glad), which is a OpenGL Loader-Generator.
* I am using [ASSIMP](https://www.assimp.org/) as my model loading library.
* This project uses [tracy](https://github.com/wolfpld/tracy) for profiling, which is a *seriously* cool tool.
