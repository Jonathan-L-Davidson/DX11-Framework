Make object loader create objects from json file while accessing shaders and textures from both managers.

Make textures load textures from json file

Make input manager
Move input part of graphics manager to input manager.

Make camera setpos and whatnot work.

Make player controlled object move and rotate based on keys entered.

Have 4 cameras:
FPS camera
TPS camera
Top-down view
static view

Make JSON load camera positions based off JSON file.

Add in dynamic plane generation

Add hardcoded cube and pyramids back in?

Setup wireframe system

Catch up with tutorials


Optional extras:
Add transparency.
Add height map for plane.
Add skybox
Add sun.
Add lens flare.

So far:
DX11 refactor done! Now I can work on object management and other asset management controllers.
Doesn't crash when game is loaded.
Set Shader to work with object.
Make Graphic manager draw each object in scene given by the object manager.
Create an Object Manager.
Create a shader manager
Create a texture manager
Make a get time functional globally