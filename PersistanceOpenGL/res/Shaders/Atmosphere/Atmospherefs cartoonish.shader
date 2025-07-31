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


#define RAY_BETA vec3(5.5e-6, 13.0e-6, 22.4e-6)

float u_atmosphereradius = 4;
float u_planetradius = 1;
float u_scaleheight = 2;
float samples = 10;

vec3 wavelengths = vec3(650, 570, 475);

vec3 dirtosun = vec3(0,1,0);
vec3 esun = vec3(40);



vec3 uSunPos = vec3(0,10,-1);


#define PI 3.141592
#define iSteps 16
#define jSteps 8


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

vec2 rayspheremaxdepth(vec3 origin, vec3 direction, float sphereradius, float maxdepth)
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
        farT = min(maxdepth,(-b + s)/(2*a));

        if(farT >= 0)
        {
            return vec2(closeT, farT);

        }

        

    }
    
    return vec2(1./0., 0);
}


vec3 atmosphere(vec3 r, vec3 r0, float raylength, vec3 pSun, float iSun, float rPlanet, float rAtmos, vec3 kRlh, float kMie, float shRlh, float shMie, float g) {
    // Normalize the sun and view directions.
    pSun = normalize(pSun);
    r = normalize(r);

    float iStepSize = raylength / float(iSteps); 

    // Initialize the primary ray time.
    float iTime = 0.0;

    // Initialize accumulators for Rayleigh and Mie scattering.
    vec3 totalRlh = vec3(0,0,0);
    vec3 totalMie = vec3(0,0,0);

    // Initialize optical depth accumulators for the primary ray.
    float iOdRlh = 0.0;
    float iOdMie = 0.0;

    // Calculate the Rayleigh and Mie phases.
    float mu = dot(r, pSun);
    float mumu = mu * mu;
    float gg = g * g;
    float pRlh = 3.0 / (16.0 * PI) * (1.0 + mumu);
    float pMie = 3.0 / (8.0 * PI) * ((1.0 - gg) * (mumu + 1.0)) / (pow(1.0 + gg - 2.0 * mu * g, 1.5) * (2.0 + gg));

    // Sample the primary ray.
    for (int i = 0; i < iSteps; i++) {

        // Calculate the primary ray sample position.
        vec3 iPos = r0 + r * (iTime + iStepSize * 0.5);

        // Calculate the height of the sample.
        float iHeight = length(iPos) - rPlanet;

        // Calculate the optical depth of the Rayleigh and Mie scattering for this step.
        float odStepRlh = exp(-iHeight / shRlh) * iStepSize;
        float odStepMie = exp(-iHeight / shMie) * iStepSize;

        // Accumulate optical depth.
        iOdRlh += odStepRlh;
        iOdMie += odStepMie;

        // Calculate the step size of the secondary ray.
        float sunraylength = raysphere(iPos, dirtosun, rAtmos).y;

        float jStepSize = sunraylength / float(jSteps);

        // Initialize the secondary ray time.
        float jTime = 0.0;

        // Initialize optical depth accumulators for the secondary ray.
        float jOdRlh = 0.0;
        float jOdMie = 0.0;

        // Sample the secondary ray.
        for (int j = 0; j < jSteps; j++) {

            // Calculate the secondary ray sample position.
            vec3 jPos = iPos + pSun * (jTime + jStepSize * 0.5);

            // Calculate the height of the sample.
            float jHeight = length(jPos) - rPlanet;
            

            // Increment the secondary ray time.
            jTime += jStepSize;
        }

        // Calculate attenuation.
        vec3 attn = exp(-(kMie * (iOdMie + jOdMie) + kRlh * (iOdRlh + jOdRlh)));

        // Accumulate scattering.
        totalRlh += odStepRlh * attn;
        totalMie += odStepMie * attn;

        // Increment the primary ray time.
        iTime += iStepSize;

    }

    // Calculate and return the final color.
    return iSun * (pRlh * kRlh * totalRlh + pMie * kMie * totalMie);
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
        
        
        float alpha = clamp(raylength * thickness, 0, 1);

        vec4 atmospherecolor = vec4(atmosphere(lookdirection, origin, raylength, vec3(0, 10, 0), esun.x, u_planetradius, u_atmosphereradius, vec3(1, 1, 1), 0, u_scaleheight, 0, 0.88),1.0);


        FragColor = mix(originalcolor, atmospherecolor, alpha);


       
    

    }

    else
    {
            
        FragColor = originalcolor;    
        
       
    }







    





    






}