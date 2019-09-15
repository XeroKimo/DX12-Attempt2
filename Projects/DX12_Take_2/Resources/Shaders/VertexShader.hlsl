struct VertexInput
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer constantBuffer : register(b0)
{
	float3 position;
}

PixelInput main( VertexInput input)
{
	PixelInput output;
	output.position = float4(input.position, 1.0f);
	output.position += float4(position, 0.0f);
	output.color = input.color;
	return output;
}