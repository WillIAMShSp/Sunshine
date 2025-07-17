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
    glfwSwapInterval(1);


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




#pragma region Shader declarations


    const char* solidsourcevs = "res/Shaders/TerrainGenerator/Terrainvs.shader";
    const char* solidsourcefs = "res/Shaders/Solidfs.shader";


    Shader solidsource(solidsourcevs, solidsourcefs);


    const char* basicfs = "res/Shaders/TerrainGenerator/Terrainfs.shader";


    Shader terrainsource(solidsourcevs, basicfs);


    terrainsource.Bind();
    terrainsource.SetUniform1i("amountoflights", 1);


    terrainsource.SetUniform3f("gLights[0].LightPosition", 5.f, 100.f, 5.f);
    terrainsource.SetUniform3f("gLights[0].LightColor", 1.f, 1.f, 1.f);
    terrainsource.SetUniform1f("gLights[0].AmbientIntensity", 0.2f);


    terrainsource.SetUniform3f("Mat.Color", 1, 1, 1);
    terrainsource.SetUniform3f("Mat.DiffuseColor", 1, 1, 1);
    terrainsource.SetUniform1f("Mat.DiffuseIntensity", 1);


#pragma endregion



    Renderer render;




#pragma region Basic shader initialization


    glm::mat4 model = glm::mat4(1.f);

    Camera camera(screenwidth, screenheight, glm::vec3(0.f, 0.f, 5.f));

    //Texture texture("res/Textures/PlaceHolderTexture.png");
    //texture.Bind();


#pragma endregion

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









    bt.SetSize(size);

    lattice.SetSize(size, size);



    perlin.ApplyPerlinNoiseToArray2D(lattice, frequency, amplitude, octaves, persistence);



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






        /* Render here */
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.0f, 0.0f, 0.0f, 1.f);
        render.Clear();


        terrainsource.SetUniform3f("gLights[0].LightPosition", lightx, lighty, lightz);

        camera.Input(window);



        camera.Matrix(60.f, 0.1f, 1000.f, terrainsource);



        //bt.Draw();
        planet.SetPlanetSettings(planetsettings);
        //     
        planet.UpdateMinMax();
        //

        planet.Draw();


        glDisable(GL_DEPTH_TEST);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("LightSettings");


        ImGui::SliderFloat("Light X", &lightx, -100, 100);
        ImGui::SliderFloat("Light Y", &lighty, -100, 100);
        ImGui::SliderFloat("Light Z", &lightz, -100, 100);

        ImGui::End();

        ImGui::Begin("Planet Settings");

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

        ImGui::End();

        ImGui::Begin("Controls");



        ImGui::Checkbox("Lock Mouse", &camera.lockmouse);
        ImGui::Checkbox("Wireframe Mode", &wireframe);

        ImGui::End();

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

