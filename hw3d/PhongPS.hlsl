//0123456789a0123456789b0123456789c0123456789d0123456789e0123456789f0123456789g0123456789h0123456789g0123456789h0
cbuffer LightCBuf //set per frame (slot 0)
{
    float3 lightPos; 
    //float3 materialColor = { 0.7f, 0.7f, 0.7f }; //gray

    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;

    float attConst; //attenuation constant 
    float attLin; //linear
    float attQuad ; //quadratic 
};

cbuffer ObjectCBuf //set per object (slot 1) 
{
    float3 materialColor; 
    float specularIntensity; 
    float specularPower; 
};



//for some detail on how the above parameters might be chosen (for range of "100" in example above), see: 
//https://wiki.ogre3d.org/-Point+Light+Attenuation

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
    const float3 vToL = lightPos - worldPos; 
    
    const float distToL = length(vToL); //"geometric" length (vector magnitude) - not array length
    //length is referred to as an "intrisic" function in HLSLf
    
    const float3 dirToL = vToL / distToL; //NORMALIZED 
    
    //diffuse attenuation: 
    const float att = 1.0f/(attConst + attLin * distToL + attQuad*(distToL * distToL));
    //diffuse intensity: 
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n)); 
    //dot intrinsic function here - * means "element-wise" (Hadamard) multiplication
    
    //specular things (how "shiny" is an object?)
    const float3 w = n * dot(vToL, n); 
    const float3 r = w * 2.0f - vToL; //r as in "reflected" light
    
    //reflect(); //an "intrinsic" that can do the above
    
    const float3 specular = att*(diffuseColor * diffuseIntensity) *
                    specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);
    
    
    //final color 
    return float4(saturate(diffuse + ambient + specular) * materialColor, 1.0f);
    //interestingly, "saturate" function clamps output between 0.0f and 1.0f

}