cbuffer CBuf //passed in by TransformCbuf::Bind()
{
    matrix modelView;
    matrix modelViewProj;
};


struct VSOut
{
    float3 cameraPos : Position;
    float3 normal : Normal;
    float3 color : Color;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : POSITION, float3 n : Normal, float3 color : Color)
{
    VSOut vso;
    vso.cameraPos = (float3) mul(float4(pos, 1.0f), modelView);
    
    vso.normal = mul(n, (float3x3) modelView); //typecast here (model is 4 x 4)
    
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);

    vso.color = color;
    
    return vso;
}