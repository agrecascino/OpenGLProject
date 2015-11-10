#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <cstdio>
//#include <regex>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace std;
GLFWwindow *window;
GLuint buffer;
GLuint varray;
vector<glm::vec4> posvec;
GLuint program;
GLuint MVPloc;
glm::mat4 MVP;
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){



    // Create the shaders

    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);

    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);



    // Read the Vertex Shader code from the file

    std::string VertexShaderCode;

    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);

    if(VertexShaderStream.is_open()){

        std::string Line = "";

        while(getline(VertexShaderStream, Line))

            VertexShaderCode += "\n" + Line;

        VertexShaderStream.close();

    }else{

        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);

        getchar();

        return 0;

    }



    // Read the Fragment Shader code from the file

    std::string FragmentShaderCode;

    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);

    if(FragmentShaderStream.is_open()){

        std::string Line = "";

        while(getline(FragmentShaderStream, Line))

            FragmentShaderCode += "\n" + Line;

        FragmentShaderStream.close();

    }



    GLint Result = GL_FALSE;

    int InfoLogLength;





    // Compile Vertex Shader

    printf("Compiling shader : %s\n", vertex_file_path);

    char const * VertexSourcePointer = VertexShaderCode.c_str();

    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);

    glCompileShader(VertexShaderID);



    // Check Vertex Shader

    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);

    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if ( InfoLogLength > 0 ){

        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);

        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);

        printf("%s\n", &VertexShaderErrorMessage[0]);

    }







    // Compile Fragment Shader

    printf("Compiling shader : %s\n", fragment_file_path);

    char const * FragmentSourcePointer = FragmentShaderCode.c_str();

    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);

    glCompileShader(FragmentShaderID);



    // Check Fragment Shader

    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);

    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if ( InfoLogLength > 0 ){

        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);

        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);

        printf("%s\n", &FragmentShaderErrorMessage[0]);

    }







    // Link the program

    printf("Linking program\n");

    GLuint ProgramID = glCreateProgram();

    glAttachShader(ProgramID, VertexShaderID);

    glAttachShader(ProgramID, FragmentShaderID);

    glLinkProgram(ProgramID);



    // Check the program

    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);

    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if ( InfoLogLength > 0 ){

        std::vector<char> ProgramErrorMessage(InfoLogLength+1);

        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);

        printf("%s\n", &ProgramErrorMessage[0]);

    }





    glDetachShader(ProgramID, VertexShaderID);

    glDetachShader(ProgramID, FragmentShaderID);



    glDeleteShader(VertexShaderID);

    glDeleteShader(FragmentShaderID);



    return ProgramID;

 }
void loadmap(string mapfile)
{
    fstream file(mapfile,ios::in);

    string pusher;
    vector<string> stringvec;
    while(getline(file,pusher))
        stringvec.push_back(pusher);
    vector<glm::vec4> vecpusher;
    vector<string> valstring;
    for(int i =0;i < stringvec.size();i++)
    {
    boost::trim_if(stringvec[i], boost::is_any_of("\t "));
    boost::algorithm::split(valstring,stringvec[i],boost::is_any_of(":"),boost::token_compress_on);
    for(int j =0;j < valstring.size();j++)
    {
        if(valstring[j] == "vector")
        {
            vector<string> halfconv;
           boost::algorithm::split(halfconv, valstring[j + 1],boost::is_any_of(","),boost::token_compress_on);
           for(int k = 0;k < halfconv.size();k++)
           {
               vecpusher.push_back(glm::vec4(stof(halfconv[k]),stof(halfconv[k+1]),stof(halfconv[k+2]),1.0));
               k += 2;
           }
           j++;
        }
    }
    }
    posvec.insert(posvec.end(),vecpusher.begin(),vecpusher.end());
    file.close();
}

void init3d()
{
  glGenVertexArrays(1,&varray);
  glBindVertexArray(varray);
  loadmap("map");
 //posvec.push_back(glm::vec4(-1.0,-1.0,0.5,1.0));
  //posvec.push_back(glm::vec4(1.0,-1.0,0.5,1.0));
   //posvec.push_back(glm::vec4(0.0,1.0,0.5,1.0));

   //posvec.push_back(glm::vec4(2.0,6.0,1.0,1.0));
 glClearColor(0.0,0.3,0.8,1.0);
 glGenBuffers(1,&buffer);
 glBindBuffer(GL_ARRAY_BUFFER,buffer);
 glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * posvec.size(),posvec.data(), GL_STATIC_DRAW);
 program = LoadShaders("vertex","fragment");
 MVPloc = glGetUniformLocation(program, "MVP");
 glm::mat4 Model = glm::mat4(1.0f);
 glm::mat4 View = glm::lookAt(glm::vec3(4,3,3),glm::vec3(0,0,0),glm::vec3(0,1,0));
 glm::mat4 Projection = glm::perspective(45.0f,4.0f / 3.0f,0.1f,100.0f);
 MVP = Projection * View * Model;
 //glUniformMatrix4fv(MVPloc,1,GL_FALSE,&MVP[0][0]);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_SAMPLES,2);
    window = glfwCreateWindow(800,600,"title",NULL,NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = true;
    glewInit();
    init3d();
    glfwSetInputMode(window,GLFW_STICKY_KEYS,GL_TRUE);
    while(!glfwWindowShouldClose(window) && !glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,(void*)0);
    glUniformMatrix4fv(MVPloc,1,GL_FALSE,&MVP[0][0]);
    glDrawArrays(GL_TRIANGLE_STRIP,0,posvec.size());

    glFlush();
    glfwSwapBuffers(window);
    glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

