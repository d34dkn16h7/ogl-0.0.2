#version 150

uniform mat4 cameraMatrix;
uniform mat4 modelMatrix;

in vec2 textureCoordIn;
in vec4 vertexIn;
out vec3 pos;
out vec2 textureCoord;

void main()
{
    gl_Position = cameraMatrix * modelMatrix * vertexIn;
    textureCoord = textureCoordIn;
}
