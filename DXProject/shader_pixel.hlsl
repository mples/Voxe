cbuffer frame_buffer : register(b0) {
    float eyePos;
}

cbuffer light_buffer : register(b1) {
    float3 ambientColor;
    float ambientStrength;

    float3 diffuseColor;
    float diffuseStrength;

    float3 specularColor;
    float specularStrength;

    float3 lightPos;
    float lightRange;

    float lightAttenuationA;
    float lightAttenuationB;
    float lightAttenuationC;
    float pad;
}

struct PS_INPUT {
    float4 in_pos : SV_POSITION;
    float2 in_tex_coord : TEX_COORD;
    float3 in_normal : NORMAL;
    float3 in_world_pos : WORLD_POS;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET {
    float3 pixel_color = objTexture.Sample(objSamplerState, input.in_tex_coord);

    float3 ambient = ambientColor * ambientStrength;

    float3 light_vector = normalize(lightPos - input.in_world_pos);

    float light_distance = distance(lightPos, input.in_world_pos);

    float attenuation = 1 / (lightAttenuationA + lightAttenuationB * light_distance + lightAttenuationC * pow(light_distance, 2));

    float3 diffuse_factor = max(dot(light_vector, input.in_normal), 0);

    float3 diffuse = attenuation * diffuse_factor * diffuseStrength * diffuseColor;

    float3 to_eye = normalize(eyePos - input.in_world_pos);

    float3 reflection = reflect(-light_vector, input.in_normal);
    float spec_factor = pow(max(dot(reflection, to_eye), 0), specularStrength);

    float3 specular = attenuation * spec_factor * specularColor;

    float3 final_color = pixel_color * (ambient + diffuse + specular);
    return float4(final_color, 1.0f);
}