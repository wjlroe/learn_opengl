#version 330 core
struct Material {
  sampler2D diffuse;
  sampler2D specular;
  // sampler2D emission;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 direction;
  float cutoff;
  float outerCutoff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
  vec3 lightDir = normalize(light.position - FragPos);

  // ambient
  vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

  // diffuse
  vec3 norm = normalize(Normal);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

  // specular
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

  // spotlight
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutoff - light.outerCutoff;
  float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
  diffuse *= intensity;
  specular *= intensity;

  // vec3 emission = texture(material.emission, TexCoords).rgb;

  // attenuation
  float distance = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  vec3 result = ambient + diffuse + specular;
  FragColor = vec4(result, 1.0);

}

