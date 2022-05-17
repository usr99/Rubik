# version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aColor;
layout(location = 3) in mat4 aTransform;

out vec4 Color;
out vec2 v_TexCoord;
flat out int InstanceID;

uniform mat4 u_MVP;

void main()
{
    Color = vec4(aColor, 1.0);

    gl_Position = u_MVP * aTransform * vec4(aPosition, 1.0);
    v_TexCoord = aTexCoord;

	InstanceID = gl_InstanceID;
};
