#version 330 core
layout (location = 0) in vec3 position; // 位置变量的属性位置值为 0
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec3 vertexCalulatedLightColor;

// lighting in world space.
void main()
{

    vec3 worldNormal = mat3(transpose(inverse(model))) * normal;
    vec3 worldVertexPos = vec3(model * vec4(position, 1.0f));
    vec3 worldLightPos = lightPos;                   // lightPos that pass in was worldSpace.
    vec3 worldViewPos = vec3(model * vec4(viewPos, 1.0f));

    float ambientStrength =  0.1f;
    float specularStrength = 0.5f;

    // calulate ambient light.
    vec3 ambient = ambientStrength * lightColor;

    // caluate diffuse light.
    vec3 lightDir = normalize(worldLightPos - worldVertexPos);
    vec3 diffuse = max(dot(lightDir, worldNormal), 0) * lightColor;

    // caluate specular light.
    vec3 viewDir = normalize(worldViewPos - worldVertexPos);
    vec3 refectDir = reflect(-lightDir, worldNormal);
    vec3 specular = specularStrength * pow(max(dot(viewDir, refectDir), 0), 32) * lightColor;

    gl_Position = projection * view * model * vec4(position, 1.0f);
    vertexCalulatedLightColor = (ambient +  specular + diffuse) * lightColor;
}

