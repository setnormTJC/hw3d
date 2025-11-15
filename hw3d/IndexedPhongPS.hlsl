cbuffer LightCBuf //set per frame (slot 0)
{
    float3 lightPos;

    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;

    float attConst;
    float attLin; 
    float attQuad; 
};

cbuffer ObjectCBuf //set per object (slot 1) 
{
    float3 materialColors[6];
    float padding; //used only to guarantee the 16 byte width requirement 
    float specularIntensity;
    float specularPower;
};



//for some detail on how the above parameters might be chosen (for range of "100" in example above), see: 
//https://wiki.ogre3d.org/-Point+Light+Attenuation

float4 main(float3 worldPos : Position, float3 n : Normal, uint tid : SV_PrimitiveID) : SV_Target
//NOTE: SV_PrimitiveID requires "Shader Model 5.0 
//ALSO note: SV_PrimitiveID REDUCES the graphics debugger's capabilities
{
    const float3 vToL = lightPos - worldPos;
    
    const float distToL = length(vToL); 
  
    const float3 dirToL = vToL / distToL; 
    
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
    
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
 
    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.0f - vToL; //r as in "reflected" light
    
    //reflect(); //an "intrinsic" that can do the above
    
    const float3 specular = att * (diffuseColor * diffuseIntensity) *
                    specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);
    
    //final color 
    return float4(saturate(diffuse + ambient + specular) * materialColors[tid % 6], 1.0f);
}