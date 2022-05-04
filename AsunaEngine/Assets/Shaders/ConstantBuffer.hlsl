#include "Lighting.hlsl"

cbuffer ConstantBufferPerFrame : register(b0)
{
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix vp;
    DirectionLight directionLight;
    SpotLight spotLights[4];
};

cbuffer ConstantBufferPerObject : register(b1)
{
    matrix worldMatrix;
}

cbuffer ConstantBufferPerMaterial : register(b2)
{
    float4 mainColor;
    matrix modelMatrix;
}