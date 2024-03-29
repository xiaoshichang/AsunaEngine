#include "Lighting.hlsl"

cbuffer ConstantBufferPerFrame : register(b0)
{
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix vp;
    DirectionLight directionLight;
    SpotLight spotLights[4];
    PointLight pointLights[4];
    matrix lightViewProj;
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