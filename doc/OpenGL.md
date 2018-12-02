# OpenGL Wrapper API Architecture

This API, greatly inspired by the Python library
[GLumpy](https://glumpy.github.io/), allows to wrap OpenGL core API
(>= 3.3) within C++ classes

This document is at the same time a document describing the API
architecture but also an introduction to OpenGL for people interesting
to study OpenGL tutorials.

**Warning:** *this API is currently in gestation. This document is the
expected final API behavior, when reading this line, the current
version may not yet be totally compliant with this page. If you notice
something wrong, please contribute by reporting either a bug or a
fix !*

## Why this API and why again an N-th OpenGL wrapper project ?

* The first need from this API is that contrary to legacy OpenGL,
learning modern OpenGL (aka OpenGL core), is very difficult for
beginners. Indeed before painting a simple triangle you have to
understand (stop breathing) what are VBOs, VAOs, shader programs, how
to compile shaders, what are attributes and uniforms and how to locate
them in shaders how to create link between the VAO and VBOs then
finally how to tell to OpenGL how to interprete VBOs ... (start
breathing). This is very confusing because beginers has to read
several pages of unknown concepts before succeeding in drawing a
simple scene. In addition many tutorials are not so clear and well
written: they sometimes content of saying "VAO: just do this way we do
not have to know more ..." or "attribute location: why not to do like
this !". For SimTaDyn project I did not want to build scenes like a
game. OpenGL is made for games not for painting map applications but I
did not wanted to use libraries like Cairo because I wanted to have
different views of the same map: editing in 2D and displaying in 3D
results (for example 3D cities with roads and cars). I needed an
OpenGL wrapper !

* The second need from this API is that OpenGL does not manage dynamic
size VBOs easily (as far as I know). I needed to edit maps with an
arbitrary number of nodes and arcs which is problematic with fixed
size VBO (OpenGL is made for games and characters are rendering with
almost the same number of vertices (if we except teslations).

* I looked for a such kind of OpenGL library on github but I founded
none which allowed me to write OpenGL scenes in few lines until I
found GLumpy (unfortunately written in Python). C++ wrappers only gave
me a C++ flavour to OpenGL functions which I did not care
(glGenFooBar() vs gl::genFooBar()).

* What I immediatly liked with glumpy is the good class encapsulation
(that I explain in this document) and GLumpy seem to have fixed the
problem of dynamic size. the goal of this wrapper is to hide as much
as possible OpenGL routines as well OpenGL routine calls
order. Therefore it offers to developpers a minimal number of public
methods (which are principaly begin() and end() methods). In
consequence, developpers can create 3D scenes in few lines of code
without knowing how OpenGL guts. The only knowledges required is to
know how to write shaders. In drawback the API is not made for fast
rendering due to additional checks it made internally.

* The first idea of GLumpy is to allow the developper to manipulate
vertices from the software point of view and they are displayed
automaticaly on the screen by the API. Data of the 3D scene are both
in the CPU and the GPU. The developper modifies CPU side and the API
automaticaly transfer them to the GPU. Possibly the OpenGL context can
be lately started. Therefore this library is most made for developping
scentific programs than a game.

* As I said in the first point, tutorials teach OpenGL in the wrong
way. What I did not immediatly understood whith GLumpy is that API
sees these things in the opposite order: instead of starting from list
of triangles to finish to shaders for displaying them, it takes the
opposite direction: starting from shaders and finishing with the list
of triangles.

For all these reasons I started to make a portage of it into C++ !
Anyway my API is not a 100% full feature portage from python to C++ of
the glumpy library:

* Mostly due to the fact that C++ is a strongly typed language which
does not offer the same flexibility than python. For examples: storing
templated classes in the same container or numpy views.

* The second reason is that GLumpy does not use by default VAOs but
access to VBOs through OpenGL program attributes, which also
perturbates me because in the case of you have several models
(therefore several VAOs) in the scene to paint this library seems less
well conceived to draw them but thanks to typage flexibility of
Python, attributes of the sahder can be bind to VAOs.

* My API is a little different: VAOs are 3D models we want to
draw. the API starts loading shaders and creates directly VBOs inside
a VAO. VAOs are created from the program shader. A single VAO can be
known from its shader. The developper can access to VBOs data through
the shader attribute name (strings) and fill GPU buffers with desired
data.

## OpenGL terminology

OpenGL objects are:

* Vertex Buffer Object (VBO) is a portion of GPU memory in which we
store data like vertices, textures, normals, colors.

* Element Buffer Object (EBO) is an buffer that indexes vertices in
the aim to compress vertices by remplacing their information (usually
a 3D coordinate in float) by an identifier (for example an uint8).

* Vertex Array Object (VAO) is an object referencing several VBO (and
up to one EBO) and telling OpenGL how to interprete data. VAOs were
optional in tranditional OpenGL version but they are now mandatory.
VAO they avoid for each display of the scene to transfer data from CPU
into VBOs and prevent telling to OpenGL how to interpret data.

* VAO links to VBO is named attribute locations are also an OpenGL
object.

* VAO can be seen has the identifier of a 3D model. For example in a 3D
scene showing two house, and one car: you have to create two VAOs: one
for the car, one for the house. When displaying the two house we just
display the same object at two different location.

* Shader

* Uniform, attributes

* Attribute locations: is not because your shader attribute is named
position that is enough to name your VBO position. You have to tell
OpenGL the link between the VAO and the VBO.

* RenderBuffer: OpenGL can either paint to the screen or in a
texture. The last option allows to create sophisticate 2D effect by
injecting the texture into a second shader that will apply a more
complex image processing before displaying to result into either the
screen or to a new texture (for a 2nd process for example). Different
kind of rendrer buffers can be: DepthBuffer (the Z coordonate of
object are displayed into a grey texture) or ColorBuffer or
StencilBuffer (for cutting regions of the scene).

## OpenGL Object class

The following table allows to understand that OpenGL routines for any
kind (VAO, VBO ...) of OpenGL objects can be grouped into six
functions: creation/relase, activation/deactivation, setup/update.
[Click here](https://github.com/Lecrapouille/SimTaDyn/blob/dev-new-opengl/src/common/graphics/OpenGL/doc/CodeSheet.txt)
to see in raw format the table.

|              | create()             | setup()                                       | activate()                                       | update()        | deactivate()               | release()                      | m_handle | m_target                                                 |
|--------------|----------------------|-----------------------------------------------|--------------------------------------------------|-----------------|----------------------------|--------------------------------|----------|----------------------------------------------------------|
| VAO          | glGenVertexArrays    | -                                             | glBindVertexArray                                | -               | glBindVertexArray          | glDeleteVertexArrays           | create() | -                                                        |
| VBO          | glGenBuffers         | glBufferData                                  | glBindBuffer                                     | glBufferSubData | glBindBuffer               | glDeleteBuffers                | create() | GL_ARRAY_BUFFER                                          |
| EBO          | glGenBuffers         | glBufferData                                  | glBindBuffer                                     | glBufferSubData | glBindBuffer               | glDeleteBuffers                | create() | GL_ELEMENT_ARRAY_BUFFER                                  |
| Shader       | glCreateShader       | GlShaderSource, GlCompileShader               | -                                                | -               | -                          | glDeleteShader                 | create() | GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER |
| Program      | glCreateProgram      | GlAttachShader, GlLinkProgram, GlDetachShader | glUseProgram                                     | -               | glUseProgram               | GlDeleteProgram,GlDetachShader | create() | -                                                        |
| Uniform      | glGetUniformLocation | -                                             | -                                                | -               | -                          | -                              | create() | -                                                        |
| Attribute    | glGetAttribLocation  | -                                             | GlEnableVertexAttribArray, GlVertexAttribPointer | -               | glDisableVertexAttribArray | -                              | create() | -                                                        |
| RenderBuffer | glGenRenderbuffers   | -                                             | GlBindRenderbuffer, GlRenderbufferStorage        | -               | glBindRenderbuffer         | -                              | create() | GL_RENDERBUFFER                                          |
| Texture      | glGenTextures        | glTexParameterf                               | glBindTexture                                    |                 | glBindTexture              | glDeleteTextures               | create() |                                                          |
| Texture1D    |                      | glTexImage1D                                  |                                                  | glTexSubImage1D |                            |                                |          | GL_TEXTURE_1D                                            |
| Texture2D    |                      | glTexImage2D                                  |                                                  | glTexSubImage2D |                            |                                |          | GL_TEXTURE_2D                                            |
| TextureDepth |                      | glTexImage2D                                  |                                                  | glTexSubImage2D |                            |                                |          | GL_TEXTURE_2D                                            |
| Texture3D    |                      | glTexImage2D                                  |                                                  | glTexSubImage2D |                            |                                |          | GL_TEXTURE_CUBE_MAP                                      |

API routines are:
- create() creates an OpenGL object (GPU side). An OpenGL Object is just a reference on the GPU memory that the CPU can use (named m_handle in our classes) and instead manipulating a GPU
address OpenGL returns an identifier (1, 2 ...). Note that 0 has a particular meaning and used for unbinding an Object (more details below).
- release() allows to liberate OpenGL Object from the GPU memory.
- setup() allows to parametrize the object (like done for textures, Shader, Program).
- activate(): allows to bind m_handle and deactivate() to unbind (0 is used instead of m_handle). What is binding ? OpenGL can only operate with a single object for each kind of object (1 VAO
is used at a time, as well as VBO, Program ...). You can see that like a global variable inside OpenGL state machine that you change with bind functions. The advantage is that do not have to pass
the object reference in all OpenGL functions. They all refer to binded objects. The drawback is that you can forget to bind/unbind and use the wrong object references.

These routines are virtual methods of the Interface class named IGLObject (a pure virtual class). Any OpenGL Object is a class inheriting from IGLObject and implementing all these methods. The
table is a summary of which OpenGL functions is called by the concrete methods (implementation). If we think well (glumpy did it for me ;-) we can hide the presence of these methods to the
developper: indeed they are private methodes in IGLObject: they are acceeded by two public method: begin() and end(). Here is the pseudo code of the begin() method:
```
begin()
{
  if (need_create) { need_create = create() || return; }
  activate();
  if (need_setup) { need_setup = setup() || return; }
  if (need_update) { need_update = update(); }
}
```
need_create, need_setup, need_update are the states of IGLObject: they are private variable members remembering what IGLObject method (implemented by IGLObject base class) to call. The real code
performs more checks than this pseudo code. If a method fails (for example OpenGL context is not yet created) it can be redone later. The end() method simply calls the deactivate() method.

## Pending Data

The API allows the developper to modifiy data located in the CPU. Modified data are updated automatically to the GPU for their display. This particularly true for VBOs, textures and
uniforms. As a consequence you can work without OpenGL context ready. VBOs are an array of data. CPU side, the API maintains for these object index of the contiguous portion of dirty data. These
data are send at the very last moment to GPU through the update(); method.

Let suppose the following VBO array storing three consecutive 3D position (x,y,z). Let suppose there is no pending data.

| 0 | 0 | 0 | 1 | 1 | 1 | 2 | 2 | 2 | PendingData={,} |
|---|---|---|---|---|---|---|---|---|-----------------|

Let change the y position of the 1st position with the value 42 which is now "dirty".

| 0 | 42 | 0 | 1 | 1 | 1 | 2 | 2 | 2 | PendingData={1,1} |
|---|----|---|---|---|---|---|---|---|-------------------|

Pending data is now refering to the first position of the array.

Now let change the z position of the 2nd position with the value 43 which is now "dirty".

| 0 | 42 | 0 | 1 | 1 | 43 | 2 | 2 | 2 | PendingData={1,5} |
|---|----|---|---|---|----|---|---|---|-------------------|

Now all data from position 1 to 5 are considered as dirty. Pending data is now refering to the first and fifth position of the array.

Now let change the x position of the 1st position with the value 44 which is now "dirty".

| 44 | 42 | 0 | 1 | 1 | 43 | 2 | 2 | 2 | PendingData={0,5} |
|----|----|---|---|---|----|---|---|---|-------------------|

Now all data from position 0 to 5 are considered as dirty. Pending data is now refering to the zeroth and fifth position of the array.

Let suppose that VBO::update() is now called. All dirty data (position 0 to 5) are flushed to the GPU, there is no more dirty data and
pending data indices are cleared.

| 44 | 42 | 0 | 1 | 1 | 43 | 2 | 2 | 2 | PendingData={,} |
|----|----|---|---|---|----|---|---|---|-----------------|

Flushing bulk of data, with in our case, some of them were not really dirty, seems not an optimized way of doing things. This is true
but this allows to reduce OpenGL routine calls. In addition we can imagine more sophisticated method in the case where order of element
is not important: in this case you can simply swap position of the dirty element with the first non dirty element.

## Class Inheritance

The following picture shows the inheritance of OpenGL classes.

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/dev-new-opengl/doc/uml/opengl_inhe.png)



## Class Composition

The following picture shows the composition of OpenGL classes.

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/dev-new-opengl/doc/uml/opengl_compo.png)

## Pseudo code of an OpenGL program

Init phase:
- First write your fragment and vertex shaders either in a std::string or from a text file.
- Load the script into the GLFragmentShader and GLVertesShader with method fromFile() or fromString().
- Optionally add a geometry shader.
- Make GL*Shader compiled by a GLProgram. This one, if no errors in shaders occured, will create a list of GLUniform, GLAttribute and VBOs.
- Call the method GLProgram::createVAO() to create has much as VAO as needed.
- For each VAO created, fill their attached VBOs with data (depending on your shader: vertices position, colors, textures) use the exact name of attributes written in shaders.
- Use GLProgram::attributeNames() to get the list of VBO names.

Draw phase:
- For each VAO makes it used with GLProgram::bind() then call the GLProgram::draw()
```

```
