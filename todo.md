Fix objects not rendering.

Cleanup destructors and make sure all pointers are cleaned up.

Create subtypes of objects with custom Update() calls to make them s p e e n.

Make object loader create objects from json file while accessing shaders and textures from both managers.
Make textures load textures from json file

Make camera setpos and whatnot work.

Make input manager
Move input part of graphics manager to input manager.
Setup wireframe system

Make a get time functional globally

!MAYBE! Create a light object..?
!MAYBE! Make light object s p e e n


So far:
DX11 refactor done!...ish. Now I can work on object management and other asset management controllers.
Doesn't crash when game is loaded.
Memory leaks fixed.