#shader vertex
#version 330 core 

layout(location = 0) in vec4 pos;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_MVP;

out vec2 v_TextureCoord;

void main()
{
   gl_Position = u_MVP * pos;
   v_TextureCoord = texCoord;
}

#shader fragment
#version 330 core

// uniform vec4 u_Color;
uniform sampler2D u_Texture;

layout(location = 0) out vec4 color;

in vec2 v_TextureCoord;

void main()
{
	vec4 texColor = texture(u_Texture, v_TextureCoord);
	color = texColor;
	// color = u_Color;
}
