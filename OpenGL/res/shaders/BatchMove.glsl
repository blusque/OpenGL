#shader vertex
#version 330 core 

layout(location = 0) in vec4 pos;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * pos;
}

#shader fragment
#version 330 core

uniform vec4 u_Color;

layout(location = 0) out vec4 o_Color;

void main()
{
	o_Color = u_Color;
}