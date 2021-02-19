#vertex shader
#version 330 core 

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texIndex;

out vec2 v_texCoord;
out float v_texIndex;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * position;
   v_texIndex = texIndex;
   v_texCoord = texCoord;
};


#fragment shader
#version 330 core 

layout(location = 0) out vec4 color;

in vec2 v_texCoord;
in float v_texIndex;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_texCoord);
	color = texColor;
};