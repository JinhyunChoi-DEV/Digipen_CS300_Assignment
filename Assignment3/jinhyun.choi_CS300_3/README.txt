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
   Can change dfferent 3D models
   Toggle: Turn On/Off Toggle What you want to draw Normal Line (Vertex Normal/Face Normal).
   Using "reload shader" button to change shader
   Using Scenario button to change 3 types different lighting situations
   Can change materials information
   Can change global constant informations
   Can change materials informations
   Can change each light status and informations
   Have fun with using GUI to load different models/shaders/materials/lights.

* Assumption that the application to fail:
 - I believe that there is no existing assumption that the application to fail.

* Part of the assignment has been completed:
 - All part Completed

* Part of the assignment has not been completed:
 - N/A (None and finished all)

* Relevant source codes are located:
 - Path: Engine
   PPM.hpp / PPM.cpp
   PPMFileReader.hpp / PPMFileReader.cpp
   Texture.hpp / Texture.cpp
   Light.hpp / Light.cpp
   LightTypeHelper.hpp
   TextureTypeHelper.hpp
   UVGenerator.hpp
   UniformBlockObjectManager.hpp / UniformBlockObjectManager.cpp

 - Path: ShaderFiles
   BlinnShading.vert / BlinnShading.frag
   PhongLighting.vert / PhongLighting.frag
   PhongShading.vert / PhongShading.frag
   Light.glsl
   Texture.glsl
   TransformModel.glsl

 - Path: EngineSimulator
   Assignment2Stage.cpp

* Tested Machine
 - Remote: My own Desktop
   OS: Window11
   GPU: RTX 3080
   OpenGL Version: 4.0

* Spend Time:
 - total taken 1 weeks ~ 1.5 weeks

* Additional
 - The ImGui and glad.c file is the third-pary codes so I did not added own header comment. Please heed this.