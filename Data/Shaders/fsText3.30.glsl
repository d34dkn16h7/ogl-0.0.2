#version 330

in vec2 textureCoord;
out vec4 outColor;

uniform sampler2D textu;

void main()
{
	outColor = vec4(1,1,1,1);
}
