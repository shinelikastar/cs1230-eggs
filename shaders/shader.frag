#version 400 core

in vec2 texc;
in float depth;
in vec4 position_cameraSpaceF;
in vec4 normal_cameraSpaceF;
in vec4 position_worldSpaceF;
in vec4 normal_worldSpaceF;
in mat4 vF;

layout(location=0) out vec4 fragColor;
layout(location=1) out vec4 curDepth;

uniform int useTexture = 0;
uniform sampler2D tex;
uniform float repeatU;
uniform float repeatV;

// Light data
const int MAX_LIGHTS = 10;
uniform int lightTypes[MAX_LIGHTS];         // 0 for point, 1 for directional
uniform vec3 lightPositions[MAX_LIGHTS];    // For point lights
uniform vec3 lightDirections[MAX_LIGHTS];   // For directional lights
//uniform vec3 lightAttenuations[MAX_LIGHTS]; // Constant, linear, and quadratic term
uniform vec3 lightColors[MAX_LIGHTS];

// Material data
uniform vec3 ambient_color;
uniform vec3 diffuse_color;
uniform vec3 specular_color;
uniform float shininess;

uniform bool useLighting;     // Whether to calculate lighting using lighting equation
uniform bool useBumpMap = false;

uniform sampler2D bump;

uniform float repeatBumpU;
uniform float repeatBumpV;

vec3 color;
vec3 texColor;

void phongShading() {
    if (useLighting)
    {
        color = ambient_color.xyz; // Add ambient component

        vec2 repeatCoord = vec2(0);
        repeatCoord.x = fract(texc.x * repeatBumpU);
        repeatCoord.y = fract(texc.y * repeatBumpV);
        float bumpVal = texture(bump, repeatCoord).r;

        vec4 new_position_worldSpaceF = position_worldSpaceF;
        if (useBumpMap) {
            new_position_worldSpaceF = position_worldSpaceF + (bumpVal * normal_worldSpaceF);
        }

        for (int i = 0; i < MAX_LIGHTS; i++) {
            vec4 vertexToLight = vec4(0);
            // Point Light
            if (lightTypes[i] == 0) {
                vertexToLight = normalize(vec4(lightPositions[i], 1) - new_position_worldSpaceF);
            } else if (lightTypes[i] == 1) {
                // Dir Light
                vertexToLight = normalize(vec4(-lightDirections[i], 0.f));
            }

            // Add diffuse component
            float diffuseIntensity = max(0.0, dot(vertexToLight, normal_worldSpaceF));
            vec3 mixed_diffuse = diffuse_color;
            if(useTexture == 1) {
                mixed_diffuse = mix(diffuse_color, texColor, 0.8);
            }

            color += max(vec3(0), lightColors[i] * mixed_diffuse * diffuseIntensity);

            // Add specular component
            vec4 lightReflection = normalize(-reflect(vertexToLight, normal_worldSpaceF));
            vec4 eyeDirection = normalize(vec4(0,0,0,1) - position_cameraSpaceF);
            float specIntensity = pow(max(0.0, dot(eyeDirection, vF * lightReflection)), shininess);
//            color += max (vec3(0), lightColors[i] * specular_color * specIntensity);
        }
    }
    else
    {
        color = ambient_color + diffuse_color;
    }

    color = clamp(color, 0.0, 1.0);
}

void main(){
    vec2 repeatCoord = vec2(0);
    repeatCoord.x = fract(texc.x * repeatU);
    repeatCoord.y = fract(texc.y * repeatV);

    texColor = vec3(0);

    if (useTexture == 1) {
        texColor = clamp(texture(tex, repeatCoord), 0, 1).rgb;
    }

    phongShading();

    curDepth = vec4(depth);

    fragColor = vec4(color, 1);
}
