#version 330 core
out vec4 FragColor;


    float near = 0.1; 
    float far  = 1000.0; 
  
    float LinearizeDepth(float depth) 
    {
        float z = depth * 2.0 - 1.0; // back to NDC 
        return (2.0 * near * far) / (far + near - z * (far - near));	
    }

void main()
{             
    //gl_FragDepth = gl_FragCoord.z;
    //FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    //float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    //FragColor = vec4(vec3(depth), 1.0);


    //gl_FragDepth = LinearizeDepth(gl_FragCoord.z) / far;

   


   







}  