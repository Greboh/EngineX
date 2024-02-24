//type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec3 v_Position;
out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    v_Position = a_Position;
    v_TexCoord = a_TexCoord;
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0f);
}

//type fragment 
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec2 v_TexCoord;

void main()
{
    color = vec4(v_TexCoord, 0.0f, 1.0f);
}    