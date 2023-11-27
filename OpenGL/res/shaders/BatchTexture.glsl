#shader vertex
#version 330 core 

layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 textureCoord;

uniform mat4 u_MVP;

out vec2 v_TexCoord;

void main()
{
   gl_Position = u_MVP * pos;
   v_TexCoord = textureCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

void main()
{
	// o_Color = v_Color;
	o_Color = vec4(v_TexCoord, 0.0, 1.0);
}