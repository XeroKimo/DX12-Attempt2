struct VertexInput
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

cbuffer constantBuffer : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projMatrix;
}

PixelInput main( VertexInput input)
{
	PixelInput output;
	output.position = float4(input.position, 1.0f);
	//output.position = mul(worldMatrix, output.position);
	//output.position = mul(viewMatrix, output.position);
	//output.position = mul(projMatrix, output.position);
	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projMatrix);
	output.color = input.color;
	output.uv = input.uv;
	return output;
}