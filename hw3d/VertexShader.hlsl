// Simple pass-through vertex shader
struct VSInput
{
    float2 pos : Position;
    float3 col : Color;
};

struct VSOut //pixel shader input 
{
    float4 pos : SV_Position; //system value
    float3 col : COLOR;
};

VSOut main(VSInput input)
{
    VSOut output;
    output.pos = float4(input.pos, 1.0f, 1.0f);
    output.col = input.col;
    
    return output;
}
