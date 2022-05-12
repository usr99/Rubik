# version 330 core

layout(location = 0) out vec4 color;

in vec4 Color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
	
    if (texColor.a == 1.0)
    {
        color = texColor;
    }
    else
    {
        color = Color;
    }
};
