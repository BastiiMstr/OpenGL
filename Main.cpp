#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include "src/Renderer.h"
#include "src/VertexBuffer.h"
#include "src/VertexBufferLayout.h"
#include "src/IndexBuffer.h"
#include "src/VertexArray.h"
#include "src/Figures.h"

#include "Shader/Shader.h"
#include "textures/Textures.h"

#include "outside/imGui/imgui.h"
#include "outside/imGui/imgui_impl_glfw_gl3.h"

void ChangeQuadPos(Vertex* quad, glm::vec2 position ,float size, float rotation)
{
    auto q1 = CreateSquare(position.x, position.y, size, rotation);
    memcpy(quad, q1.data(), q1.size() * sizeof(Vertex));
}

bool ColisionDetection(glm::vec3 objA, glm::vec3 objB, float objA_size, float objB_size)
{
    bool collisionX = objA.x + objA_size >= objB.x && objB.x + objB_size >= objA.x;
    bool collisionY = objA.y + objA_size >= objB.y && objB.y + objB_size >= objA.y;

    return collisionX && collisionY;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    {
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "mamy problem klej nie dziala" << std::endl;

    
    unsigned int Quad[] = {
        0 , 1 , 2,
        2 , 3 , 0,
    };
    

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
    
    #pragma region "plane init"
        glm::vec3 planev3 = { -4.0f, 0.0f, 0.0f };
        glm::vec2 planeSize = { 1.5f, 1.0f };
        VertexArray va;
        VertexBuffer vb(nullptr, sizeof(Vertex) * 4);
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(Quad, sizeof(Quad));
    
    #pragma endregion

    #pragma region "bird A init"

        VertexArray birdAvA;
        VertexBuffer birdAvB(nullptr, sizeof(Vertex) * 4);
        VertexBufferLayout birdAvBL;
        birdAvBL.Push<float>(2);
        birdAvBL.Push<float>(2);
        birdAvA.AddBuffer(birdAvB, birdAvBL);

        IndexBuffer birdAiB(Quad, sizeof(Quad));

    #pragma endregion

    #pragma region "Bird B init"
        VertexArray birdBvA;
        VertexBuffer birdBvB(nullptr, sizeof(Vertex) * 4);
        VertexBufferLayout birdBvBL;
        birdBvBL.Push<float>(2);
        birdBvBL.Push<float>(2);
        birdBvA.AddBuffer(birdBvB, birdBvBL);

        IndexBuffer birdBiB(Quad, sizeof(Quad));
    #pragma endregion

    #pragma region "Bird C init"
        VertexArray birdCvA;
        VertexBuffer birdCvB(nullptr, sizeof(Vertex) * 4);
        VertexBufferLayout birdCvBL;
        birdCvBL.Push<float>(2);
        birdCvBL.Push<float>(2);
        birdCvA.AddBuffer(birdCvB, birdCvBL);

        IndexBuffer birdCiB(Quad, sizeof(Quad));
    #pragma endregion

    #pragma region "Bird D init"
        VertexArray birdDvA;
        VertexBuffer birdDvB(nullptr, sizeof(Vertex) * 4);
        VertexBufferLayout birdDvBL;
        birdDvBL.Push<float>(2);
        birdDvBL.Push<float>(2);
        birdDvA.AddBuffer(birdDvB, birdDvBL);

        IndexBuffer birdDiB(Quad, sizeof(Quad));
    #pragma endregion

    #pragma region "Bg init"

        Vertex bgVt[4];
        glm::vec3 bg_v3(-8.0f, -4.5f, 0.0f);
        glm::vec2 bg_Size(16.0f, 9.0f); 
        auto bg = CreateRectangle(bg_v3, bg_Size, 0.0f);
        memcpy(bgVt, bg.data(), bg.size() * sizeof(Vertex));

        VertexArray bgVa;
        VertexBuffer bgVb(bgVt, sizeof(Vertex) * 4);
        VertexBufferLayout bgVbL;
        bgVbL.Push<float>(2);
        bgVbL.Push<float>(2);
        bgVa.AddBuffer(bgVb, bgVbL);

        IndexBuffer bgIb(Quad, sizeof(Quad));
    #pragma endregion

    glm::mat4 proj = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));


    //teraz -8 to jest nasze max left
    //-7 lub -4.5 nasze bottom nie wiem 

    //Shader
    Shader shader("Shader.shader");
    shader.Bind();



    Renderer renderer;

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    #pragma region "Tekstury"
    Texture Planetex("textures/PNG/plane1.png");
    {
        Planetex.AddNewTexture("textures/PNG/plane2.png");
        Planetex.AddNewTexture("textures/PNG/plane3.png");
        Planetex.AddNewTexture("textures/PNG/plane4.png");
        Planetex.AddNewTexture("textures/PNG/plane5.png");
        Planetex.AddNewTexture("textures/PNG/plane6.png");
        Planetex.AddNewTexture("textures/PNG/plane7.png");
        Planetex.AddNewTexture("textures/PNG/plane8.png");
    }

    Texture birdAtex("textures/PNG/bird1-2.png");
    {
        birdAtex.AddNewTexture("textures/PNG/bird1-3.png");
        birdAtex.AddNewTexture("textures/PNG/bird1-4.png");
        birdAtex.AddNewTexture("textures/PNG/bird1-5.png");
    }
    Texture birdBtex("textures/PNG/bird2-2.png");
    {
        birdBtex.AddNewTexture("textures/PNG/bird2-3.png");
        birdBtex.AddNewTexture("textures/PNG/bird2-4.png");
        birdBtex.AddNewTexture("textures/PNG/bird2-5.png");
    }
    Texture birdDD("textures/PNG/bird4-2.png");

    Texture birdCtex("textures/PNG/bird3-2.png");
    {
        birdCtex.AddNewTexture("textures/PNG/bird3-3.png");
        birdCtex.AddNewTexture("textures/PNG/bird3-4.png");
        birdCtex.AddNewTexture("textures/PNG/bird3-5.png");
    }
    

    Texture birdDtex("textures/PNG/bird4-2.png");
    {
        birdDtex.AddNewTexture("textures/PNG/bird4-3.png");
        birdDtex.AddNewTexture("textures/PNG/bird4-4.png");
        birdDtex.AddNewTexture("textures/PNG/bird4-5.png");
    }


    Texture background("textures/PNG/bg.png");

    background.Bind(0);
    Planetex.Bind(1);
    birdAtex.Bind(2);   //A
    birdBtex.Bind(3);   //B
    birdCtex.Bind(4);   //C
    birdDtex.Bind(5);   //D
    birdDD.Bind(7);

    #pragma endregion


    //Unbinding VertexArray, VertexBuffers, indexBuffer and Shader
    va.Unbind();
    vb.Unbind();
    shader.Unbind();
    ib.Unbind();
    

    float birdSpeed = 0.07f; 
    glm::vec3 birds[4];
    birds[0] = { 10.0f , -7.0f , 0 };
    birds[1] = { 12.0f ,  1.5f , 0 };
    birds[2] = {  9.0f , -1.5f , 0 };
    birds[3] = { 14.0f , -3.0f , 0 };

    int birdIndex = 2;
    int birdChangeTimer = 0;


    int j = 0;
    //glm::vec3 BirdTransform(birdpos_x, birdpos_y, 0);

    //Update 
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        ImGui_ImplGlfwGL3_NewFrame();

        shader.Bind();

    #pragma region "zas poz obiektow"
        {
            Vertex bird[4];
            ChangeQuadPos(bird, birds[0], 1.0f, 0.0f);

            birdAvB.Bind();
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(bird), bird);
        }
        //ptak A
        {
            Vertex bird[4];
            ChangeQuadPos(bird, birds[1], 1.0f, 0.0f);

            birdBvB.Bind();
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(bird), bird);
        }
        //Ptak B
        {
            Vertex bird[4];
            ChangeQuadPos(bird, birds[2], 1.0f, 0.0f);

            birdCvB.Bind();
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(bird), bird);
        }
        //Ptak C
        {
            Vertex bird[4];
            ChangeQuadPos(bird, birds[3], 1.0f, 0.0f);

            birdDvB.Bind();
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(bird), bird);
        }
        //Ptak D
        {
            Vertex plane[4];
            auto q1 = CreateRectangle(planev3, planeSize, 0.0f);
            memcpy(plane, q1.data(), q1.size() * sizeof(Vertex));

            vb.Bind();
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(plane), plane);
        }
    #pragma endregion


    #pragma region "rysowanie obiektow"
        {
            shader.SetUniform1i("u_Texture", 0);
            glm::mat4 model = glm::translate(glm::mat4(1.0f), bg_v3);
            glm::mat4 mvp = proj * view;// *model;
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(bgVa, bgIb, shader);
        }
        //background
        {
            shader.SetUniform1i("u_Texture", 1);
            glm::mat4 model = glm::translate(glm::mat4(1.0f), planev3);
            glm::mat4 mvp = proj * view;// *model;
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        } 
        //Samolot
        { 
            shader.SetUniform1i("u_Texture", 2);
            //glm::mat4 model = glm::translate(glm::mat4(1.0f), birds[0]);
            glm::mat4 mvp = proj * view;// *model;
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(birdAvA, birdAiB, shader);
        }
        //Ptak A
        {
            shader.SetUniform1i("u_Texture", 3);
            //glm::mat4 model = glm::translate(glm::mat4(1.0f), birds[1]);
            glm::mat4 mvp = proj * view;// *model;
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(birdBvA, birdBiB, shader);
        } 
        //Ptak B
        {
            shader.SetUniform1i("u_Texture", 4);
            //glm::mat4 model = glm::translate(glm::mat4(1.0f), birds[2]);
            glm::mat4 mvp = proj * view;// *model;
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(birdCvA, birdCiB, shader);
        } 
        //Ptak C
        {
            shader.SetUniform1i("u_Texture", 5);
            //glm::mat4 model = glm::translate(glm::mat4(1.0f), birds[3]);
            glm::mat4 mvp = proj * view;// *model;
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(birdDvA, birdDiB, shader);
        }
        //Ptak D

    #pragma endregion

    #pragma region "animate"
        if (birdChangeTimer < 10)
            birdChangeTimer++;
        else
        {
            birdChangeTimer = 0;
            Planetex.ChangeImage();
            birdAtex.ChangeImage();
            birdBtex.ChangeImage();
            birdCtex.ChangeImage();
            birdDtex.ChangeImage();
        }
    #pragma endregion


    #pragma region "Przesuwanie obiektów"
        for (int i = 0; i < 4; i++)
        {
            if (ColisionDetection(planev3,birds[i],1.0f,1.0f))
            {
                j++;
                std::cout<<"collision "<<j<<" bird "<<i<<std::endl;
            }

            if (birds[i].x > -8.0f)
            {
                birds[i].x -= birdSpeed;
            }
            else
            {
                birds[i].x = 10.0f;
                birds[i].y = rand() % 9 - 4.5f;
            }
        }

        if (planev3.y > -5.0f)
            planev3.y -= 0.05f;
        else
            std::cout << "destroyed";

    #pragma endregion

    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int left = glfwGetKey(window, GLFW_KEY_LEFT);

    if (space == GLFW_PRESS)
        planev3.y += 0.1f;
    if (right == GLFW_PRESS)
        planev3.x += 0.1f;
    if (left == GLFW_PRESS)
        planev3.x -= 0.1f;


        { 
            ImGui::SliderFloat3("TranslationA", &planev3.x, 0.0f, 8.0f);            // Editing PlaneTransform  
            //ImGui::SliderFloat3("TranslationB", &BirdTransform.x, -9.0f, 0.0f);            // Editing PlaneTransform  

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}