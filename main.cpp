#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <thread>
#include <chrono>
#include "itsnotok.h"
#include <fstream>
#include <Newton.h>
//#include <freetype2/ft2build.h>
//#include <freetype2/freetype.h>
#include <cstdio>
#include "moduleplayer.h"
//#include <regex>

#include "entity.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace std;

//FT_Library ft;

//FT_Face face;

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

    }

    void loadmap(string mapfile)
    {

        fstream file(mapfile,ios::in);

        string pusher;
       // bool v1 = false,v2 = false,v3 = false;
        //btVector3 triangle[3];
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


                        //cout << "we are at:" << k << endl;
                        vec.push_back(glm::vec3(stof(halfconv[k]),stof(halfconv[k+1]),stof(halfconv[k+2])));
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
                    thread t(playsong,valstring[j+1]);
                    t.detach();
                    j++;
                }
                if(valstring[j] == "entity")
                {
                    vector<string> halfconv;
                    boost::algorithm::split(halfconv,valstring[j+1],boost::is_any_of(","),boost::token_compress_on);
                    Entity *pusher = Loadentity(stof(halfconv[0]),stof(halfconv[1]),stof(halfconv[2]),halfconv[3]);
                    if(pusher != NULL)
                    {

                        entities.push_back(*pusher);
                        cout << "entity pushed" << endl;
                    }
                    j++;
                }
            }
        }
        file.close();
        godhelpme();

    }
    void mapeventloop()
    {
        //entity->setxy(sin(entity->getx()),cos(entity->gety()),sin(entity->getz()));
        MainCamera.CameraUpdateLoop();

        render();
        collide();
    }
    void collide()
    {
        int collision = 0;
        float* a = new float[30];
        float* b = new float[30];
        float* c = new float[30];
        vector<glm::vec3> avec;
        vector<glm::vec3> bvec;
        vector<glm::vec3> cvec;
        avec.erase(avec.begin(),avec.end());
        bvec.erase(bvec.begin(),bvec.end());
        cvec.erase(cvec.begin(),cvec.end());
        long long *stuff = new long long[320];
        long long *stuff2 = new long long[320];
        //*((char*) NULL ) = 0;




        collision = NewtonCollisionCollide(CollisionWorld,10,MainCamera.CameraAABB,&MainCamera.GetCollisionTransformation()[0][0],MapCollision,&glm::transpose(glm::mat4(1.0f))[0][0],(float*)a,(float*)b,(float*)c,(long long*)stuff,(long long*)stuff2,0);

        if(collision > 0)
        {
        for(int i =0;i < 30;i += 3)
        {
            avec.push_back(glm::vec3(a[i],a[i+1],a[i+2]));
            bvec.push_back(glm::vec3(b[i],b[i+1],b[i+2]));
            cvec.push_back(glm::vec3(c[i],c[i+1],c[i+2]));
        }
        vector<Collision> collisions;
        for(int i = 0; i < avec.size();i++)
        {
            collisions.push_back(Collision(avec[i],bvec[i],cvec[i]));
        }
      //  }
        //MainCamera.wdis = false;
        //MainCamera.ddis = false;
        //MainCamera.adis = false;
        //MainCamera.sdis = false;
        for(int i =0;i < collisions.size();i++)
        {
           /*
           if(MainCamera.position.x > avec[i].x && MainCamera.ModelFacing.negx)
           {
               MainCamera.sdis = true;
               MainCamera.position.x += 0.05;
           }
           */

          //normal + pen try??????????????????
           /*
           if(MainCamera.position.x > avec[i].x && MainCamera.ModelFacing.negx )
           {
               MainCamera.position.x = MainCamera.prevpos.x;
           }
           if(MainCamera.position.y > avec[i].y && MainCamera.ModelFacing.negy)
           {
               MainCamera.position.y = MainCamera.prevpos.y;
           }
           if(MainCamera.position.y < avec[i].y && MainCamera.ModelFacing.posy)
           {
               MainCamera.position.y =  MainCamera.prevpos.y;
           }
           if(MainCamera.position.x < avec[i].x && MainCamera.ModelFacing.posx)
           {

               MainCamera.position.z =MainCamera.prevpos.x;
           }

           if(MainCamera.position.z > avec[i].z && MainCamera.ModelFacing.negz)
           {

               MainCamera.position.z = MainCamera.prevpos.z;
           }
           if(MainCamera.position.z < avec[i].z && MainCamera.ModelFacing.posz)
           {

               MainCamera.position.z = MainCamera.prevpos.z;
           }*/
           //finish collision using right and -right and fix one x direction
            if(MainCamera.position != MainCamera.prevpos && !collisions[i].isfloor)
             {
             MainCamera.position -= MainCamera.position - MainCamera.prevpos;

             }
            else
            {
                MainCamera.position.y = MainCamera.prevpos.y;
            }

        }

           // else
          // {
           //     MainCamera.position.y += 1;
           // }

        }
       else
       {
         MainCamera.position.y -= 0.2;
       }


    }

    void godhelpme()
    {
       // vector<Point> points_yo;
        //IndexedTriangle k;

        //vector<IndexedTriangle> itri;
        //k.mVRef



        MapCollision = NewtonCreateTreeCollision(CollisionWorld,NULL);
        //OPCC.mIMesh->SetPointers(localindices.data(),vecpusher.data());
        NewtonTreeCollisionBeginBuild(MapCollision);
        for(int i = 0;i < vec.size();i += 3)
        {
            float Mapping[9] = {vec[i].x,vec[i].y,vec[i].z,vec[i+1].x,vec[i+1].y,vec[i+1].z,vec[i+2].x,vec[i+2].y,vec[i+2].z};
            NewtonTreeCollisionAddFace(MapCollision,3,(float*)Mapping,3*sizeof(float),1);
            //helpme.addTriangle(btVector3(vec[i].x,vec[i].y,vec[i].z),btVector3(vec[i+1].x,vec[i+1].y,vec[i+1].z),btVector3(vec[i+2].x,vec[i+2].y,vec[i+2].z));
           /* points_yo.push_back(Point(vec[i].x,vec[i].y,vec[i].z));
            points_yo.push_back(Point(vec[i+1].x,vec[i+1].y,vec[i+1].z));
            points_yo.push_back(Point(vec[i+2].x,vec[i+2].y,vec[i+2].z));
            itri.push_back(IndexedTriangle());

            itri[i].mVRef[0] = i;
            itri[i].mVRef[1] = i+1;
            itri[i].mVRef[2] = i+2;*/


        }
        NewtonTreeCollisionEndBuild(MapCollision,0);



        MainCamera.CameraInitCollision();

        //make this do something
    }

    void render()
    {

        posvec.insert(posvec.end(),vecpusher.begin(),vecpusher.end());
        indices.insert(indices.end(),localindices.begin(),localindices.end());
        for(int i = 0;i < entities.size();i++)
        {
            entities[i].render();
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebuffer[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned short),&indices[0], GL_STATIC_DRAW);
     }

private:
    Entity *Loadentity(int x,int y,int z,string filename)
    {

        Entity *returnthis = NULL;
        fstream entityf(filename,ios::in);
        string data;

        int hp = 10;

        vector<string> dataterp;
        vector<glm::vec4> vectordata;
        while(getline(entityf,data))
            dataterp.push_back(data);
        for(int i = 0;i < dataterp.size();i++)
        {
            boost::trim_if(dataterp[i], boost::is_any_of("\t "));
            vector<string> push;

            boost::algorithm::split(push,dataterp[i],boost::is_any_of(":"),boost::token_compress_on);
            for(int  j =0;j < push.size();j++)
            {
                if(push[j] == "hp")
                {
                    hp = stoi(push[j + 1]);
                    j++;
                }
                if(push[j] == "vector")
                {
                    vector<string> actualdata;
                    boost::split(actualdata,push[j+1],boost::is_any_of(","),boost::token_compress_on);
                    for(int i =0;i < actualdata.size();i++)
                        cout << actualdata[i] << endl;
                    vectordata.push_back(glm::vec4(stof(actualdata[0]),stof(actualdata[1]),stof(actualdata[2]),1.0));
                    j++;
                }
 
            }
        }
        returnthis = new Entity(x,y,z,hp,vectordata);
        entityf.close();
        return returnthis;
    }

    //Model MapModel;
    vector<Entity> entities;
    vector<glm::vec3> vec;
    NewtonBody *MapBody;
    NewtonCollision *MapCollision;
    vector<glm::vec4> vecpusher;
    vector<unsigned short> localindices;
};
Map currentmap;
void init3d()
{

    CollisionWorld = NewtonCreate();
    NewtonSetSolverModel(CollisionWorld,1);
    //FT_Init_FreeType(&ft);
    //FT_New_Face(ft,"FreeSans.ttf",0,&face);
    //FT_Set_Pixel_Sizes(face,0,48);
    //FT_Load_Char(face, 'A',FT_LOAD_RENDER);
    //Opcode::InitOpcode();
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    glfwSwapInterval(1);
    //glUniformMatrix4fv(MVPloc,1,GL_FALSE,&MVP[0][0]);
    //glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
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
    window = glfwCreateWindow(1280,720,"title",NULL,NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = true;
    glewInit();
    init3d();

    //glfwSetCursor(window,BlankCursor());
    glfwSetInputMode(window,GLFW_STICKY_KEYS,GL_TRUE);
    while(!glfwWindowShouldClose(window) && !glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        posvec.erase(posvec.begin(),posvec.end());
        indices.erase(indices.begin(),indices.end());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program);
        currentmap.mapeventloop();
        MVP = MainCamera.ProjectionMatrix * MainCamera.ViewMatrix * glm::mat4(1.0f);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * posvec.size(),posvec.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,(void*)0);
        glUniformMatrix4fv(MVPloc,1,GL_FALSE,&MVP[0][0]);



        int bufsize = 0;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebuffer[0]);
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER,GL_BUFFER_SIZE, &bufsize);
        bufsize /= sizeof(unsigned short);

        glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_SHORT,(void*)0);



        glFlush();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

