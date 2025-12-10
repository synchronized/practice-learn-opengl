#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform mat4 view;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    float shininess;
};
uniform Material material;

struct DirectionLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionLight dirLight;

vec3 calc_direction_light(DirectionLight light, vec3 viewDir, vec3 norm, vec3 material_diffuse, vec3 material_specular) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material_diffuse);

    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material_specular); 

    return light.ambient * material_diffuse + diffuse + specular;
}

vec3 sample_diffuse()
{
    vec3 d1 = texture(material.texture_diffuse1, TexCoords).rgb;
    vec3 d2 = texture(material.texture_diffuse2, TexCoords).rgb;
    return length(d2) > 0.0 ? d1 + d2 : d1;
}

vec3 sample_specular()
{
    vec3 s1 = texture(material.texture_specular1, TexCoords).rgb;
    vec3 s2 = texture(material.texture_specular2, TexCoords).rgb;
    return length(s2) > 0.0 ? s1 + s2 : s1;
}

void main()
{
    vec3 viewDir = normalize(-FragPos); // view-space camera at origin
    vec3 norm = normalize(Normal);

    vec3 material_diffuse = sample_diffuse();
    vec3 material_specular = sample_specular();

    vec3 result = calc_direction_light(dirLight, viewDir, norm, material_diffuse, material_specular);
    FragColor = vec4(result, 1.0);
}