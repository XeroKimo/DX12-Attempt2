

SamplerState s1 : register (s0);
Texture2D t1 : register(t0);

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

float4 main(PixelInput input) : SV_TARGET
{
	return t1.Sample(s1, input.uv);
}