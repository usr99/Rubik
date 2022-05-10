# version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aColor;
layout(location = 3) in mat4 aTransform;

out vec3 Normal;
out vec3 crntPos;
out vec4 Color;
out vec2 v_TexCoord;

uniform mat4 u_Camera;
uniform mat4 u_Model;

void main()
{
        vec3 aNormal = vec3(0.0, 0.0, 1.0);
        mat4 ModelViewMatrix = u_Model * aTransform;

        mat3 normalMatrix = mat3(ModelViewMatrix);
        normalMatrix = inverse(normalMatrix);
        normalMatrix = transpose(normalMatrix);
        Normal = normalize(normalMatrix * aNormal);

        crntPos = vec3(ModelViewMatrix * vec4(aPosition, 1.0));

        Color = vec4(aColor, 1.0);

        gl_Position = u_Camera * vec4(crntPos, 1.0);
        v_TexCoord = aTexCoord;
};
