# OpenGL

## Drawing in OpenGL
### Graphics Pipeline
- transfer of 3D coordinates to 2D space of the screen
- subparts:
  1. 3D coordinates -> 2D coordinates
  2. 2D coordinates -> colour pixels
- graphics pipeline has a lot of stages, that can be defined as some **functions**
- this functions are called shaders

#### Stages:
0. prepare array of Vertices
1. vertex shader -> takes 3D vertices and gives another 3D vertices
2. assembly of the primitive -> takes vertices -> gives primitive
3. geometry shader -> takes the primitive -> gives resulting primitive ~~some geometry~~
4. rasterization stage -> takes resulting primitives -> gives fragments (pixels on the screen)
5. fragment shader -> takes fragments -> gives color of the pixel
6. blending stage -> cutting fragments that are not seen

#### Vertices layout:
In our VBO, we copied an array of vertices with next params: 
- 1 vertex - 3 coordinates - 12 byt
- 1 coordinate - 4 bytes / 32 bits
- all vertices are packed (in our case, at least) without gaps

With all the all that info, we can tell GL how to **interpret** our data
(see glVertexAttribPointer, glEnableVertexAttribArray)

For that we will need to call a number of functions each time, which can be too ugly and big.

Instead, opengl **forces** us to use **VAO**

#### VAO - Vertex Array Object
This is an object, which encapsulates in itself:
  - calls of _glEnableVertexAttribArray()_ and _glDisableVertexAttribArray()_, 
which are responsible for which **vertex attribute config** is being used for interpretation of vertices

  - **vertex attribute configs** from _glVertexAttribPointer_ function 
  - **VBO`s**, that are bound with vertex attributes by _glVertexAttribPointer_ function

![4-4.png](img/4-4.png)

