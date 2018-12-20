# OpenGL Natural Landscape
Landscape scene rendered in OpenGL. By Meng Dong and Tinda Wang.


## Requirements
`C++ 11` or higher required. Packages `GLM`, `GLEW`, `GLFW` included in the repo along with the `stb_image` open source public image loader.

## Run
To run the program on macOS, in the command line, enter the following,

```$ cd src```

```$ make```

```$ ./main```

Be sure to have the packages installed. The versions included in the repo require cmake to install.

The project was built on macOS Mojave. The program has not been tested on other machines or operating systems.

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
 
 `A` : Adding more trees (Three instead of one for now)

## Code Structure

![Alt text](screenshots/struct.png?raw=true "Code Structure")

## Future Work
Adding hieghtmap terrain, water, wind, tree physics, environment mapping etc.
