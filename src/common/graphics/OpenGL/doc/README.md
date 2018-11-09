Differences with Glumpy

TODO:
prog['posi'] = {...}; with OpenGL context not yet created => do not throw but create + m_active = false;

*.hpp
=====

C++: can pass a name for the debug

IGLObject.hpp:
==============

C++: m_handle when not set can be 0 or -1 while in Python: m_handle is -1
C++: m_need_delete does not exist, replaced by canBeReleased() or isValid()
C++: no id count
C++: begin() vs. activate()
C++: begin() plus de if: activate() protected by isValid()
C++: destroy() has protection directly in the caller method therfore does not need check like done in python
C++: create(), setup(), update() return a bool instead of void in python
C++: geometry shader not implemented

GLProgram.hpp
=============

C++: shader core version not used

GLShader.hpp
============

C++ kooks, snippets not used/not implemented
C++ remplacing opengl core version in shader not used/implemented
C++ no FromString FromFile distinction with '{' inside the shader
C++ setup is used for compiling insted of inside the create()/update() in python code
C++ no pretty print of shader compilation error

VAO.hpp
=======

C++ does not have a VBO

VBO.hpp
=======

C++: code of create() has been moved into setup(). So does not need activate() and aactivet()
C++: container can be extended while its data has not been updated into GPU.
C++: not distinction of string a stream with the '{' inshader call explicit methods instead


GLVarible.hpp
=============

C++: active, dtype, gtype not used/implemented

GLUniform.hpp
=============

C++: code for texture has been moved into Sampler class.
Python code uniform for data and uniform for texture is mixed

GLSampler.hpp
=============

C++ GLSampler1D, GLSampler3D not implemented

GLtexture.hpp
=============

C++ Only texture2D implemented

