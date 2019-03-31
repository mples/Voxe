cbuffer object_buffer
{
    row_major float4x4 mvp_matrix;
    row_major float4x4 model_matrix;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 tex_coord : TEX_COORD;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 out_pos : SV_POSITION;
    float2 out_tex_coord : TEX_COORD;
    float3 out_normal : NORMAL;
    float3 out_world_pos : WORLD_POS;
};

VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT output;
    output.out_pos = mul(float4(input.pos, 1.0f), mvp_matrix);
    output.out_tex_coord = input.tex_coord;
    output.out_normal = normalize(mul(float4(input.normal, 0.0f), model_matrix));
    output.out_world_pos = mul(float4(input.pos, 1.0f), model_matrix);
	return output;
}