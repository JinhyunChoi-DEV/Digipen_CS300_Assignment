* Name: 
- Jinhyun Choi

* DigiPen Id: 
- jinhyun.choi

* How To Use User Interface:
 - Camera Movement:
   A: Move to Left.
   D: Move to Right.
   W: Move to Front.
   S: Move to Back.
   Q: Rotate by Left (Base on Yaw).
   E: Rotate by Right (Base on Yaw).
   Z: Rotate by Up (Base on Pitch).
   C: Rotate by Down (Base on Pitch).
   R: Reset Camera's Position and Rotation.
   ESC: Close Current Application.
 - GUI:
   Toggle: Turn On/Off Toggle What you want to draw Normal Line (Vertex Normal/Face Normal).
   DropDown: Select Object What you want to draw.

* Assumption that the application to fail:
 - I believe that there is no existing assumption that the application to fail.

* Part of the assignment has been completed:
 - All part Completed:
   Setting up OpenGL application, Reading data for OBJ file, Implementing vertex and fragment shaders (Phong Lighting), Scene Setup, Face&Vertex Normals calculation and display, Adding GUI controls.

* Part of the assignment has not been completed:
 - N/A (None and finished all)

* Relevant source codes are located:
 - Path: Engine
   Application.hpp / Application.cpp
   Camera.hpp / Camera.hpp
   Graphic.hpp / Graphic.cpp
   GUIManager.hpp / GUIManager.cpp
   Mesh.hpp / Mesh.cpp
   MeshManager.hpp / MeshManager.cpp
   ObjectLoader.hpp / ObjectLoader.cpp
   Shader.hpp / Shader.cpp
   ShaderManager.hpp / ShaderManager.cpp
   VertexHelper.hpp
   VertexObjectManager.hpp / VertexObjectManager.cpp
 - Path: Engine/ShaderFiles
   Default.vert / Default.frag
   Line.vert / Line.frag
 - Path: EngineSimulator
   Assignment1Stage.hpp / Assignment1Stage.cpp
   OrbitSphere.hpp / OrbitSphere.cpp
   SphereMesh.hpp / SphereMesh.cpp

* Tested Machine
 - Remote: My own Desktop
   OS: Window11
   GPU: RTX 3080
   OpenGL Version: 4.0

* Spend Time:
 - 20~25 hours (taken 3~4 days and spend each days 4~6 hours)

* Additional
 - The ImGui and glad.c file is the third-pary codes so I did not added own header comment. Please heed this.