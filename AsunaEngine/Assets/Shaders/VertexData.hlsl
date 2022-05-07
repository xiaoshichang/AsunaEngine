
struct VertexInput_P3N3T3
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 texcoord : TEXCOORD;
};

struct VertexOutput_P4N4T4
{
	float4 position : SV_POSITION;
	float4 nw : NORMAL;
	float4 texcoord : TEXCOORD;
};

struct VertexInput_P3
{
	float3 position : POSITION;
};

struct VertexOutput_P4
{
	float4 position : SV_POSITION;
};