#version 330 core
in vec3 ourColor;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos; // in view space.
// uniform vec3 viewPos; // in view space view posi must vec3(0 0 0)

vec3 viewPos = vec3(0.0f, 0.0f, 0.0f);

void main()
{
    float ambientStrength = 0.1f;
    float specularStrength = 0.5f;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // ambient light cal.
    vec3 ambient = ambientStrength * lightColor;

    // diffuse light cal.
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    // specular light cal.
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0), 32);
    vec3 specular = spec * specularStrength * lightColor;


    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0f);
}
