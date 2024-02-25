//type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec3 v_Position;
out vec4 v_Color;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    v_Position = a_Position;
    v_Color = a_Color;
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0f);
}

//type fragment 
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec4 v_Color;

void main()
{
    color = v_Color;
}    