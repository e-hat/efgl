# efgl

*I am currently working on a rewrite of this engine in Vulkan*

## About
This engine is somewhere I can implement modern rendering techniques and hopefully make some beautiful images along the way. So far, this engine has successfully implemented the following features:

* Clustered forward shading model using OpenGL compute shaders
* Physically based shading model
* Linear lighting
* HDR with Reinhard tonemapping
* Gamma-correction

The current roadmap is as follows:

* Shadows using NVidia's PCSS algorithm, which builds off the classic shadowmapping technique and cleanly replcaces the PCF step.
* Post-processing, including SSAO. 

### Progress So Far

<img src="progress/sphere.PNG" width="900" height="506"/>

Sphere primitive generated.

<img src="progress/clustered_demo.PNG" width="900" height="506">

Clustered forward shading result with 1024 lights on the Sponza scene. This is using a Blinn-Phong lighting model and HDR + Reinhard tonemapping, along with gamma correction.

<img src="progress/phong_demo.PNG" width="700" height="700" />

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
* ~~[Microsoft Visual Studio 2019](https://visualstudio.microsoft.com/vs/), although I plan to make this cross-platform eventually. I've never tested on other platforms so I can't say what would happen exactly.~~ I believe it is now cross platform (I am currently developing it on linux instead of windows). You'll need CMake and you will probably want a good IDE as well. I like CLion.
### Installation
Open a command prompt in the directory where want to clone the repo. Enter `git clone --recursive https://github.com/deadbird11/efgl.git`, then `cd efgl`. Create a build directory, like `build`, then `cd` into it. Run `cmake ..` and it should generate some Makefiles. You can also generate other project files for whatever platform you're on. Finally, if you're using make, run `make install` and it should build an `efgl` binary. Enjoy!

## Acknowledgements
* This project currently relies on [GLFW](https://www.glfw.org/), which is an open-source library for OpenGL.    
* This project also relies on [glad](https://github.com/Dav1dde/glad), which is a OpenGL Loader-Generator.
* I am using [ASSIMP](https://www.assimp.org/) as my model loading library.
* This project uses [tracy](https://github.com/wolfpld/tracy) for profiling.
