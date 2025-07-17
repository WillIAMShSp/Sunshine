
#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normals;

uniform mat4 u_Model = mat4(1.0);
uniform mat4 u_View;
uniform mat4 u_Projection;


void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
}  