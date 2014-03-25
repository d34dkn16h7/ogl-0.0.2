#version 150

in vec3 col;
out vec4 outColor;

uniform sampler2D textu;

void main()
{	
	outColor = vec4(col.x,col.y,col.z,1);
}