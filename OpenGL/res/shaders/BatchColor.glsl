#shader vertex
#version 330 core 

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 color;

uniform mat4 u_MVP;

out vec4 v_Color;

void main()
{
   gl_Position = u_MVP * pos;
   v_Color = color;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;

void main()
{
	o_Color = v_Color;
}