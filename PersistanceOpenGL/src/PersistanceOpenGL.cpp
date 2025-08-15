#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include <iostream>
#include <string>
#include <vector>

//vendor dependencies
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



//written classes
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "FrameBufferObject.h"
#include "Square.h"
#include "Renderer.h"
#include "Texture.h"
#include "ModelLoader.h"
#include "Vertex.h"
#include "MaterialLoader.h"
#include "Camera.h"
#include "FullScreenQuad.h"


#include "CubeMap.h"


#include "BaseTerrain.h"
#include "PerlinNoise.h"
#include "PerlinImplementation.h"
#include "BasePlanet.h"
#include "PlanetSettings.h"
#include "Atmosphere/AtmospherePostProc.h"
#include "../res/Assets/Heart/Heart.h"

#include "RenderableObject.h"









#define BREAK() __debugbreak();




void GLAPIENTRY MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    std::cout << "[OPENGL ERROR] ( " << type << " )" << message << std::endl;
    BREAK();
}




//const int screenwidth = 1000;
//const int screenheight = 1000;


const int screenwidth = 1920;
const int screenheight = 1080;


int main(void)
{





#pragma region GLFW initialization


    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;



    glfwWindowHint(GLFW_CONTEXT_DEBUG, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenwidth, screenheight, "Game", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        std::cout << "error\n";
    glfwSwapInterval(0);


    // Here's where we can code ourselves!

    std::cout << glGetString(GL_VERSION) << std::endl;




    glViewport(0, 0, screenwidth, screenheight);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CW);
    //glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

#pragma endregion


    float deltatime = 0;
    float currenttime = 0;
    float lasttime = 0;








#pragma region Shader declarations
    
    Shader atmosphereshader("res/Shaders/Screenvs.shader", "res/Shaders/Atmosphere/Atmospherefs.shader");

    Shader depthshader("res/Shaders/BasicDepthvs.shader", "res/Shaders/BasicDepthfs.shader");

    Shader screenshader("res/Shaders/Screenvs.shader","res/Shaders/Screenfs.shader");

    Shader testshader("res/Shaders/Screenvs.shader", "res/Shaders/testfs.shader");
    


    const char* solidsourcevs = "res/Shaders/TerrainGenerator/Terrainvs.shader";
    const char* solidsourcefs = "res/Shaders/Solidfs.shader";


    Shader solidsource(solidsourcevs, solidsourcefs);


    const char* basicfs = "res/Shaders/TerrainGenerator/Terrainfs.shader";


    Shader terrainsource(solidsourcevs, basicfs);

    Shader basicsource("res/Shaders/Basicvs.shader", "res/Shaders/Basicfs.shader");

    Shader basicinstancingsource("res/Shaders/BasicInstancevs.shader", "res/Shaders/Basicfs.shader");
    
    Shader pictureshader("res/Shaders/Picture/Picturevs.shader", "res/Shaders/Picture/Picturefs.shader");


    //terrainsource
  
    terrainsource.SetUniform1i("amountoflights", 1);


    terrainsource.SetUniform3f("gLights[0].LightPosition", 5.f, 100.f, 5.f);
    terrainsource.SetUniform3f("gLights[0].LightColor", 1.f, 1.f, 1.f);
    terrainsource.SetUniform1f("gLights[0].AmbientIntensity", 0.2f);


    terrainsource.SetUniform3f("Mat.Color", 1, 1, 1);
    terrainsource.SetUniform3f("Mat.DiffuseColor", 1, 1, 1);
    terrainsource.SetUniform1f("Mat.DiffuseIntensity", 1);



    //basicsource

    basicsource.SetUniform1i("amountoflights", 1);


    basicsource.SetUniform3f("gLights[0].LightPosition", 5.f, 100.f, 5.f);
    basicsource.SetUniform3f("gLights[0].LightColor", 1.f, 1.f, 1.f);
    basicsource.SetUniform1f("gLights[0].AmbientIntensity", 0.2f);


    basicsource.SetUniform3f("Mat.Color", 1, 1, 1);
    basicsource.SetUniform3f("Mat.DiffuseColor", 1, 1, 1);
    basicsource.SetUniform1f("Mat.DiffuseIntensity", 1);

    //basicinstancingsource

    basicinstancingsource.SetUniform1i("amountoflights", 1);


    basicinstancingsource.SetUniform3f("gLights[0].LightPosition", 5.f, 100.f, 5.f);
    basicinstancingsource.SetUniform3f("gLights[0].LightColor", 1.f, 1.f, 1.f);
    basicinstancingsource.SetUniform1f("gLights[0].AmbientIntensity", 0.2f);


    basicinstancingsource.SetUniform3f("Mat.Color", 1, 1, 1);
    basicinstancingsource.SetUniform3f("Mat.DiffuseColor", 1, 1, 1);
    basicinstancingsource.SetUniform1f("Mat.DiffuseIntensity", 1);

    



#pragma endregion



    Renderer render;


    RenderableObject heart;

    ModelLoader heartmodel("res/Models/Heart.obj", "res/Materials/Heart.mtl", basicsource);
    ModelLoader heartinstancedmodel("res/Models/Heart.obj", "res/Materials/Heart.mtl", basicinstancingsource);
    
    heart.SetModel(heartinstancedmodel);
    heart.SetRenderer(render);
    heart.SetShader(basicinstancingsource);

  
    
    


#pragma region Basic shader initialization


    glm::mat4 model = glm::mat4(1.f);

    Camera camera(screenwidth, screenheight, glm::vec3(0.f, 0.f, 5.f));

    //Texture texture("res/Textures/PlaceHolderTexture.png");
    //texture.Bind();


#pragma endregion





    FrameBufferObject dfbo(screenwidth, screenheight);

    dfbo.SetDepthMap();


  
    


    FullScreenQuad quad;
    
    

    











#pragma region IMGUI initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

#pragma endregion

    BaseTerrain bt;
    Array2D lattice;


    unsigned int size = 100;

    float frequency = 0.05f;

    int octaves = 24;

    float persistence = 0.3;

    float amplitude = 50.f;

    PerlinNoise perlin;

    glm::vec3 btposition(0,0,0); //this is the position of the topleft point of the lattice.







    bt.SetSize(size);

    lattice.SetSize(size, size);



    perlin.ApplyPerlinNoiseToArray2D(lattice, frequency, amplitude, octaves, persistence);


    bt.SetShader(terrainsource);

    bt.SetHeightMap(lattice);

    bt.UpdateHeight();

    bt.UpdateNormals();

    bt.InitMesh();

    



    bool wireframe = false;


    float lightx = 0;
    float lighty = 0;
    float lightz = 0;




    BasePlanet planet(50, terrainsource);
    planet.SetFaces();



    PlanetSettings planetsettings;










    bool viewplanet = true;

   




    FrameBufferObject frmbffr(screenwidth, screenheight);

    frmbffr.SetTexture2D();
    frmbffr.SetRenderBuffer();










    bool isdepth = false;




    atmosphereshader.SetUniform1i("u_screentexture", 0);
    screenshader.SetUniform1i("u_screentexture", 0);
    atmosphereshader.SetUniform1i("u_depthtexture", 1);

    pictureshader.SetUniform1i("u_pictex", 0);





    float atmosphererad = 4;
    float planetrad = 1;

    glm::vec3 wavelengths(700, 530, 440);

    float scttrstrngth = 10;
    float scaleheight = 0.25;
    float sunscaleheight = 0.25;
    float esun = 20;
    float timeofday = 0;
    glm::vec3 dirtosun = glm::vec3(0,1,0);






    

    
    

    int amountofhearts = 10;




    std::vector<glm::mat4> matrices;
    matrices.resize(amountofhearts*amountofhearts);
    
    int offset = 3;
    


  /*  for (int i = 0; i < amountofhearts; i++)
    {
        glm::mat4 modelmatrix = glm::mat4(1.0);
        

        modelmatrix = glm::translate(modelmatrix, glm::vec3(i * 0.5, i * 0.1, 0));

        matrices[i] = modelmatrix;

    }*/


    glm::vec3 positions[100];

    int f = 0;
    for (int y = 0; y < amountofhearts; y++)
    {

        for (int i = 0; i < amountofhearts; i++)
        {

            positions[f] = glm::vec3(i, 0, y);






            f++;
        }

        


    }


    for (int i = 0; i < matrices.size(); i++)
    {
        glm::mat4 modelmatrix = glm::mat4(1.0);

        modelmatrix = glm::translate(modelmatrix, positions[i]);

        //std::cout << "posX= " << positions[i].x<<" posZ= "<<positions[i].z<<"\n";



        matrices[i] = modelmatrix;


    }


    

    VertexBuffer translations;

    translations.AddBuffer(matrices);


    VertexArrayLayout instancinglayout;



    instancinglayout.push<float>(4);
    instancinglayout.push<float>(4);
    instancinglayout.push<float>(4);
    instancinglayout.push<float>(4);



    VertexArray& heartvao = heartinstancedmodel.GetVAO();


    translations.Bind();

    //heartinstancedmodel.SetInstancing(translations, instancinglayout);
    //heartinstancedmodel.SetInstancingMat4(translations);
    heart.SetInstancingParameters(amountofhearts * amountofhearts, translations, instancinglayout);

    /*heartvao.Bind();

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);


    heartvao.UnBind();*/



    FullScreenQuad picquad;

    


    Texture pictex("res/Textures/Isa.png");








    glm::mat4 picmodel = glm::mat4(1.0);


    glm::vec3 picpos(0, 0, 0);

    float& picx = picpos.x;
    float& picy = picpos.y;
    float& picz = picpos.z;


    
















 



























    bool CHARGE = false;
    float Zposoffset = 0;
    
    float counter = 0;
    float fps = 0;
    float totalsumoffps = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {


        //updated every frame!

        if (wireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        }

        currenttime = static_cast<float>(glfwGetTime());
        deltatime = currenttime - lasttime; 
        lasttime = currenttime;
        
        

        
        fps = 1 / deltatime;

        std::string sfps = std::to_string((fps));
        std::string newtitle = "Game - FPS: " + sfps;
        glfwSetWindowTitle(window, newtitle.c_str());
        





        /* Render here */
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.f, 0.f, 0.f, 0.f);
        render.Clear();


        //terrainsource.SetUniform3f("gLights[0].LightPosition", lightx, lighty, lightz);
        basicsource.SetUniform3f("gLights[0].LightPosition", lightx, lighty, lightz);
        basicinstancingsource.SetUniform3f("gLights[0].LightPosition", lightx, lighty, lightz);

        camera.Input(window, deltatime);



        camera.Matrix(60.f, 0.1f, 1000.f, terrainsource);
        camera.Matrix(60.f, 0.1f, 1000.f, basicsource);
        camera.Matrix(60.f, 0.1f, 1000.f, basicinstancingsource);
        camera.Matrix(60.f, 0.1f, 1000.f, pictureshader);





      
        planet.SetPlanetSettings(planetsettings);
           
        planet.UpdateMinMax();
      


        

        //if (CHARGE)
        //{
        //    Zposoffset += 1 * deltatime;

        //}


        //int f = 0;
        //for (int y = 0; y < amountofhearts; y++)
        //{

        //    for (int i = 0; i < amountofhearts; i++)
        //    {

        //        positions[f] = glm::vec3(i, 0, y+Zposoffset);






        //        f++;
        //    }




        //}


        //for (int i = 0; i < matrices.size(); i++)
        //{
        //    
        //    //std::cout << "posX= " << positions[i].x<<" posZ= "<<positions[i].z<<"\n";



        //    glm::translate(matrices[i], positions[i]);


        //}

        //translations.AddBuffer(matrices);






       





        frmbffr.Bind();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        render.Clear();
        
        
        //bt.SetShader(terrainsource);


        basicinstancingsource.Bind();
        
        if (viewplanet)
        {
           //planet.Draw();
            //bt.Draw();

            //heart.Draw();
            
            //heartinstancedmodel.DrawInstanced(render, amountofhearts * amountofhearts);
            heart.DrawInstanced(amountofhearts * amountofhearts);

            
            picmodel = glm::mat4(1.0);
            picmodel = glm::translate(picmodel, picpos);
            pictureshader.SetUniformMat4fv("u_Model", picmodel);
            pictex.Bind(0);
            render.Draw(picquad.vao, 6, pictureshader);


        }


        frmbffr.UnBind();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        render.Clear();


        //here goes the depth shader;




        
        //depth buffer
        

        //camera.Matrix(60.f, 0.1f, 1000.f, depthshader);
        
        //depthshader.Bind();


        /*dfbo.Bind();
        glClearColor(0.f,0.1f,0.0f,1.0f);
        render.Clear();

        bt.SetShader(depthshader);
        

        if (viewplanet)
        {
            planet.DrawDepth(depthshader);
            bt.Draw();
        }
        dfbo.UnBind();*/


        glDisable(GL_DEPTH_TEST);


        // atmosphere shader uniforms
      /*  atmosphereshader.SetUniform3f("u_pos", camera.GetPosition());
        atmosphereshader.SetUniformMat4fv("u_inverseview", camera.GetInverseViewMatrix());
        atmosphereshader.SetUniformMat4fv("u_inverseproj", camera.GetInverseProjectionMatrix());

        float scatterR = glm::pow(400 / wavelengths.x, 4) * scttrstrngth;
        float scatterG = glm::pow(400 / wavelengths.y, 4) * scttrstrngth;
        float scatterB = glm::pow(400 / wavelengths.z, 4) * scttrstrngth;

        dirtosun.x = glm::cos(timeofday);
        dirtosun.y = glm::sin(timeofday);



        glm::vec3 scatteringcoff(scatterR, scatterG, scatterB);

        float minplanetpos = planet.GetMinValue();

        atmosphereshader.SetUniform3f("u_wavelengths", scatteringcoff);
        atmosphereshader.SetUniform1f("u_atmosphereradius", atmosphererad);
        atmosphereshader.SetUniform1f("u_planetradius", minplanetpos);
        atmosphereshader.SetUniform1f("u_scaleheight", scaleheight);
        atmosphereshader.SetUniform1f("u_sunscaleheight", sunscaleheight);
        atmosphereshader.SetUniform1f("esun", esun);
        atmosphereshader.SetUniform3f("u_dirtosun", dirtosun);*/

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        render.Clear();

        frmbffr.GetTex().Bind(0);
        //dfbo.GetDepthTex().Bind(1);

        //atmosphereshader.Bind();
        screenshader.Bind();
        

        quad.Draw();










       

        
        /*atmosphere.SetCameraUniforms(camera);
        
        atmosphere.SetScreenTexture(frmbffr.GetTex());
        
        atmosphere.SetDepthBuffer(dfbo.GetDepthTex());
        
        atmosphere.Draw();


       */



        



       






        
        
       

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("LightSettings");


        ImGui::SliderFloat("Light X", &lightx, -100, 100);
        ImGui::SliderFloat("Light Y", &lighty, -100, 100);
        ImGui::SliderFloat("Light Z", &lightz, -100, 100);

        ImGui::End();


        //Imgui planetsettings
       /* ImGui::Begin("Planet Settings");

        if (ImGui::Button("Generate"))
        {
            planet.m_resetminmax = true;
            planet.UpdateFaces();
            planet.SmoothenNormals();
        }
        ;
        ImGui::SliderFloat("Planet Radius", &planetsettings.radius, 0.f, 100.f);
        ImGui::SliderFloat("Planet Mass", &planetsettings.mass, 0.f, 10000.f);
        ImGui::SliderFloat("Planet MinValue", &planetsettings.minvalue, 0.f, planetsettings.radius);
        ImGui::SliderInt("Amount of Layers", &planetsettings.numoflayers, 0, 8);

        ImGui::SliderFloat("Noise Centre X", &planetsettings.noisecentre.x, -10.f, 10.f);
        ImGui::SliderFloat("Noise Centre Y", &planetsettings.noisecentre.y, -10.f, 10.f);
        ImGui::SliderFloat("Noise Centre Z", &planetsettings.noisecentre.z, -10.f, 10.f);

        ImGui::Checkbox("Use the first layer as a mask?", &planetsettings.usefirstlayerasmask);

        if (planetsettings.numoflayers > 0)
        {
            for (int i = 0; i < planetsettings.numoflayers; i++)
            {
                std::string layer = "Layer " + std::to_string(i);

                ImGui::Text(layer.c_str());
                ImGui::SliderFloat((layer + (std::string)" Roughness").c_str(), &planetsettings.perlinlayers[i].roughness, 0.f, 10.f);
                ImGui::SliderFloat((layer + (std::string)" Strength").c_str(), &planetsettings.perlinlayers[i].strength, 0.f, 10.f);
                ImGui::SliderFloat((layer + (std::string)" Persistance").c_str(), &planetsettings.perlinlayers[i].persistance, 0.f, 10.f);
                ImGui::SliderInt((layer + (std::string)" Amount of Octaves").c_str(), &planetsettings.perlinlayers[i].octaves, 0, 24);

            }
        }

        ImGui::End();*/

        ImGui::Begin("Controls");



        ImGui::Checkbox("Lock Mouse", &camera.lockmouse);
        ImGui::Checkbox("Wireframe Mode", &wireframe);
        ImGui::Checkbox("isdepth?", &isdepth);

        ImGui::End();

        ImGui::Begin("CHARGE!!!");

        ImGui::SliderFloat("Light X", &picx, -120, 120);
        ImGui::SliderFloat("Light Y", &picy, -120, 120);
        ImGui::SliderFloat("Light Z", &picz, -120, 120);

        ImGui::Checkbox("CHARGE?!?!", &CHARGE);


        

        ImGui::End();


        //imgui atmosphere settings
       /* ImGui::Begin("atmosphere");

        ImGui::SliderFloat("Red Wavelength", &wavelengths.x, 0, 700);
        ImGui::SliderFloat("Green Wavelength", &wavelengths.y, 0, 700);
        ImGui::SliderFloat("Blue Wavelength", &wavelengths.z, 0, 700);
        ImGui::SliderFloat("Scattering Strength", &scttrstrngth, 0, 10);
        ImGui::SliderFloat("Sun Intensity", &esun, 0, 70);

        ImGui::SliderFloat("Density Scale Height", &scaleheight, 0, 1);
        ImGui::SliderFloat("Sun Density Scale Height", &sunscaleheight, 0, 1);
        ImGui::SliderFloat("Atmosphere Radius", &atmosphererad,0, 1000);
        ImGui::SliderFloat("Planet Radius", &planetrad, 0, 1000);
        ImGui::SliderFloat("Time of Day", &timeofday, 0, 6.3);

        ImGui::End();*/


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());








        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }



    glfwTerminate();
    return 0;

}

