# mazeRaylib
Run around in the maze to shoot enemies. I am just trying out [raylib](https://www.raylib.com/) in C. 

## Prerequisites
- [gcc](https://gcc.gnu.org/)

## Configure
NOTE: for linux
- First get raylib and build it
- This will add raylib as a submodule and build it
```
source ./build-raylib.sh
```

## Build and Run
NOTE: for linux
- Debug build
```
source ./build.sh
./build/main
```
- Release build
```
source ./build-rel.sh
./build/rel/main
```

## Controls
- `WASD` to move player, also while moving:
    - Hold `Shift` to speed up
    - Hold `Left control` to slow down
- move mouse to look around
- `Esc` to open pause menu
- `CTRL+Q` to quit the application

### In debug builds
- `Tab` to toggle between Scene camera and Player camera
- To control Scene camera:
    - `WASDQE` to move the camera
    - Hold `Shift` to slow down
    - Hold right mouse button and move mouse to look around
    - Hold middle mouse button to orbit the camera around y-axis

## Map file details
(path: `assets/map/`)
- `#` - walls
- `e` - enemy
- `<, >, v, ^` - player (each character specifies the direction the player is facing)
- `.` - empty space

## Screenshots
- Game
<img src="img/game.png">

- Scene camera (only in debug builds)
<img src="img/maze.png">

- Pause menu
<img src="img/pause-menu.png">
