# version 330 core

layout(location = 0) out vec4 color;

in vec4 Color;
in vec2 v_TexCoord;
flat in int InstanceID;

uniform sampler2D u_Texture;
uniform sampler2D u_LogoTexture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
	
    color = texColor;
	if (InstanceID == 4 && color.a != 1)
	{
		color = texture(u_LogoTexture, v_TexCoord);
	}

	if (color.a != 1.0)
	{
		color = Color;
	}
};
