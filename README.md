# mazeRaylib
Just trying out raylib in C. Run around in the maze to shoot enemies.

## Configure
- First build raylib
```
source ./build-raylib.sh
```

## Build and Run
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

## Map file
(`assets/map/`)
- `#` - walls
- `e` - enemy
- `<, >, v, ^` - player (each character specifies the direction the player is facing)
- `.` - empty space
