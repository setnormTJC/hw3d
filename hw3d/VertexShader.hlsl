// Simple pass-through vertex shader
struct VSInput
{
    float2 pos : Position;
    float3 col : Color;
};

cbuffer CBuf //cbuffer (constant buffer) is a keyword in HLSL
{
    matrix tranform; //matrix in HLSL means 4 by 4 AND column major 
};

struct VSOut //pixel shader input 
{
    float4 pos : SV_Position; //system value
    float3 col : COLOR;
};

VSOut main(VSInput input)
{
    VSOut output;
    output.pos = mul(float4(input.pos, 1.0f, 1.0f), tranform);
    output.col = input.col;
    
    return output;
}
