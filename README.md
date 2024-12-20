# Voxel 1993
A basic 3d engine written in C++ like it's Doom 1993. All the rendering is done manually (nothing fancy like using OpenGL for the 3d rendering).
Though it still uses OpenGL for the window creation and input handling. Goal is to have a simple 3d engine capable of rendering a 3d voxel world.

## Building
Project is setup to use CMake. To build the project first create a build directory and run CMake. Then run make to build the project.

```bash
mkdir build
cd build
cmake ..
make
```
*For development consider using [compiledb](https://github.com/nickdiego/compiledb).*

## Project Structure
- `src/` - Contains all the source code for the project
- `include/` - Contains all the header files for the project
- `textures/` - Contains all the textures used in the project 
- `build/` - Contains the build files generated by CMake

## Features

### Rendering
Rendering is currently done using a Z-Buffering algorithm. Later I plan to combine this with a Voxel Octree
in combination with multi-threading to render the world. Z-Buffering will prevent overdraw while using multi-threading.
- [x] ~~Painters algorithm for rendering (antioptimization)~~
- [x] Z-Buffering for rendering
- [ ] Voxel Octree

### Chunking System
World is going to be split into chunks for better performance of size 8x8x8. 
- [ ] Single chunk
- [ ] Chunking system
- [ ] Multi-threaded chunk queue management

### World Generation
World is going to be generated using Perlin Noise. Later planning to implement a combination of noises.
- [ ] Perlin Noise
- [ ] Create world using Perlin Noise
- [ ] Combine multiple noises

### Loading/Saving
World is going to be split into segments made up of chunks.
- [ ] Basic load/save system
- [ ] World segmentation

## Progress

### First Major Performance Improvement
First major performance improvmement was gaind by switching from a painters algorithm to a Z-Buffering algorithm.

#### Test on 4x4x4 chunk
As expected Z-Buffering is faster than the painters algorithm. By a factor of 1.33x.
|                   | Z-Buffering           | Painters Algorithm    |
|---                |---                    |---                    |
| Time per frame    | 0.0277323             | 0.0368229             |
| Average FPS       | 600.983               | 452.616               |

#### Test on 32x32x32 chunk
Now this is wieard. The difference between the two algorithms is not as big as expected. Thats because the we are still sorting a small number of voxels.
And since the only difference is that the painters algorithm sorts the voxels before rendering them (since I didn't remove the Z-Buffering implementation),
it gets to write to the Z-Buffer only once (since the voxels are already sorted). While the Z-Buffering algorithm writes to the Z-Buffer for every voxel that
is closer to the camera. This is why the Z-Buffering algorithm is not much faster than the painters algorithm (at least that's what I think).
|                   | Z-Buffering           | Painters Algorithm    |
|---                |---                    |---                    |
| Time per frame    | 3.83142               | 4.1841                |
| Average FPS       | 13.05                 | 11.95                 |

#### Test on 64x64x64 chunk
While testing on a 64x64x64 chunk both of the algorithms are slow since I don't have any other optimizations yet. So
the testing was pretty pointless. If you're still wandering the Z-Buffering rendered at 5 FPS while the painters algorithm struggled to reach a single FPS.

## Resources
- [YouTube - 3DSage](https://www.youtube.com/@3DSage) - Basic 3d engine tutorial series
- [YouTube - Cem Yuksel](https://www.youtube.com/@cem_yuksel) - General 3d graphics tutorials

- [Blog - The Infinite Loop](https://geidav.wordpress.com/) - Voxel Octree
- [Blog - Voxel Compression](https://eisenwave.github.io/voxel-compression-docs/index.html) - Voxel Compression
- [Blog - Building a High-Performance Voxel Engine in Unity](https://medium.com/@adamy1558/building-a-high-performance-voxel-engine-in-unity-a-step-by-step-guide-part-1-voxels-chunks-86275c079fb8) - General creation of a voxel engine

- [Voxel.Wiki](https://voxel.wiki/) - General voxel algorithms
