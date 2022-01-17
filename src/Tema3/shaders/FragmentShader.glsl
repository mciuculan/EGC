#version 330

// Input
in vec2 texcoord;
in vec3 world_position;
in vec3 world_normal;

in float elapsedTime_;

// Uniform properties
uniform vec3 color;
uniform sampler2D myTexture;
uniform vec3 floorLightsPos[9];
uniform vec3 floorLightsColor[9];

uniform vec3 ceilingLightsPos[6];
uniform vec3 ceilingLightsColor[6];
uniform vec3 ceilingLightsDir[6];

uniform vec3 eye_position;
uniform float material_kd, material_ks, material_shininess;
uniform int mode;
uniform bool textured;

uniform vec3 ballPos;
uniform bool transparent;

// Output
layout(location = 0) out vec4 out_color;

vec3 floorLightContribution(vec3 floorLightPos, vec3 floorLightColor) {
    vec3 L = normalize(floorLightPos - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 R = reflect(-L, world_normal);

    float intensity = 2.5;

    float diffuse_light =  material_kd * intensity * max(dot(world_normal, L), 0);
    float specular_light = 0;

    if (diffuse_light > 0)
        specular_light = material_ks * intensity * pow(max(dot(V, R), 0), material_shininess);

    float aFactor = 0.0f;
    float dist = distance(world_position, floorLightPos);

    if (dist < 1.5)
	    aFactor = pow(1.5 - dist, 2);

    return floorLightColor * aFactor * (diffuse_light + specular_light);
}

vec3 ceilingLightContribution(vec3 ceilingLightPos, vec3 ceilingLightColor, vec3 ceilingLightDir) {
    vec3 L = normalize(ceilingLightPos - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    vec3 N = world_normal;
    vec3 R = reflect(-L, N);

     float intensity = 7.5;

    float diffuse_light =  material_kd * intensity * max(dot(N, L), 0);
    float specular_light = 0;

    if (diffuse_light > 0)
    {
        specular_light = material_ks * intensity * pow(max(dot(V, R), 0), material_shininess);
    }
   
    float aFactor;
    float cut_off = radians(15.0f);
    float spot_light = dot(-L, ceilingLightDir);
    float spot_light_limit = cos(cut_off);
    
    if (spot_light > cos(cut_off))
    {
        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        float light_att_factor = pow(linear_att, 2);
        aFactor = light_att_factor * (diffuse_light + specular_light);
    }

    float dist = distance(ceilingLightPos, world_position);
    
    return ceilingLightColor * aFactor * (diffuse_light + specular_light) / (dist * dist + 1);
}

vec3 ballLightContribution(vec3 ballPos) {
    vec2 texcoord_;
    vec3 light_dir = world_position - ballPos;

    texcoord_.x = (1.0 / (2 * 3.14159)) * atan (light_dir.x, light_dir.z) - 0.075 * elapsedTime_;
    texcoord_.y = (1.0 / 3.14159) * acos (light_dir.y / length (light_dir));

    vec3 color = texture2D(myTexture, texcoord_).xyz;

    vec3 N = normalize(world_normal);
    vec3 V = normalize(eye_position - world_position);
    vec3 L = normalize(ballPos - world_position);
    vec3 H = normalize(L + V);

    float dist = distance(ballPos, world_position);
    float intensity = 5;

    float diffuse_light = material_kd * intensity * max(dot(N, L), 0);
    float specular_light = 0;

    if (diffuse_light > 0)
        specular_light = material_ks * intensity * pow(max(dot(N, H), 0), material_shininess);

    return color * (diffuse_light + specular_light) / (dist * dist + 1);
}

void main()
{
    if (textured)
        out_color = texture2D(myTexture, texcoord);
    else {
        vec3 currColor = vec3(0, 0, 0);

        if (mode == 0 || mode == 1 || mode == 2)
            currColor = color;

        if (mode == 0 || mode == 1)
            for (int i = 0; i < 9; i++)
                currColor += floorLightContribution(floorLightsPos[i], floorLightsColor[i]);

        if (mode == 0 || mode == 2)
            for (int i = 0; i < 6; i++)
                currColor += ceilingLightContribution(ceilingLightsPos[i], ceilingLightsColor[i], ceilingLightsDir[i]);

        if (mode == 0 || mode == 3)
            currColor += ballLightContribution(ballPos);

        if (transparent)
            out_color = vec4(currColor, 0.5);
        else
            out_color = vec4(currColor, 1);
    }
}
