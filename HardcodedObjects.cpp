#include "HardcodedObjects.h"

Cube::Cube(DxDevice* device) {
    SimpleVertex vertices[] =
    {
        { XMFLOAT3(-1.0f, 1.0f, 1.0f),    XMFLOAT3(-0.816497f,0.408248f, 0.408248),       XMFLOAT2(0.0f, 0.0f), },
        { XMFLOAT3(1.0f, 1.0f, 1.0f),     XMFLOAT3(0.816497f, 0.408248f, 0.408248f),      XMFLOAT2(1.0f, 0.0f), },
        { XMFLOAT3(-1.0f, -1.0f, 1.0f),   XMFLOAT3(-0.666667f, -0.666667f, 0.333333f),    XMFLOAT2(0.0f, 1.0f), },
        { XMFLOAT3(1.0f, -1.0f, 1.0f),    XMFLOAT3(0.408248f, -0.408248f, 0.816497f),     XMFLOAT2(1.0f, 1.0f), },
        { XMFLOAT3(1.0f, -1.0f, -1.0f),   XMFLOAT3(0.666667f, -0.666667f, -0.333333f),    XMFLOAT2(1.0f, 1.0f), },
        { XMFLOAT3(-1.0f, -1.0f, -1.0f),  XMFLOAT3(-0.408248f, -0.408248f,-0.816497f),    XMFLOAT2(0.0f, 1.0f), },
        { XMFLOAT3(1.0f, 1.0f, -1.0f),    XMFLOAT3(0.333333f, 0.666667f, -0.666667f),     XMFLOAT2(1.0f, 0.0f), },
        { XMFLOAT3(-1.0f, 1.0f, -1.0f),   XMFLOAT3(-0.816497f, 0.408248f, -0.408248f),    XMFLOAT2(0.0f, 0.0f), },
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

    MeshData* mesh = new MeshData();

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 8;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;

    device->GetDevice()->CreateBuffer(&bd, &InitData, &mesh->VertexBuffer);

    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * sizeof(indices);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = indices;
    device->GetDevice()->CreateBuffer(&bd, &InitData, &mesh->IndexBuffer);

    mesh->IndexCount = 36;

    LoadModel(mesh);
}

Cube::~Cube() {
}

Pyramid::Pyramid(DxDevice* device) {
    SimpleVertex pyramidVertices[] = {
    { XMFLOAT3(1.0f, 0.0f,-1.0f),       XMFLOAT3(0.333333f,0.333333f,-0.666667f),       XMFLOAT2(1.0f, 1.0f), },
    { XMFLOAT3(1.0f, 0.0f, 1.0f),       XMFLOAT3(0.816497f,0.333333f,0.408248f),        XMFLOAT2(0.0f, 1.0f), },
    { XMFLOAT3(-1.0f, 0.0f, 1.0f),      XMFLOAT3(-0.333333f,0.333333f,0.666667f),       XMFLOAT2(1.0f, 1.0f), },
    { XMFLOAT3(-1.0f, 0.0f,-1.0f),      XMFLOAT3(-0.408248f,0.333333f,-0.816497f),      XMFLOAT2(0.0f, 1.0f), },
    { XMFLOAT3(0.0f, 1.0f, 0.0f),       XMFLOAT3(0.0f,1.0f,0.0f),                       XMFLOAT2(0.5f, 0.0f), },
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

    MeshData* mesh = new MeshData();

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 5;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = pyramidVertices;

    device->GetDevice()->CreateBuffer(&bd, &InitData, &mesh->VertexBuffer);

    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * sizeof(indicesPyramid);
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = indicesPyramid;
    device->GetDevice()->CreateBuffer(&bd, &InitData, &mesh->IndexBuffer);

    mesh->IndexCount = 18;

    LoadModel(mesh);
}

Pyramid::~Pyramid() {
}