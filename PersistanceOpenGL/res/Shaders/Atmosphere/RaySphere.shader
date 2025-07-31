#version 330 core
#define PI 3.1415926535897932384626433832795
#define RAY_BETA vec3(5.5e-6, 13.0e-6, 22.4e-6) /* rayleigh, affects the color of the sky */
#define MIE_BETA vec3(21e-6) /* mie, affects the color of the blob around the sun */
#define AMBIENT_BETA vec3(0.0) /* ambient, affects the scattering color when there is no lighting from the sun */
#define ABSORPTION_BETA vec3(2.04e-5, 4.97e-5, 1.95e-6) /* what color gets absorbed by the atmosphere (Due to things like ozone) */
#define G 0.7 /* mie scattering direction, or how big the blob around the sun is */
// and the heights (how far to go up before the scattering has no effect)
#define HEIGHT_RAY 8e3 /* rayleigh height */
#define HEIGHT_MIE 1.2e3 /* and mie */
#define HEIGHT_ABSORPTION 30e3 /* at what height the absorption is at it's maximum */
#define ABSORPTION_FALLOFF 4e3 /* how much the absorption decreases the further away it gets from the maximum height */

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D u_screentexture;

uniform sampler2D u_depthtexture;

uniform vec3 u_pos;
uniform mat4 u_inverseview;
uniform mat4 u_inverseproj;



float screenwidth = 1920;
float screenheight = 1080;
float aspectratio = screenwidth/screenheight;


uniform vec3 u_lightposition = vec3(0.0,10.0,0.0);
//uniform vec3 u_invwavelength = vec3(1.0);
uniform vec3 u_wavelengths = vec3(1.0);
uniform vec3 u_scatteringcoefficients = vec3(1.0);


uniform float u_densityfalloff = 1;
uniform float u_atmosphereradius = 1.0;
uniform float u_planetradius = 0.5;
uniform vec3 u_planetcentre = vec3(0.0);
uniform float u_scaleheight = 0.25;

uniform int u_inscatterpoints = 10;
uniform int u_opticaldepthpoints = 10;


vec3 dirtosun = vec3(0,1,0);

float sunintensity = 40;




vec2 raysphere(vec3 origin, vec3 direction, float sphereradius)
{
    float a = dot(direction, direction);

	float b = 2 * dot(origin, direction);

	float c = dot(origin, origin) - sphereradius * sphereradius;

    float closeT;
    float farT;


    float discriminant = b*b - 4*a*c;

    if(discriminant >= 0)
    {
        
        float s = sqrt(discriminant);


        closeT = max(0,(-b - s)/(2*a));
        farT = (-b + s)/(2*a);

        if(farT >= 0)
        {
            return vec2(closeT, farT);

        }

        

    }
    
    return vec2(1./0., 0);
    
}


float n = 1.0003;
float Ns = 2.545 * pow(10,25);


float calcK()
{
    float k = (2*PI* (pow(n,4) - 2*n*n +1))/(3*Ns);

    return k;
}



float densityatpoint(vec3 point)
{
    float height = (length(point) - u_planetradius)/ u_atmosphereradius - u_planetradius;

    float density = exp(-height/u_scaleheight);

    return density;
    
}


vec3 opticaldepth(vec3 origin, vec3 raydirection, float raylength)
{


    return vec3(0.0);



}





















void main()
{ 

    //vec4 originalcolor = texture(u_screentexture, TexCoords);
    vec4 depthbuffer = texture(u_depthtexture, TexCoords);
    vec4 originalcolor = texture(u_screentexture, TexCoords);




    vec2 screencords = vec2(gl_FragCoord.x / screenwidth, gl_FragCoord.y / screenheight);
    
    screencords = screencords * 2.0 - 1.0;

    vec3 origin = u_pos;

    vec4 target = u_inverseproj * vec4(screencords, 1.0, 1.0);

    vec3 lookdirection = vec3(u_inverseview * vec4(normalize(target.xyz/target.w),0));

    lookdirection = normalize(lookdirection);
    float fragmentdepth = depthbuffer.x * length(lookdirection);
    
    
    const float epsilon = 0.0001;

    vec2 hitinfo = raysphere(origin, lookdirection, u_atmosphereradius);
    
    vec3 closeT = origin + lookdirection * (hitinfo.x);


    vec2 planethit = raysphere(origin, lookdirection, u_planetradius);



    float raylength = min(hitinfo.y, planethit.x);




    if(hitinfo.y > 0)
    {
        
        //vec3 color = CalculateLight(closeT, lookdirection, raylength);
        //vec3 color = calculate_scattering(u_pos, lookdirection, raylength, vec3(0.0), -dirtosun, vec3(sunintensity), u_planetcentre, u_planetradius, u_atmosphereradius, u_scatteringcoefficients, MIE_BETA, ABSORPTION_BETA, AMBIENT_BETA, G, HEIGHT_RAY, HEIGHT_MIE, HEIGHT_ABSORPTION, ABSORPTION_FALLOFF, 10, 10);
        
        //vec4 finalcolor = vec4(color, 1.0);

        vec4 finalcolor = originalcolor;


        FragColor = finalcolor;



        //FragColor = vec4(, 1.0);
        //FragColor = vec4(yes, 1);


        //FragColor = vec4(inscatter(origin, lookdirection, hitinfo.y),1);    
    }

    

    FragColor = vec4(1.0,0.0,0.0,1.0);
    

}