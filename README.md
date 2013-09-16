# Inside Game

## Description
This is a digitized version of [this game](http://boardgamegeek.com/boardgame/30179/inside)

The game is in really early stage of development and the code isn't very nice at the moment.

So far you can place cubes on the board with one magor bug, 
you can place a cube that has no standing cubes below it.

## Implementation

I'm using OpenGL ES2 for the rendering, glm for math related things, 
glfw for creating gl contexts and managing input, libpng for loading images.

Only linux is supported as a platform for now.
 
## Roadmap

1. Playable game in human vs human mode. 
2. Port game to android.
3. Port game to web using emscripten.
4. Implement human vs computer game mode.

## Compiling

1. Clone the repo.
2. Cd to main project directory and create a build directory.
3. cd to build/ and type cmake ../ && make

## License 

See LICENSE file

 
