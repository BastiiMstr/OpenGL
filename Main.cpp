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
#include "Shader/Shader.h"
#include "textures/Textures.h"



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


    float positions[] = {
        -0.9f , -0.1f, 0.0f, 0.0f, //dolny lewy
        -0.74f , -0.1f, 1.0f, 0.0f, //dolny prawy
        -0.74f , 0.1f, 1.0f, 1.0f,
        -0.9f , 0.1f, 0.0f, 1.0f
    };

    unsigned int indexes[] = {
        0 , 1 , 2,
        2 , 3 , 0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    //Vertex Array and buffer initialization
    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    //Index Buffer
    IndexBuffer ib(indexes, 6);

    glm::mat4 proj = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f);

    //teraz -8 to jest nasze max left
    //-4.5 nasze bottom

    //Shader
    Shader shader("Shader.shader");
    shader.Bind(); //teoretycznie mozna wyrzucic bo nie zmieniam nic w aktualnym ustawieniu shadera
    shader.SetUniformMat4f("u_MVP", proj);


    Renderer renderer;

    Texture texture("textures/PNG/plane.png");
    texture.Bind();

    shader.SetUniform1i("u_Texture", 0);

    //Unbinding VertexArray, VertexBuffers, indexBuffer and Shader
    va.Unbind();
    vb.Unbind();
    shader.Unbind();
    ib.Unbind();



    float r = 0.0f;
    float translate = 0.05f;

    //Update 
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();


        shader.Bind();

        renderer.Draw(va, ib, shader);

        if (r > 1.0f)
            translate = -0.05f;
        else if (r < 0.0f)
            translate = 0.05f;

        r += translate;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}
    glfwTerminate();
    return 0;
}