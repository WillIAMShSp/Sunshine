#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;


uniform mat4 u_Model = mat4(1.0);
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform int amountoflights = 0;



void main()
{


    gl_Position = u_Projection *  u_View  * u_Model * vec4(aPos,0.0, 1.0); 

    TexCoords = aTexCoords;
}  