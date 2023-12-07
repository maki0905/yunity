struct VertexShaderOutput
{
    float4 position : SV_POSITION;
};

struct VertexShaderInput
{
    float4 position : POSITIONT0;
};


VertexShaderOutput main( VertexShaderInput input ) : SV_POSITION
{
    VertexShaderOutput output;
    output.position = input.position;
    return output;
}