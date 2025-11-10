cbuffer CBuf
{
    matrix transform;
}

/*Position semantic and colon MUST be used - else no match in input layout */ 
float4 main(float3 pos : Position) : SV_Position
{
    /*SV_Position semantic MUST be used here*/
    return mul(float4(pos, 1.0f), transform);

}