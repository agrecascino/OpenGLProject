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
vector<unsigned short> indices;
vector<GLuint> ebuffer;
class Entity
{
public:
    Entity(float xp,float yp,float zp, int hp,vector<glm::vec4> data)
    {
        health = hp;
        x=xp;
        y=yp;
        z = zp;
        vertexdata = data;
        originalx = x;
        originaly = y;
        originalz = z;
        setxy(x,y,z);
        ebuffer.push_back(GLuint());
        arraycurrent = ebuffer.size() - 1;
        glGenBuffers(1,&ebuffer[ebuffer.size() - 1]);

    }

    void setxy(float xp, float yp, float zp)
    {

        x = xp;
        y = yp;
        z = zp;
        if(xymod){
        for(int i =0;i < vertexdata.size();i++)
        {

            vertexdata[i].x -= originalx;
            vertexdata[i].y -= originaly;
            vertexdata[i].z -= originalz;
        }
        }
        for(int i =0;i < vertexdata.size();i++)
        {

            vertexdata[i].x += xp;
            vertexdata[i].y += yp;
            vertexdata[i].z += zp;
        }
    }
    void sethp(int hp)
    {
     hp = health;
    }
    int gethp(){return health;}
    int getx(){return x;}
    int gety(){return y;}
    int getz(){return z;}
    void setdata(vector<glm::vec4> data)
    {
        vertexdata = data;
        setup =false;
    }
    void setupindexdata()
    {
        bool pushed = false;
        for(int i = 0;i < vertexdata.size();i++)
        {
            pushed = false;
            for(int j =0;j < vertexdata.size();j++)
            {
                if(vertexdata[i] == vertexdata[j])
                {
                    indexdata.push_back((unsigned short)(j + posvec.size()));
                    pushed = true;
                }
            }
            if(pushed == false)
                indexdata.push_back((unsigned short)(i + posvec.size()));
        }
        setup = true;
    }

    void render()
    {
        if(!setup)
        setupindexdata();
        posvec.insert(posvec.end(),vertexdata.begin(),vertexdata.end());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebuffer[arraycurrent]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexdata.size() * sizeof(unsigned short),&indexdata[0],GL_STATIC_DRAW);
    }

private:
    int arraycurrent;
    bool setup;
    int health;
    float originalx,originaly,originalz;
    float x,y,z;
    vector<glm::vec4> vertexdata;
    vector<unsigned short> indexdata;
    bool isadded = false;
    bool xymod= false;
};

class Camera
{   public:
    Camera(glm::vec3 pos, float h,float v, float fov, float s = 3, float mousespeed = 0.005f)
    {
     hangle = h;
     position = pos;
     vangle = v;
     initfov = fov;
     speed = s;
     mousesens = mousespeed;

    }

    void CameraUpdateLoop()
    {
        xpos = 0;
        ypos = 0;
        deltatime = glfwGetTime() - prevtime;
        glfwGetCursorPos(window,&xpos,&ypos);

        glfwSetCursorPos(window,800/2,600/2);
        hangle += mousesens * deltatime *  (int)(400 - xpos);
        vangle += mousesens * deltatime *   (int)(300 - ypos);
        direction = glm::vec3(cos(vangle)* sin(hangle),sin(vangle),cos(vangle)* cos(hangle));
        right = glm::vec3(sin(hangle - 3.14f/2.0f),0,cos(hangle - 3.14f/2.0f));
        up = glm::vec3(glm::cross(right,direction));
        if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
        {
            position += direction * deltatime * speed;
        }
        if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
        {
            position -= direction * deltatime * speed;
        }
        if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS)
        {
            position += right * deltatime * speed;
        }
        if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)
        {
            position -= right * deltatime * speed;
        }
        if(vangle < -90.0f)
        {
            vangle = -90.0f;
        }
        if(vangle > 90.0f)
        {
            vangle = 90.0f;
        }
        cout << vangle << endl;
        cout << hangle << endl;
        ProjectionMatrix = glm::perspective(initfov,4.0f/3.0f,0.1f,100.0f);
        ViewMatrix= (glm::lookAt(position,position+direction,up));
        prevtime = glfwGetTime();
    }
    glm::mat4 ProjectionMatrix = glm::perspective(initfov,4.0f/3.0f,0.1f,100.0f);
    glm::mat4 ViewMatrix= (glm::lookAt(position,position+direction,up));


private:
    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;
    float prevtime;
    double xpos,ypos;
    glm::vec3 position;
    float hangle;
    float vangle;
    float initfov;
    float speed;
    float deltatime;
    float mousesens;
};
Camera MainCamera(glm::vec3(0,0,0),3.14f,0.0f,45.0f,3,0.05f);
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
class Map
{
public:
    Map()
    {
        vec.push_back(glm::vec4(1.3,1.2,1.1,1.0));
        vec.push_back(glm::vec4(1.4,0.5 ,1.2,1.0));
        vec.push_back(glm::vec4(1.5,1.4,1.3,1.0));
    }

void loadmap(string mapfile)
{
    entity = new Entity(3.0f,-6.0f,5.0f,10,vec);
    fstream file(mapfile,ios::in);

    string pusher;
    vector<string> stringvec;
    while(getline(file,pusher))
        stringvec.push_back(pusher);

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
               bool isthere = false;
               unsigned int vertlocation;
               cout << "we are at:" << k << endl;
               for(int l =0;l < vecpusher.size(); l++)
               {

                   if(glm::vec4(stof(halfconv[k]),stof(halfconv[k+1]),stof(halfconv[k+2]),1.0) == vecpusher[l])
                   {
                   isthere = true;
                   vertlocation = l;
                   }
               }
               if(isthere == false)
               {
               vecpusher.push_back(glm::vec4(stof(halfconv[k]),stof(halfconv[k+1]),stof(halfconv[k+2]),1.0));
               localindices.push_back((unsigned short)vecpusher.size() - 1);
               }
               else
               localindices.push_back((unsigned short)vertlocation);
               k += 2;
           }
           j++;
        }
        if(valstring[j] == "music")
        {
            j++;
        }
    }
    }
    file.close();

}
void mapeventloop()
{
    //entity->setxy(sin(entity->getx()),cos(entity->gety()),sin(entity->getz()));
    MainCamera.CameraUpdateLoop();
    //if()
}

void render()
{

    posvec.insert(posvec.end(),vecpusher.begin(),vecpusher.end());
    indices.insert(indices.end(),localindices.begin(),localindices.end());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebuffer[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned short),&indices[0], GL_STATIC_DRAW);
    entity->render();
}

private:
vector<Entity> entities;
vector<glm::vec4> vec;
Entity *entity = NULL;
vector<glm::vec4> vecpusher;
vector<unsigned short> localindices;
};
Map currentmap;
void init3d()
{

  glGenVertexArrays(1,&varray);
  glBindVertexArray(varray);

 //posvec.push_back(glm::vec4(-1.0,-1.0,0.5,1.0));
  //posvec.push_back(glm::vec4(1.0,-1.0,0.5,1.0));
   //posvec.push_back(glm::vec4(0.0,1.0,0.5,1.0));
  ebuffer.push_back(GLuint());
 glGenBuffers(1,&ebuffer[0]);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebuffer[0]);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned short),&indices[0], GL_STATIC_DRAW);
 currentmap.loadmap("map");
   //posvec.push_back(glm::vec4(2.0,6.0,1.0,1.0));
 glClearColor(0.0,0.3,0.8,1.0);
 glGenBuffers(1,&buffer);
 glBindBuffer(GL_ARRAY_BUFFER,buffer);
 glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * posvec.size(),posvec.data(), GL_STATIC_DRAW);
 program = LoadShaders("vertex","fragment");
 MVPloc = glGetUniformLocation(program, "MVP");
 glEnable(GL_DEPTH_TEST);
 glDepthFunc(GL_LEQUAL);
 glEnable(GL_POINT_SIZE);
 glPointSize(20.0f);
 //glUniformMatrix4fv(MVPloc,1,GL_FALSE,&MVP[0][0]);
 glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
}
GLFWcursor* BlankCursor()
{
int w=1;//16;
int h=1;//16;
unsigned char pixels[w * h * 4];
memset(pixels, 0x00, sizeof(pixels));
GLFWimage image;
image.width = w;
image.height = h;
image.pixels = pixels;
return glfwCreateCursor(&image, 0, 0);
}
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_SAMPLES,64);
    window = glfwCreateWindow(800,600,"title",NULL,NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = true;
    glewInit();
    init3d();

    glfwSetCursor(window,BlankCursor());
    glfwSetInputMode(window,GLFW_STICKY_KEYS,GL_TRUE);
    while(!glfwWindowShouldClose(window) && !glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        posvec.erase(posvec.begin(),posvec.end());
     indices.erase(indices.begin(),indices.end());
     currentmap.render();
     currentmap.mapeventloop();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(program);

    MVP = MainCamera.ProjectionMatrix * MainCamera.ViewMatrix * glm::mat4(1.0f);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * posvec.size(),posvec.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,(void*)0);
    glUniformMatrix4fv(MVPloc,1,GL_FALSE,&MVP[0][0]);
\

    for(int i =0;i < ebuffer.size();i++)
    {
        int bufsize = 0;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebuffer[i]);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER,GL_BUFFER_SIZE, &bufsize);
    bufsize /= sizeof(unsigned short);
    //cant wake up
    glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_SHORT,(void*)0);
    }


    glFlush();
    glfwSwapBuffers(window);
    glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

