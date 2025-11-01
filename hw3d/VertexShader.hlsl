// Simple pass-through vertex shader
struct VSInput
{
    float3 pos : POSITION;
    float3 col : COLOR;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float3 col : COLOR;
};

PSInput main(VSInput input)
{
    PSInput output;
    output.pos = float4(input.pos, 1.0f);
    output.col = input.col;
    return output;
}
