#version 330 core

out vec4 FragColor; // Color de salida del fragmento

in vec3 crntPos;    // Posición actual del fragmento
in vec3 normal;     // Normal del fragmento
in vec2 texCoord;   // Coordenadas de textura

uniform sampler2D diffuse0;  // Textura difusa
uniform sampler2D specular0; // Textura especular

uniform vec4 lightColor;      // Color de la luz
uniform vec3 lightPos;        // Posición de la luz
uniform vec3 camPos;          // Posición de la cámara

void main()
{
    // Calcular la dirección de la luz
    vec3 lightDirection = normalize(lightPos - crntPos);

    // Calcular la luz ambiente
    vec3 ambient = 0.1 * lightColor.rgb; // Valor constante para la luz ambiental

    // Calcular la intensidad de la luz difusa
    float diffuseStrength = max(dot(normalize(normal), lightDirection), 0.0);
    vec3 diffuse = diffuseStrength * lightColor.rgb;

    // Calcular la luz especular
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflection = reflect(-lightDirection, normalize(normal));
    float spec = pow(max(dot(viewDirection, reflection), 0.0), 16); // Exponente del brillo especular
    vec3 specular = 0.5 * spec * lightColor.rgb; // Valor constante para la luz especular

    // Obtener colores de las texturas
    vec3 diffuseColor = texture(diffuse0, texCoord).rgb;   // Color de la textura difusa
    vec3 specularColor = texture(specular0, texCoord).rgb; // Color de la textura especular

    // Combinación final del color
    vec3 finalColor = ambient + diffuse * diffuseColor + specular * specularColor;

    FragColor = vec4(finalColor, 1.0); // Color final del fragmento
}
