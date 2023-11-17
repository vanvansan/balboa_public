#include "hw3.h"
#include "3rdparty/glad.h" // needs to be included before GLFW!
#include "3rdparty/glfw/include/GLFW/glfw3.h"
#include "hw3_scenes.h"

#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>
#include <glm-master/glm/gtc/type_ptr.hpp>


using namespace hw3;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "// layout (location = 1) in vec2 aTexCoord;\n"

    "// out vec2 TexCoord;\n"
    "uniform mat4 transform;\n"
    "void main()\n"
    "{\n"
        "gl_Position = transform * vec4(aPos, 1.0);\n"
        "// TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
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

GLuint hw_compileShader(const char* source ,GLenum type){
    //create shader id
    GLuint ID = glCreateShader(type);
    // passin shader source code
    glShaderSource(ID, 1, &source, NULL );
    // compile and assign shader
    glCompileShader(ID);
    // check if shader successfully compiled
    int success = 0;
    char infoLog[512];
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!(success)){
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::???::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return ID;
}


GLuint hw_link_pragram(GLuint vertexShader, GLuint fragShader){
    GLuint shaderProgram =  glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
    char infoLog[512];
    // check if linking works
    int success = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::SHADER_PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // delete used shader sources
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader); 
    return shaderProgram;
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
    GLFWwindow* window = newWindow(720, 720, "hw 3_2");
    initialize_glad();

    // compile shader
    GLuint vertexShader = hw_compileShader(vertexShaderSource ,GL_VERTEX_SHADER);
    GLuint fragShader = hw_compileShader(fragShaderSource ,GL_FRAGMENT_SHADER);
    
    // link the shade to program
    GLuint shaderProgram = hw_link_pragram(vertexShader, fragShader);
    

    // primitive input
    GLfloat vertices[] = {
    0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.0f, 0.5f,
     -0.5f,  0.5f, 0.0f   
    }; 

    GLuint faces[] = {
        0 , 1 , 3,
        1 , 2 , 3,
        0, 2, 3
    };

    // gen VAO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); //geneate a buffer with id
    glGenBuffers(1, &EBO); //geneate a buffer with id


    glBindVertexArray(VAO);
    // set vertex attributes pointers

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the buffer type
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the buffer type
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), &faces, GL_STATIC_DRAW);

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0); 

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );

    // render window
    while(!glfwWindowShouldClose(window)){
        // input
        processInput(window);
        // clear color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // create transformations
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // pass to shaderProgram
        glUseProgram(shaderProgram);
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // render the triangle
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 1, 3); // draw one triangle
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSetFramebufferSizeCallback(window, resize_window);
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glfwTerminate();
}

void hw_3_3(const std::vector<std::string> &params) {
    if (params.size() == 0) {
        return;
    }
    // parsing scene
    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;
    Camera camera = scene.camera;
    Vector3f background = scene.background;
    std::vector<TriangleMesh> meshes = scene.meshes;
    TriangleMesh mesh;


    hw_initialize_gl();

    // create window
    GLFWwindow* window = newWindow(720, 720, "hw 3_2");
    initialize_glad();

    // compile shader
    // GLuint vertexShader = hw_compileShader(vertexShaderSource ,GL_VERTEX_SHADER);
    // GLuint fragShader = hw_compileShader(fragShaderSource ,GL_FRAGMENT_SHADER);
    Shader shader("hw3_3_vert", "hw3_3_frag");
    
    // link the shade to program
    // GLuint shaderProgram = hw_link_pragram(vertexShader, fragShader);
    

    // primitive input
    GLfloat vertices[] = {
    0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.0f, 0.5f,
     -0.5f,  0.5f, 0.0f   
    }; 

    GLuint faces[] = {
        0 , 1 , 3,
        1 , 2 , 3,
        0, 2, 3
    };

    // gen VAO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); //geneate a buffer with id
    glGenBuffers(1, &EBO); //geneate a buffer with id


    glBindVertexArray(VAO);
    // set vertex attributes pointers

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the buffer type
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the buffer type
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), &faces, GL_STATIC_DRAW);

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0); 

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );

    // render window
    while(!glfwWindowShouldClose(window)){
        // input
        processInput(window);
        // clear color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // create transformations
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        // pass to shaderProgram
        // glUseProgram(shader);
        shader.use();
        unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // render the triangle
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 1, 3); // draw one triangle
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSetFramebufferSizeCallback(window, resize_window);
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glfwTerminate();
    
}

void hw_3_4(const std::vector<std::string> &params) {
    // HW 3.4: Render a scene with lighting
    if (params.size() == 0) {
        return;
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;
}

