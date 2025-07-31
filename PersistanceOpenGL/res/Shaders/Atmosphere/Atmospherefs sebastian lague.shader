#version 330 core

#define RAY_BETA vec3(5.5e-6, 13.0e-6, 22.4e-6)

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





uniform int u_isdepth = 1;





uniform float u_atmosphereradius = 2;
uniform float u_planetradius = 1;
uniform float u_densityfalloff = 4;
 float u_scaleheight;
 float samples = 10;

uniform vec3 u_wavelengths = vec3(650, 570, 475);

 vec3 dirtosun = vec3(0,1,0);

 float esun = 1;


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













 float densityatpoint(vec3 densitySamplePoint) 
 {
				float heightAboveSurface = length(densitySamplePoint) - u_planetradius;
				float height01 = heightAboveSurface / (u_atmosphereradius - u_planetradius);
				float localDensity = exp(-height01 * u_densityfalloff) * (1 - height01);
				return localDensity;
 }


 float opticaldepth(vec3 origin, vec3 direction, float raylength)
 {
 
     float opticaldepth = 0;

     vec3 samplepoint = origin;

     float stepsize = raylength/samples;

     for(int i = 0; i < samples; i++)
     {
            
         float density = densityatpoint(samplepoint);

         opticaldepth+= density * stepsize;


         samplepoint += direction * stepsize;
     
     }


     return opticaldepth;

 }




 vec3 inscatter(vec3 origin, vec3 direction, float raylength, vec3 originalcolor)
 {
     vec3 light;
     vec3 samplepoint = origin;
     float stepsize = raylength/samples;

     float camopticaldepth;

     for(int i = 0; i < samples; i++)
     {
     
         float density = densityatpoint(samplepoint);



         float solarlength = raysphere(samplepoint, dirtosun, u_atmosphereradius).y;


         float sunopticaldepth = opticaldepth(samplepoint, dirtosun, solarlength);

         camopticaldepth = opticaldepth(samplepoint, -direction, stepsize * i);
 
         vec3 transmittance = exp(-(camopticaldepth + sunopticaldepth) * u_wavelengths);

        

         
         
         light += density * transmittance * u_wavelengths * stepsize;


         
         samplepoint += direction * stepsize;

     }
     
     light*= u_wavelengths * esun * stepsize/ u_planetradius;


     return originalcolor  + light;

     
 }





























void main()
{ 
	
    vec2 screencords = vec2(gl_FragCoord.x / screenwidth, gl_FragCoord.y / screenheight);
    
    screencords = screencords * 2.0 - 1.0;

    vec3 origin = u_pos;

    vec4 target = u_inverseproj * vec4(screencords, 1.0, 1.0);

    vec3 lookdirection = vec3(u_inverseview * vec4(normalize(target.xyz/target.w),0));

    vec4 originalcolor = texture(u_screentexture,TexCoords);

    lookdirection = normalize(lookdirection);

    vec2 hitinfo = raysphere(origin, lookdirection, u_atmosphereradius);


    vec2 planethit = raysphere(origin, lookdirection, u_planetradius);


    float epsilon = .00001;


    
    
    float raylength = min(planethit.x, hitinfo.y);


    if(hitinfo.y > 0)
    {
        vec3 closeT = origin + lookdirection * (hitinfo.x + epsilon); 
        
        vec3 light = inscatter(closeT, lookdirection, raylength - epsilon * 2, originalcolor.xyz);

        

        
        FragColor = vec4(light, 1);
        

    
         


    }

    else
    {
            
        FragColor = originalcolor;    
        
       
    }

    






}