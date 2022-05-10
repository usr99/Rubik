# version 330 core

layout(location = 0) out vec4 color;

in vec3 Normal;
in vec3 crntPos;
in vec4 Color;
in vec2 v_TexCoord;

uniform vec3 u_LightPosition;
uniform sampler2D u_Texture;

void main()
{
        float ambient = 0.2;

        vec3 norm = normalize(Normal);
        vec3 lightDirection = normalize(u_LightPosition - crntPos);

        float diffuse = max(dot(norm, lightDirection), 0.0);

        vec4 texColor = texture(u_Texture, v_TexCoord);

        if (texColor.a != 0.0)
        {
                color = texColor * (diffuse + ambient);
        }
        else
        {
                color = Color * (diffuse + ambient);
        }
        color.a = 1.0;
};
