#version 400 core

layout(location = 0) in vec3 position; // Position of the vertex
layout(location = 1) in vec3 normal;   // Normal of the vertex
layout(location = 5) in vec2 texCoord; // UV texture coordinates
layout(location = 10) in float arrowOffset; // Sideways offset for billboarded normal arrows

out vec2 texc;
out float depth;
out vec4 position_cameraSpaceF;
out vec4 normal_cameraSpaceF;
out vec4 position_worldSpaceF;
out vec4 normal_worldSpaceF;
out mat4 vF;

// Transformation matrices
uniform mat4 p;
uniform mat4 v;
uniform mat4 m;


void main()
{
    texc = texCoord;

    vec4 position_cameraSpace = v * m * vec4(position, 1.0);
    vec4 normal_cameraSpace = vec4(normalize(mat3(transpose(inverse(v * m))) * normal), 0);

    vec4 position_worldSpace = m * vec4(position, 1.0);
    vec4 normal_worldSpace = vec4(normalize(mat3(transpose(inverse(m))) * normal), 0);

    gl_Position = p * position_cameraSpace;

    vF = v;

    position_cameraSpaceF = position_cameraSpace;
    normal_cameraSpaceF = normal_cameraSpace;
    position_worldSpaceF = position_worldSpace;
    normal_worldSpaceF = normal_worldSpace;

    // DOF ==============================================

    depth = clamp(-position_cameraSpace.z / 5., 0.0, 1.0);
    depth = 1.0 - depth;

    // ==================================================
}
