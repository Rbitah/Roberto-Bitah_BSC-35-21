 #version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 viewPos;

// Material
uniform sampler2D ourTexture;

// Directional light
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

// Point light
struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform PointLight pointLight;

void main()
{
    // Normalized vectors
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Directional Light 
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 ambient = dirLight.ambient * vec3(texture(ourTexture, TexCoord));
    vec3 diffuse = dirLight.diffuse * diff * vec3(texture(ourTexture, TexCoord));
    vec3 specular = dirLight.specular * spec;

    // Point Light
    vec3 pointLightDir = normalize(pointLight.position - FragPos);
    float pointDiff = max(dot(norm, pointLightDir), 0.0);
    vec3 pointReflect = reflect(-pointLightDir, norm);
    float pointSpec = pow(max(dot(viewDir, pointReflect), 0.0), 32.0);

    vec3 pointAmbient = pointLight.ambient * vec3(texture(ourTexture, TexCoord));
    vec3 pointDiffuse = pointLight.diffuse * pointDiff * vec3(texture(ourTexture, TexCoord));
    vec3 pointSpecular = pointLight.specular * pointSpec;

    vec3 result = ambient + diffuse + specular + pointAmbient + pointDiffuse + pointSpecular;
    FragColor = vec4(result, 1.0);
}
