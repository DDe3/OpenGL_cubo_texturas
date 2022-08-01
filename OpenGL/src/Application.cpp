#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "SHADER.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
using std::string;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camara
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// iluminacion
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
};

int main2()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    //Shader ourShader("./shaders/texture_vertex.shader", "./shaders/texture_fragment.shader");
    Shader ourShader("./shaders/cube_vertex.shader", "./shaders/cube_fragment.shader");


    float faces[6][32] =
    {
        // Front
        {
            // positions      // texture coords
         0.5f,  0.5f, 0.5f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.5f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.5f,   0.0f, 1.0f  // top left 
        },

        // Back
        {
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f  // top left 
        },

        // Izquierda
        {
        -0.5f,  0.5f, 0.5f,   1.0f, 1.0f, // top right
        -0.5f, -0.5f, 0.5f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // top left 
        },
        // Derecha
        {
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom right
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom left
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f  // top left 
        },
        // Top
        {
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom right
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // top left 
        },
        // Bottom
        {
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // top right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom left
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f  // top left
        }
    };


    // ----------------------------------- FRONT FACE ----------------------------------------------------
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[0]), faces[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // ---------------------------------------------------------------------------------------------------

    // ----------------------------------- LEFT FACE -----------------------------------------------------
    unsigned int VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[1]), faces[1], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // --------------------------------------------------------------------------------------------------

    // ----------------------------------- RIGHT FACE -----------------------------------------------------
    unsigned int VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);

    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[2]), faces[2], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // --------------------------------------------------------------------------------------------------

    // ----------------------------------- LEFT FACE -----------------------------------------------------
    unsigned int VBO3, VAO3;
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);

    glBindVertexArray(VAO3);

    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[3]), faces[3], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // --------------------------------------------------------------------------------------------------

    // ----------------------------------- TOP FACE -----------------------------------------------------
    unsigned int VBO4, VAO4;
    glGenVertexArrays(1, &VAO4);
    glGenBuffers(1, &VBO4);

    glBindVertexArray(VAO4);

    glBindBuffer(GL_ARRAY_BUFFER, VBO4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[4]), faces[4], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // --------------------------------------------------------------------------------------------------

    // ----------------------------------- BOTTOM FACE -----------------------------------------------------
    unsigned int VBO5, VAO5;
    glGenVertexArrays(1, &VAO5);
    glGenBuffers(1, &VBO5);

    glBindVertexArray(VAO5);

    glBindBuffer(GL_ARRAY_BUFFER, VBO5);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[5]), faces[5], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // --------------------------------------------------------------------------------------------------

    unsigned int texture1, texture2, texture3;
    // ----------------------------------------------- TEXTURE 1 ------------------------------------------------------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("./textures/wood.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // -----------------------------------------------------------------------------------------------------------------

    // ----------------------------------------------- TEXTURE 2 ------------------------------------------------------

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("./textures/wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // -----------------------------------------------------------------------------------------------------------------

     // ----------------------------------------------- TEXTURE 3 ------------------------------------------------------

    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("./textures/papel.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // -----------------------------------------------------------------------------------------------------------------

    ourShader.use();


    while (!glfwWindowShouldClose(window))
    {

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // ------------------------------------- ROTACION ----------------------------------------------------------------------------
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        ourShader.setMat4("projection", projection);
        // ---------------------------------------------------------------------------------------------------------------------------

        // -------------------------------------------------------- DIBUJAR ----------------------------------------------------------


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindTexture(GL_TEXTURE_2D, texture3);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindTexture(GL_TEXTURE_2D, texture3);
        glBindVertexArray(VAO3);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO4);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO5);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteVertexArrays(1, &VAO3);
    glDeleteBuffers(1, &VBO3);
    glDeleteVertexArrays(1, &VAO4);
    glDeleteBuffers(1, &VBO4);
    glDeleteVertexArrays(1, &VAO5);
    glDeleteBuffers(1, &VBO5);


    glfwTerminate();
    return 0;


}


int main3() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    //Shader lightingShader("./shaders/iluminacion_basica_vs.shader", "./shaders/iluminacion_basica_fs.shader");
    Shader lightCubeShader("./shaders/luz_cubo_vs.shader", "./shaders/luz_cubo_fs.shader");
    Shader textureLightShader("./shaders/texture_light_vertex.shader","./shaders/texture_light_fragment.shader");


    float faces[6][32] =
    {
        // Front
        {
            // positions     // texture coords     //normals
         0.5f,  0.5f, 0.5f,   1.0f, 1.0f,        0.0f, 0.0f, 1.0f,// top right
         0.5f, -0.5f, 0.5f,   1.0f, 0.0f,        0.0f, 0.0f, 1.0f,// bottom right
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,        0.0f, 0.0f, 1.0f,// bottom left
        -0.5f,  0.5f, 0.5f,   0.0f, 1.0f,        0.0f, 0.0f, 1.0f// top left 
        },

        // Back
        {
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,       0.0f, 0.0f, -1.0f,// top right
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,       0.0f, 0.0f, -1.0f,// bottom right
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,       0.0f, 0.0f, -1.0f,// bottom left
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,       0.0f, 0.0f, -1.0f// top left 
        },

        // Izquierda
        {
        -0.5f,  0.5f, 0.5f,   1.0f, 1.0f,       -1.0f, 0.0f, 0.0f,// top right
        -0.5f, -0.5f, 0.5f,   1.0f, 0.0f,       -1.0f, 0.0f, 0.0f,// bottom right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,       -1.0f, 0.0f, 0.0f,// bottom left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,       -1.0f, 0.0f, 0.0f// top left 
        },
        // Derecha
        {
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,       1.0f, 0.0f, 0.0f,// top right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,       1.0f, 0.0f, 0.0f,// bottom right
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,       1.0f, 0.0f, 0.0f,// bottom left
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f,       1.0f, 0.0f, 0.0f  // top left 
        },
        // Top
        {
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,       0.0f, 1.0f, 0.0f,// top right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,       0.0f, 1.0f, 0.0f,// bottom right
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,       0.0f, 1.0f, 0.0f,// bottom left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,       0.0f, 1.0f, 0.0f// top left 
        },
        // Bottom
        {
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,       0.0f,-1.0f, 0.0f,// top right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,       0.0f,-1.0f, 0.0f,// bottom right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,       0.0f,-1.0f, 0.0f,// bottom left
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,       0.0f,-1.0f, 0.0f// top left
        }
    };


    // ----------------------------------- FRONT FACE ----------------------------------------------------
    unsigned int VBO0, VAO0;
    glGenVertexArrays(1, &VAO0);
    glGenBuffers(1, &VBO0);

    glBindVertexArray(VAO0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[0]), faces[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // ---------------------------------------------------------------------------------------------------

    // ----------------------------------- LEFT FACE -----------------------------------------------------
    unsigned int VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[1]), faces[1], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // --------------------------------------------------------------------------------------------------

    // ----------------------------------- RIGHT FACE -----------------------------------------------------
    unsigned int VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);

    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[2]), faces[2], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // --------------------------------------------------------------------------------------------------

    // ----------------------------------- LEFT FACE -----------------------------------------------------
    unsigned int VBO3, VAO3;
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);

    glBindVertexArray(VAO3);

    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[3]), faces[3], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // --------------------------------------------------------------------------------------------------

    // ----------------------------------- TOP FACE -----------------------------------------------------
    unsigned int VBO4, VAO4;
    glGenVertexArrays(1, &VAO4);
    glGenBuffers(1, &VBO4);

    glBindVertexArray(VAO4);

    glBindBuffer(GL_ARRAY_BUFFER, VBO4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[4]), faces[4], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // --------------------------------------------------------------------------------------------------

    // ----------------------------------- BOTTOM FACE -----------------------------------------------------
    unsigned int VBO5, VAO5;
    glGenVertexArrays(1, &VAO5);
    glGenBuffers(1, &VBO5);

    glBindVertexArray(VAO5);

    glBindBuffer(GL_ARRAY_BUFFER, VBO5);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[5]), faces[5], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // ----------------------------------------------------------------------------------------------------

    // 
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int VBO, lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(lightCubeVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    // --------------------------------------------------------------------------------------------------

    unsigned int texture1, texture2, texture3;
    // ----------------------------------------------- TEXTURE 1 ------------------------------------------------------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("./textures/wood.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // -----------------------------------------------------------------------------------------------------------------

    // ----------------------------------------------- TEXTURE 2 ------------------------------------------------------

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("./textures/wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // -----------------------------------------------------------------------------------------------------------------

     // ----------------------------------------------- TEXTURE 3 ------------------------------------------------------

    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("./textures/papel.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    textureLightShader.use();
    

    // -----------------------------------------------------------------------------------------------------------------


    while (!glfwWindowShouldClose(window))
    {
        
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        textureLightShader.use();
        textureLightShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        textureLightShader.setVec3("lightPos", lightPos);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        textureLightShader.setMat4("projection", projection);
        textureLightShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        textureLightShader.setMat4("model", model);

        // render 

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindTexture(GL_TEXTURE_2D, texture3);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindTexture(GL_TEXTURE_2D, texture3);
        glBindVertexArray(VAO3);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO4);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO5);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO0);
    glDeleteBuffers(1, &VBO0);
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteVertexArrays(1, &VAO3);
    glDeleteBuffers(1, &VBO3);
    glDeleteVertexArrays(1, &VAO4);
    glDeleteBuffers(1, &VBO4);
    glDeleteVertexArrays(1, &VAO5);
    glDeleteBuffers(1, &VBO5);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;

}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    Shader lightCubeShader("./shaders/luz_cubo_vs.shader", "./shaders/luz_cubo_fs.shader");
    Shader textureLightShader("./shaders/texture_light_vertex.shader", "./shaders/texture_light_fragment.shader");
    Shader screenShader("./shaders/framebuffer_screen_vertex.shader", "./shaders/framebuffer_screen_fragment.shader");

    float faces[6][32] =
    {
        // Front
        {
            // positions     // texture coords     //normals
         0.5f,  0.5f, 0.5f,   1.0f, 1.0f,        0.0f, 0.0f, 1.0f,// top right
         0.5f, -0.5f, 0.5f,   1.0f, 0.0f,        0.0f, 0.0f, 1.0f,// bottom right
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,        0.0f, 0.0f, 1.0f,// bottom left
        -0.5f,  0.5f, 0.5f,   0.0f, 1.0f,        0.0f, 0.0f, 1.0f// top left 
        },

        // Back
        {
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,       0.0f, 0.0f, -1.0f,// top right
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,       0.0f, 0.0f, -1.0f,// bottom right
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,       0.0f, 0.0f, -1.0f,// bottom left
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,       0.0f, 0.0f, -1.0f// top left 
        },

        // Izquierda
        {
        -0.5f,  0.5f, 0.5f,   1.0f, 1.0f,       -1.0f, 0.0f, 0.0f,// top right
        -0.5f, -0.5f, 0.5f,   1.0f, 0.0f,       -1.0f, 0.0f, 0.0f,// bottom right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,       -1.0f, 0.0f, 0.0f,// bottom left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,       -1.0f, 0.0f, 0.0f// top left 
        },
        // Derecha
        {
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,       1.0f, 0.0f, 0.0f,// top right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,       1.0f, 0.0f, 0.0f,// bottom right
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,       1.0f, 0.0f, 0.0f,// bottom left
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f,       1.0f, 0.0f, 0.0f  // top left 
        },
        // Top
        {
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,       0.0f, 1.0f, 0.0f,// top right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,       0.0f, 1.0f, 0.0f,// bottom right
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,       0.0f, 1.0f, 0.0f,// bottom left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,       0.0f, 1.0f, 0.0f// top left 
        },
        // Bottom
        {
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,       0.0f,-1.0f, 0.0f,// top right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,       0.0f,-1.0f, 0.0f,// bottom right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,       0.0f,-1.0f, 0.0f,// bottom left
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,       0.0f,-1.0f, 0.0f// top left
        }
    };


    // ----------------------------------- FRONT FACE ----------------------------------------------------
    unsigned int VBO0, VAO0;
    glGenVertexArrays(1, &VAO0);
    glGenBuffers(1, &VBO0);

    glBindVertexArray(VAO0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[0]), faces[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // ---------------------------------------------------------------------------------------------------

    // ----------------------------------- LEFT FACE -----------------------------------------------------
    unsigned int VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[1]), faces[1], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // --------------------------------------------------------------------------------------------------

    // ----------------------------------- RIGHT FACE -----------------------------------------------------
    unsigned int VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);

    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[2]), faces[2], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // --------------------------------------------------------------------------------------------------

    // ----------------------------------- LEFT FACE -----------------------------------------------------
    unsigned int VBO3, VAO3;
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);

    glBindVertexArray(VAO3);

    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[3]), faces[3], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // --------------------------------------------------------------------------------------------------

    // ----------------------------------- TOP FACE -----------------------------------------------------
    unsigned int VBO4, VAO4;
    glGenVertexArrays(1, &VAO4);
    glGenBuffers(1, &VBO4);

    glBindVertexArray(VAO4);

    glBindBuffer(GL_ARRAY_BUFFER, VBO4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[4]), faces[4], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // --------------------------------------------------------------------------------------------------

    // ----------------------------------- BOTTOM FACE -----------------------------------------------------
    unsigned int VBO5, VAO5;
    glGenVertexArrays(1, &VAO5);
    glGenBuffers(1, &VBO5);

    glBindVertexArray(VAO5);

    glBindBuffer(GL_ARRAY_BUFFER, VBO5);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faces[5]), faces[5], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // ----------------------------------------------------------------------------------------------------

    // 
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int VBO, lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(lightCubeVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    // --------------------------------------------------------------------------------------------------

    unsigned int texture1, texture2, texture3;
    // ----------------------------------------------- TEXTURE 1 ------------------------------------------------------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("./textures/wood.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // -----------------------------------------------------------------------------------------------------------------

    // ----------------------------------------------- TEXTURE 2 ------------------------------------------------------

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("./textures/wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // -----------------------------------------------------------------------------------------------------------------

     // ----------------------------------------------- TEXTURE 3 ------------------------------------------------------

    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("./textures/papel.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    textureLightShader.use();

    // ----------------------------------------------------------------------------------------------------------------

    float quadVertices[] = {  // Vertices que cubren toda la pantalla
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    // -----------------------------------------------------------------------------------------------------------------

    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // ---------------------------------- FRAMEBUFFER CONFIGURACION ----------------------------------------------------

    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Texture attachments
    unsigned int textureFbo;
    glGenTextures(1, &textureFbo);
    glBindTexture(GL_TEXTURE_2D, textureFbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureFbo, 0);

    // RenderBufferObject
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glad_glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: El Framebuffer no esta completo!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        textureLightShader.use();
        textureLightShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        textureLightShader.setVec3("lightPos", lightPos);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        textureLightShader.setMat4("projection", projection);
        textureLightShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        textureLightShader.setMat4("model", model);

        // render 

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindTexture(GL_TEXTURE_2D, texture3);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindTexture(GL_TEXTURE_2D, texture3);
        glBindVertexArray(VAO3);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO4);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO5);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // Bind al framebuffer por defecto y dibujar un quad plano con el framgento de textura añadido
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader.use();
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureFbo);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO0);
    glDeleteBuffers(1, &VBO0);
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteVertexArrays(1, &VAO3);
    glDeleteBuffers(1, &VBO3);
    glDeleteVertexArrays(1, &VAO4);
    glDeleteBuffers(1, &VBO4);
    glDeleteVertexArrays(1, &VAO5);
    glDeleteBuffers(1, &VBO5);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteFramebuffers(1, &fbo);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}









