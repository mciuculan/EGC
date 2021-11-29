#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;


void main()
{
    // TODO(student): Compute world space vectors
    vec3 vetrex_world_space_pos = vec3(Model * vec4(v_position, 1));
    vec3 light_dir = normalize(light_position - vetrex_world_space_pos);
    vec3 vertex_world_space_normal = normalize(vec3(Model * vec4(v_normal, 0)));
    vec3 vis_world_space = normalize(eye_position - vetrex_world_space_pos);
    vec3 median_world_space = normalize (light_dir + vis_world_space);

    // TODO(student): Define ambient light component
    float ambient_light = 0.25 * material_kd;

    // TODO(student): Compute diffuse light component
    float light_int = 2.5;
    float diffuse_light =  material_kd * light_int * max(dot(vertex_world_space_normal, light_dir), 0);

    // TODO(student): Compute specular light component
    float specular_light = 0;

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Gouraud (1971) shading method. There is also the Phong (1975) shading
    // method, which we'll use in the future. Don't mix them up!
    if (diffuse_light > 0)
    {
        vec3 R = reflect(-light_dir, vertex_world_space_normal);
	    specular_light = material_ks * light_int * pow(max(dot(vis_world_space, R), 0), material_shininess);
    }

    // TODO(student): Compute light
    float dist = distance(light_position, vetrex_world_space_pos);
	float aFactor = 1 / (dist * dist + 1);
	float light	= ambient_light + aFactor * (diffuse_light + specular_light);

    // TODO(student): Send color light output to fragment shader
    color = object_color * light;

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
