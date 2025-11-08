struct PSInput
{
    float4 pos : SV_Position;
    float3 col : Color;
};

cbuffer CBuf //cbuffer (constant buffer) is a keyword in HLSL
{
    //matrix tranform; //matrix in HLSL means 4 by 4 AND column major 
    float4 face_colors[6]; //relates to the "semantic" SV_PrimitiveID below
};

float4 main(uint triangleID : SV_PrimitiveID) : SV_Target
{
    return face_colors[triangleID / 2];
    
    //return float4(1.0f, 1.0f, 1.0f, 1.0f); //white triangle
}
