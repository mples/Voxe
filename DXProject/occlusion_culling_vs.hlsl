cbuffer object_buffer
{
    row_major float4x4 mvp_matrix;
    row_major float4x4 model_matrix;
};

struct VS_INPUT {
    float3 pos : POSITION;
};

struct VS_OUTPUT {
    float4 pos : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT output;
    output.pos = mul(float4(input.pos, 1.0f), mvp_matrix);
    return output;
}