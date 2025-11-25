cbuffer LightCBuf //set per frame (slot 0)
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    
    float diffuseIntensity;
    float attConst; //attenuation constant 
    float attLin; //linear
    float attQuad; //quadratic 
};


Texture2D tex;
Texture2D spec;

SamplerState splr;


//for some detail on how the above parameters might be chosen (for range of "100" in example above), see: 
//https://wiki.ogre3d.org/-Point+Light+Attenuation

float4 main(float3 worldPos : Position, float3 n : Normal, float2 tc : Texcoord) : SV_Target
{
    const float3 vToL = lightPos - worldPos;
    
    const float distToL = length(vToL); //"geometric" length (vector magnitude) - not array length
    //length is referred to as an "intrisic" function in HLSLf
    
    const float3 dirToL = vToL / distToL; //NORMALIZED 
    
    //diffuse attenuation: 
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
    //diffuse intensity: 
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
    //dot intrinsic function here - * means "element-wise" (Hadamard) multiplication
    
    //specular things (how "shiny" is an object?)
    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.0f - vToL; //r as in "reflected" light
    
 
    const float4 specularSample = spec.Sample(splr, tc);
    const float3 specularReflectionColor = specularSample.rgb;
    const float specularPower = pow(2.0f, specularSample.a * 13.0f);// magic number here
    const float3 specular = att * (diffuseColor * diffuseIntensity) * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);
	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular * specularReflectionColor), 1.0f);
    
    //interestingly, "saturate" function clamps output between 0.0f and 1.0f

}