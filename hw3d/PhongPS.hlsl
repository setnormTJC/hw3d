//0123456789a0123456789b0123456789c0123456789d0123456789e0123456789f0123456789g0123456789h0123456789g0123456789h0
cbuffer LightCBuf
{
    float3 lightPos; 
};


static const float3 materialColor = { 0.7f, 0.7f, 0.7f }; //gray

static const float3 ambient = { 0.05f, 0.05f, 0.05f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float diffuseIntensity = 1.0f; 
static const float attConst = 1.0f; //attenuation constant 
static const float attLin = 0.045f; //linear
static const float attQuad = 0.0075f; //quadratic 

//for some detail on how the above parameters might be chosen (for range of "100" in example above), see: 
//https://wiki.ogre3d.org/-Point+Light+Attenuation

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
    const float3 vToL = lightPos - worldPos; 
    
    const float distToL = length(vToL); //"geometric" length - not array length
    //length is referred to as an "intrisic" function in HLSLf
    
    const float3 dirToL = vToL / distToL;
    
    //diffuse attenuation: 
    const float att = 1.0f/(attConst + attLin * distToL + attQuad*(distToL * distToL));
    //diffuse intensity: 
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n)); 
    //dot intrinsic function here - * means "element-wise" (Hadamard) multiplication
    
    //final color 
    
    return float4(saturate(diffuse + ambient) * materialColor, 1.0f);
    //interestingly, "saturate" function clamps output between 0.0f and 1.0f

}