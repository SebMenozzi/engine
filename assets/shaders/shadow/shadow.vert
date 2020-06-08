#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTextureCoords;

out vec2 fragmentTextureCoords;

out VS_OUT {
    vec3 fragmentPosition;
    vec3 fragmentNormal;
    vec2 fragmentTextureCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform bool reverseNormals;

void main()
{
    vs_out.fragmentPosition = vec3(model * vec4(vertexPosition, 1.0));
    if(reverseNormals) // a slight hack to make sure the outer large cube displays lighting from the 'inside' instead of the default 'outside'.
        vs_out.fragmentNormal = transpose(inverse(mat3(model))) * (-1.0 * vertexNormal);
    else
        vs_out.fragmentNormal = transpose(inverse(mat3(model))) * vertexNormal;
    vs_out.fragmentTextureCoords = vertexTextureCoords;
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
}
