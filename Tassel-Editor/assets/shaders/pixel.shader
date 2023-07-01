#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out int entity;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexSlot;
in flat int v_EntityID;

uniform sampler2D u_Texture[32];

void main()
{
	color = texture(u_Texture[int(v_TexSlot)], v_TexCoord) * v_Color;
	entity = v_EntityID;
}
