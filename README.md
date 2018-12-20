# OpenGL Natural Landscape
Landscape scene rendered in OpenGL. By Meng Dong and Tingda Wang.

## Features
An extensible openGL 3D environment framework with underlying texture, shader, mesh, and camera infrastructure that renders trees, leaves and grass, all under a skybox.

## Requirements
`C++ 11` or higher required. Packages `GLM`, `GLEW`, `GLFW` included in the repo along with the `stb_image` open source public image loader. `GLSL`shader language support also required. 

The versions of `GLM`, `GLEW`, and `GLFW` that we included in the repo require `cmake` to install. Using `brew` works as well. 

## Run
The project was built on macOS Mojave. The program has not been tested on other machines or operating systems, although they may be supported/easily ported.

To run the program on macOS, in the command line, enter the following,

```$ cd src```

```$ make```

```$ ./main```

Be sure to have the required packages installed. 

## Controls

The following keyboard controls are supported:

 `R`: Toggle the rotation of world view 
 
 `Right/Left Arrow`: Change the rotation speed
 
 `W` : Zoom in/ walk forward
 
 `S` : Zoom out/ walk backward
 
 `A` : Move camera to the left
 
 `D` : Move camera to the right 
 
 `L` : Toggle leaves
 
 `G` : Grow trees upward  
 
 `B` : Regress trees downward
 
 `M` : Adding more trees (Three instead of one for now)

## Code Structure

![Alt text](screenshots/struct.png?raw=true "Code Structure")

## Future Work
Adding heightmap terrain, water, wind, tree physics, environment mapping etc.

## Acknowledgements
Everything in this project were built for educational purposes with open source tooks and frameworks. The rights to all such packages belong to their respective owners. 

The skybox texture was created by 'The Mighty Pete' at http://www.petesoasis.com which is distributed it under the GNU General Public License version 2.

<a rel="license" href="http://creativecommons.org/licenses/by-nc-nd/3.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-nd/3.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-nd/3.0/">Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License</a>.
