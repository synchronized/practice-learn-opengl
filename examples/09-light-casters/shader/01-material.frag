#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform mat4 view;

struct Material
{
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct AmbientLight
{
    vec3 ambient;
};
AmbientLight ambientLight;

struct DirectionLight
{
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
};
#define NR_DIRECTION_LIGHTS 1
uniform DirectionLight directionLights[NR_DIRECTION_LIGHTS];
int directionLightNum = 0;

struct PointLight
{
    vec3 position;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
int pointLightNum = 4;

struct FlashLight
{
    vec3 position;
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
    vec2 cutOff;
};
#define NR_FLASH_LIGHTS 1
uniform FlashLight flashLights[NR_FLASH_LIGHTS];
int flashLightNum = 0;

vec3 calc_direction_light(DirectionLight light, vec3 viewDir, vec3 norm, vec3 FragPos, vec3 material_diffuse, vec3 material_specular) {
    // diffuse 
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material_diffuse);
    
    // specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material_specular); 

    return diffuse + specular;
}

vec3 calc_point_light(PointLight light, vec3 viewDir, vec3 norm, vec3 FragPos, vec3 material_diffuse, vec3 material_specular) {
    vec3 lightPos = vec3(view * vec4(light.position, 1.0)); // Transform world-space light position to view-space light position
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0f / (light.attenuation.x + light.attenuation.y*distance + light.attenuation.z*(distance*distance));

    // diffuse
    vec3 pointLightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, pointLightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material_diffuse);
    
    // specular
    vec3 reflectDir = reflect(-pointLightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material_specular); 

    return (diffuse + specular) * attenuation;
}

vec3 calc_flash_light(FlashLight light, vec3 viewDir, vec3 norm, vec3 FragPos, vec3 material_diffuse, vec3 material_specular) {
    // Transform world-space light position to view-space light position
    vec3 lightPos = vec3(view * vec4(light.position, 1.0)); 
    vec3 lightShotDir = normalize(vec3(view * vec4(light.direction, 0.0)));
    vec3 lightDir = normalize(lightPos - FragPos);
    float theta = dot(lightDir, normalize(-lightShotDir));
    float epsilon = light.cutOff.x - light.cutOff.y;
    float intensity = clamp((theta - light.cutOff.y)/epsilon, 0.0f, 1.0f);

    float distance = length(lightPos - FragPos);
    float attenuation = 1.0f / (light.attenuation.x + light.attenuation.y*distance + light.attenuation.z*(distance*distance));

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material_diffuse);
    
    // specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material_specular); 

    return (diffuse + specular) * intensity * attenuation;
}

void main()
{
    vec3 result;
    //point light

    vec3 viewDir = normalize(-FragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    vec3 material_diffuse = vec3(texture(material.diffuse, TexCoords));
    vec3 material_specular = vec3(texture(material.specular, TexCoords));
    vec3 norm = normalize(Normal);

    // ambient
    vec3 ambient = ambientLight.ambient * material.ambient;
    result += ambient;

    //direction light
    for (int i=0; i<directionLightNum; i++) {
        result += calc_direction_light(directionLights[i], viewDir, norm, FragPos, material_diffuse, material_specular);
    }

    //point light
    for (int i=0; i<pointLightNum; i++) {
        result += calc_point_light(pointLights[i], viewDir, norm, FragPos, material_diffuse, material_specular);
    }

    for (int i=0; i<flashLightNum; i++) {
        result += calc_flash_light(flashLights[i], viewDir, norm, FragPos, material_diffuse, material_specular);
    }

    FragColor = vec4(result, 1.0);
}