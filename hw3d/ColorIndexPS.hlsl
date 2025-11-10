cbuffer CBuf
{
    float4 face_colors[8]; //hard-coded value here
}

float4 main(uint tid : SV_PrimitiveID) : SV_Target 
/*SV_Target must (most) always be here for pixel shader*/
{
    return face_colors[(tid / 2) % 8]; //again, more hard-coded stuff
}