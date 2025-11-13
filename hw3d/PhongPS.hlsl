cbuffer LightCBuf
{
    float3 lightPos; 
};


static const float3 ambient = { 0.15f, 0.15f, 0.15f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float diffuseIntensity = 1.0f; 
static const float attConst = 1.0f; //attenuation constant 
static const float attLin = 1.0f; //linear
static const float attQuad = 1.0f; //quadratic 

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
    const float3 vToL = lightPos - worldPos; 
    
    const float distToL = length(vToL); //"geometric" length - not array length
    //length is referred to as an "intrisic" function in HLSLf
    
    const float3 dirToL = vToL / distToL;
    
    //diffuse attenuation: 
    const float att = attConst + attLin * distToL + attQuad*(distToL * distToL);
    //diffuse intensity: 
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
    
    //final color 
    
    return float4(saturate(diffuse + ambient), 1.0f);
    //interestingly, "saturate" function clamps output between 0.0f and 1.0f

}