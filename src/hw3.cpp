#include "hw3.h"
#include "3rdparty/glad.h" // needs to be included before GLFW!
#include "3rdparty/glfw/include/GLFW/glfw3.h"
#include "hw3_scenes.h"

using namespace hw3;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\0";

void resize_window(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

Vector3f rotate(GLfloat* vertices, Vector3 axis, Real angle){
    // TODO
    return Vector3f();
}

// initialize openGL with version 3 and for apple
void hw_initialize_gl(){
    // initialize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
}

void initialize_glad(){
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
    }   
}

GLFWwindow* newWindow(int height, int width, const char* name){
    GLFWwindow* window = glfwCreateWindow(height , width, name, NULL, NULL);

    // incase of error
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    // put the window to current context
    glfwMakeContextCurrent(window);
    return window;
}


void hw_3_1(const std::vector<std::string> &params) {
    
    hw_initialize_gl();

    // create window obj
    GLFWwindow* window = newWindow(1280, 720, "hw 3_1");
    initialize_glad();

    while(!glfwWindowShouldClose(window)){
        // input
        processInput(window);
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // glfwSetFramebufferSizeCallback(window, resize_window);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glfwTerminate();
}

void hw_3_2(const std::vector<std::string> &params) {

    hw_initialize_gl();

    // create window
    GLFWwindow* window = newWindow(1280, 720, "hw 3_2");
    initialize_glad();

    //create shader id
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    // passin shader source code
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL );
    glShaderSource(fragShader , 1, &fragShaderSource, NULL );
    // compile and assign shader
    glCompileShader(vertexShader);
    glCompileShader(fragShader);
    // check if shader successfully compiled
    int success0 = 0;
    int success1 = 0;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success0);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success1);
    if (!(success0 && success1)){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX or FRAMENTATION::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // create shader program and attach the shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    // check if linking works
    int success = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::SHADER_PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }



    // primitive input
    GLfloat vertices[] = {
    0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.0f, 0.0f
    }; 

    GLuint faces[] = {
        0 , 1 , 3,
        1 , 2 , 3
    };

    // gen VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // set vertex attributes pointers

    GLuint VBO;
    glGenBuffers(1, &VBO); //geneate a buffer with id
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the buffer type
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);


    GLuint EBO;
    glGenBuffers(1, &EBO); //geneate a buffer with id
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the buffer type
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), &faces, GL_STATIC_DRAW);
    
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 



    // render window
    while(!glfwWindowShouldClose(window)){
        // input
        processInput(window);
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use shaderProgram
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glfwSetFramebufferSizeCallback(window, resize_window);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glfwTerminate();


    // delete used shader sources
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader); 
    

}

void hw_3_3(const std::vector<std::string> &params) {
    // HW 3.3: Render a scene
    
    if (params.size() == 0) {
        return;
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;
}

void hw_3_4(const std::vector<std::string> &params) {
    // HW 3.4: Render a scene with lighting
    if (params.size() == 0) {
        return;
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;
}
