Add hardcoded cube and pyramids back in
Add in dynamic plane generation

Create Scene

Make object loader create objects from json file while accessing shaders and textures from both managers.

Make textures load textures from json file


Make camera setpos and whatnot work.

Make player controlled object move and rotate based on keys entered.

Have 4 cameras:
FPS camera
TPS camera
Top-down view
static view

Make JSON load camera positions based off JSON file.

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
Make input manager
Move input part of graphics manager to input manager.
Setup wireframe system



Notes:

    SimpleVertex vertices[] =
    {
        { XMFLOAT3( -1.0f, 1.0f, 1.0f ),    XMFLOAT3(-0.816497f,0.408248f, 0.408248),       XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, 1.0f ),     XMFLOAT3(0.816497f, 0.408248f, 0.408248f),      XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, 1.0f ),   XMFLOAT3(-0.666667f, -0.666667f, 0.333333f),    XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, 1.0f ),    XMFLOAT3(0.408248f, -0.408248f, 0.816497f),     XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, -1.0f, -1.0f ),   XMFLOAT3(0.666667f, -0.666667f, -0.333333f),    XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, -1.0f, -1.0f ),  XMFLOAT3(-0.408248f, -0.408248f,-0.816497f),    XMFLOAT4( 0.0f, 1.0f, 1.0f, 1.0f ) },
        { XMFLOAT3( 1.0f, 1.0f, -1.0f ),    XMFLOAT3(0.333333f, 0.666667f, -0.666667f),     XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f, 1.0f, -1.0f ),   XMFLOAT3(-0.816497f, 0.408248f, -0.408248f),    XMFLOAT4( 0.5f, 0.5f, 0.5f, 1.0f ) },
    };

    SimpleVertex pyramidVertices[] = {
        { XMFLOAT3(1.0f, 0.0f,-1.0f),       XMFLOAT3(0.333333f,0.333333f,-0.666667f),       XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
        { XMFLOAT3(1.0f, 0.0f, 1.0f),       XMFLOAT3(0.816497f,0.333333f,0.408248f),        XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
        { XMFLOAT3(-1.0f, 0.0f, 1.0f),      XMFLOAT3(-0.333333f,0.333333f,0.666667f),       XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
        { XMFLOAT3(-1.0f, 0.0f,-1.0f),      XMFLOAT3(-0.408248f,0.333333f,-0.816497f),      XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
        { XMFLOAT3(0.0f, 1.0f, 0.0f),       XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
    };

        WORD indices[] =
    {
        2,1,0, // 3
        3,1,2, // 6
        3,2,4, // 9
        2,5,4, // 12
        2,0,7, // 15
        5,2,7, // 18
        6,4,5, // 21
        5,7,6, // 24
        1,3,4, // 27
        4,6,1, // 30
        1,7,0, // 33
        6,7,1, // 36
    };

    WORD indicesPyramid[] =
    {
        0,1,2, // 3
        0,2,3, // 6
        1,4,2, // 9
        2,4,3, // 12
        0,3,4, // 15
        4,1,0, // 18
    };