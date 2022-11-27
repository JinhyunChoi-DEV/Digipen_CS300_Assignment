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
   Can change model.
   Toggle: Turn On/Off Toggle What you want to draw Normal Line (Vertex Normal/Face Normal).
   Can change each light status and informations
   Using Scenario button to change 3 types different lighting situations
   Can change materials information with environment data
   Can change global constant informations
   Have fun with using GUI to load different models/materials/lights.

* Assumption that the application to fail:
 - I believe that there is no existing assumption that the application to fail.

* Part of the assignment has been completed:
 - All part Completed

* Part of the assignment has not been completed:
 - N/A (None and finished all)

* Relevant source codes are located:
 - Path: Engine
   EnvironmentMapping.hpp / EnvironmentMapping.cpp
   SkyBox.hpp / SkyBox.cpp
   TextureManager.hpp / TextureManager.cpp
   Graphic.hpp / Graphic.cpp

 - Path: ShaderFiles
   EnvironmentMapping.vert / EnvironmentMapping.frag
   FrameBuffer.vert / FrameBuffer.frag
   Skybox.vert / Skybox.frag
   Light.glsl
   Texture.glsl
   TransformModel.glsl

 - Path: EngineSimulator
   Assignment3Stage.hpp
   Assignment3Stage.cpp

* Tested Machine
 - Remote: My own Desktop
   OS: Window11
   GPU: RTX 3080
   OpenGL Version: 4.3

* Spend Time:
 - total spend time to 30~40 hours.

* Additional
 - The ImGui, glad.c, and stbi_load file is the third-pary codes so I did not added own header comment. Please heed this.
 - stbi_load file have some warning.