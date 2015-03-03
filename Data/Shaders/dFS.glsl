#version 150

in vec2 textureCoord;
out vec4 outColor;

uniform sampler2D textu;

void main()
{
	outColor = vec4(textureCoord.y,1,0,1);
}
