#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

// uniform vec3 objectColor;
// uniform vec3 lightColor;
// uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main()
{
    float ambientStrength = 0.1f;
    float specularStrength = 0.5f;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    // ambient light cal.
    vec3 ambient = light.ambient * vec3(texture2D(material.diffuse, TexCoords));

    // diffuse light cal.
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * vec3(texture2D(material.diffuse, TexCoords));

    // specular light cal.
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture2D(material.specular, TexCoords));

    // final lighting.
    vec3 result = ambient + diffuse + specular + vec3(texture2D(material.emission, TexCoords));
    color = vec4(result, 1.0f);
    //color = texture2D(material.diffuse, TexCoords);
}
