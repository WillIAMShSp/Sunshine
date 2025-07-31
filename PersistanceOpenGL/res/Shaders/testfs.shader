#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_screentexture;
uniform sampler2D u_depthtexture;

uniform int u_isdepth = 1;

void main()
{ 
    
    
    float depth = texture(u_depthtexture, TexCoords).r;


    vec4 finalcolor = vec4(vec3(depth), 1.0);


    FragColor = finalcolor;

}