struct VS_Output
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

VS_Output main(float3 inPosition : POSITION, float2 inTexcoord : TEXCOORD)
{
	VS_Output output;
	output.position = float4(inPosition, 1.0f);
	output.texcoord = inTexcoord;

	return output;
}