//type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec4 a_Color;

out vec2 v_TexCoord;
out vec4 v_Color;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;
uniform mat4 u_ModelViewProjection;

void main()
{
    v_TexCoord = a_TexCoord;
    v_Color = a_Color;
    gl_Position = u_ModelViewProjection * vec4(a_Position, 1.0f);
}

//type fragment 
#version 330 core

layout (location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;

uniform sampler2D u_Texture;
uniform vec4 u_ExtraColor;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    if (texColor == vec4(1.0))
    { 
      color = v_Color; 
    }
    else
    {
        color = texColor * u_ExtraColor;
    }
}    