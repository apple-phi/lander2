# Lucas's Mars lander graphics extension

The build requirements are modern versions of [mesa](https://mesa3d.org/), [glbinding](https://github.com/cginternals/glbinding#install-instructions), [glfw](https://github.com/glfw/glfw) and `cmake`:
```sh
cmake -B build/ -DCMAKE_BUILD_TYPE=Release
cmake --build build/ --config Release --target all --
cd build/
LanderApp.exe
```

Screenshot demonstrating the normal mapped per-pixel lighting and gamma correction after rasterization of the color map:
![](./screenshot.png)