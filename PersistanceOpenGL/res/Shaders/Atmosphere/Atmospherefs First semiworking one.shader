#version 330 core
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




uniform float u_atmosphereradius = 15;
uniform float u_planetradius = 10;
uniform float u_densityfalloff = 4;
uniform float u_scaleheight = 0.25;
uniform float u_sunscaleheight = 1;
float samples = 10;

uniform vec3 u_wavelengths = vec3(650, 570, 475);

uniform vec3 u_dirtosun = vec3(0,1,0);

uniform float esun = 3;

 float near = 0.1; 
    float far  = 1000.0; 
  
    float LinearizeDepth(float depth) 
    {
        float z = depth * 2.0 - 1.0; // back to NDC 
        return (2.0 * near * far) / (far + near - z * (far - near));	
    }



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
				float localDensity = exp(-height01 / u_scaleheight);
				return localDensity;
}

float densityatpointsun(vec3 sundensitysamplepoint)
{
    float heightAboveSurface = length(sundensitysamplepoint) - u_planetradius;
				float height01 = heightAboveSurface / (u_atmosphereradius - u_planetradius);
				float localDensity = exp(-height01 / u_sunscaleheight);
				return localDensity;
}

    



float rayphase(vec3 lookdirection)
{

    vec3 dirtoorigin = -lookdirection;

    float costheta = dot(normalize(dirtoorigin), normalize(u_dirtosun));

    return 0.75 * (1.0 + costheta * costheta);
    

}


float sunopticaldepth = 0;


bool lightsampling(vec3 samplepoint)
{

    
    sunopticaldepth = 0;


    float sunraylength = raysphere(samplepoint, u_dirtosun, u_atmosphereradius).y;




    float stepsize = sunraylength / samples;

    vec3 sunpoint = samplepoint;

    


    for(int i = 0; i < samples; i++)
    {
        

        float height = length(sunpoint) - u_planetradius;


        if(height < 0)
        {


            return false;


        }

        float density = densityatpointsun(samplepoint);

        sunopticaldepth += density * stepsize;



        sunpoint += u_dirtosun * stepsize;

    }

    
    return true;




}





vec3 Light(vec3 origin, vec3 raydirection, float raylength)
{

    float stepsize = raylength / samples;


    vec3 samplepoint = origin;

    vec3 inscatter = vec3(0);

    float viewrayopticaldepth = 0;


    for(int i = 0; i < samples; i++)
    {
        
        float segmentopticaldepth = densityatpoint(samplepoint) * stepsize;
        
        viewrayopticaldepth += segmentopticaldepth;


        bool overground = lightsampling(samplepoint);


        if(overground)
        {


            vec3 transmittance = exp(- u_wavelengths * (viewrayopticaldepth + sunopticaldepth));

            inscatter += transmittance * segmentopticaldepth;


        }




    
        
        samplepoint += raydirection * stepsize;

    }



    vec3 light = esun * rayphase(raydirection) * u_wavelengths * inscatter;

    return light;
    

}










void main()
{ 

    vec2 screencords = vec2(gl_FragCoord.x / screenwidth, gl_FragCoord.y / screenheight);
    
    screencords = screencords * 2.0 - 1.0;

    vec3 origin = u_pos;

    vec4 target = u_inverseproj * vec4(screencords, 1.0, 1.0);

    vec3 lookdirection = vec3(u_inverseview * vec4(normalize(target.xyz/target.w),0));

    lookdirection = normalize(lookdirection);


    vec4 originalcolor = texture(u_screentexture,TexCoords);
    vec4 depthbuffer = texture(u_depthtexture, TexCoords);

    float depth = depthbuffer.x;

    depth = LinearizeDepth(depth);

   


    vec2 hitinfo = raysphere(origin, lookdirection, u_atmosphereradius);


    vec2 planethit = raysphere(origin, lookdirection, u_planetradius);

    
    




   float epsilon = .0001;


    
    
   float raylength = min(hitinfo.y - hitinfo.x, depth - hitinfo.x);






   float thickness = 0.3;


   if(hitinfo.y > 0)
   {
       
        

        vec3 closeT = origin + lookdirection * (hitinfo.x + epsilon); 


        vec3 light = Light(closeT, lookdirection, raylength);

        light = 1.0 - exp(-light);


        vec4 finalcolor = originalcolor + vec4(light, 1.0);
       
        //finalcolor = 1.0 - exp(-finalcolor);



        FragColor = finalcolor;

    }

    else
    {
            
        FragColor = originalcolor;    
        
       
    }


}