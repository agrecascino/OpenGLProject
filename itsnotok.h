#ifndef ITSNOTOK_H
#define ITSNOTOK_H
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <vector>
#include <Newton.h>
#include <glm/gtx/transform.hpp>
using namespace std;
GLFWwindow *window;
NewtonWorld *CollisionWorld;
GLuint buffer;
GLuint varray;
vector<glm::vec4> posvec;
GLuint program;
GLuint MVPloc;
glm::mat4 MVP;
vector<unsigned short> indices;
vector<GLuint> ebuffer;
struct orientation
{
    bool posx = false;
    bool posy = false;
    bool posz = false;
    bool negx = false;
    bool negy = false;
    bool negz = false;
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
    void CameraInitCollision()
    {
        glm::mat4 initmatrix = glm::mat4(1.0f);
        initmatrix[3][0] = position.x;
        initmatrix[3][1] = position.y;
        initmatrix[3][2] = position.z;
        //const	float initialTM[16]	=	{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0, 0, 0.0f, 1.0f };
        CameraAABB = NewtonCreateBox(CollisionWorld,1,3,1,0,NULL);

        //http://newtondynamics.com/wiki/index.php5?title=Super_simple_quick-start_with_48_lines_of_C_example
        //http://irrlicht3d.org/wiki/index.php?n=Main.BasicCollisionDetection
    }


    void CameraUpdateLoop()
    {
        xpos = 0;
        ypos = 0;
       prevpos = position;
        deltatime = glfwGetTime() - prevtime;
        glfwGetCursorPos(window,&xpos,&ypos);
        //  cout << xpos << " " << ypos << endl;
        glfwSetCursorPos(window,800/2,600/2);
        hangle += mousesens * deltatime *  (400 - xpos);
        vangle += mousesens * deltatime *  (300 - ypos);

        direction = glm::vec3(cos(vangle)* sin(hangle),sin(vangle),cos(vangle)* cos(hangle));
        //right = glm::vec3(1.0,0.0,0.0);
        right = glm::vec3(sin(hangle - 3.14f/2.0f),0,cos(hangle - 3.14f/2.0f));
        up = glm::vec3(glm::cross(right,direction));
        if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS && !wdis)
        {
            position += direction * deltatime * speed;
            //rigidbody->translate(btVector3(direction.x,direction.y,direction.z) * deltatime * speed);

        }
        if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS && !sdis)
        {
            position -= direction * deltatime * speed;
            //rigidbody->translate(-(btVector3(direction.x,direction.y,direction.z) * deltatime * speed));
        }
        if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS && !ddis)
        {
            position += right * deltatime * speed;

            //rigidbody->translate(btVector3(right.x,right.y,right.z) * deltatime * speed);
        }
        if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS && !adis)
        {
            position -= right * deltatime * speed;
            //rigidbody->translate(-(btVector3(right.x,right.y,right.z) * deltatime * speed));
        }


        ModelFacing.negx = false;
        ModelFacing.negy = false;
        ModelFacing.negz = false;
        ModelFacing.posx = false;
        ModelFacing.posy = false;
        ModelFacing.posz = false;
        if(direction.x >= 0 ){
            ModelFacing.posx = true;
        }
        else{
            ModelFacing.negx = true;
        }
        if(direction.y > 0){
            ModelFacing.posy = true;
        }
        else if(direction.y < 0){
            ModelFacing.negy = true;
        }
        if(direction.z >= 0){
            ModelFacing.posz = true;
        }
        else{
            ModelFacing.negz = true;
        }
        TransformationMatrix[0][3] = position.x;
        TransformationMatrix[1][3] = position.y;
        TransformationMatrix[2][3] = position.z;
        cout << TransformationMatrix[2][3] << endl;
        // cout << vangle << endl;
        // cout << hangle << endl;
        ProjectionMatrix = glm::perspective(initfov,16.0f/9.0f,0.01f,1000.0f);
        ViewMatrix= (glm::lookAt(position,position+direction,up));
        prevtime = glfwGetTime();

    }
    glm::mat4 GetCollisionTransformation()
    {

        return glm::transpose(TransformationMatrix);
    }
    bool wdis = false,sdis = false,adis = false,ddis = false;
    orientation ModelFacing;
    glm::mat4 ProjectionMatrix = glm::perspective(initfov,16.0f/9.0f,0.01f,100.0f);
    glm::mat4 ViewMatrix= (glm::lookAt(position,position+direction,up));
    glm::vec3 position;
    glm::mat4 TransformationMatrix = glm::mat4(1.0f);
    float deltatime;
    NewtonCollision *CameraAABB;
    glm::vec3 prevpos;


private:


    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;
    float prevtime;
    double xpos,ypos;
    float hangle;
    float vangle;
    float initfov;
    float speed;
    float mousesens;
};
Camera MainCamera(glm::vec3(0,0,0),3.14f,0.0f,45.0f,3,0.05f);
struct Collision
{
    Collision(glm::vec3 loc,glm::vec3 norm, glm::vec3 pen)
    {
        location = loc;
        normal = norm;
        penetration = pen;
        if(loc.x > MainCamera.position.x - 1.7 && loc.x < MainCamera.position.x + 1.7)
        {
            if(loc.z > MainCamera.position.z - 1.7 && loc.z < MainCamera.position.z + 1.7)
            {
                if(loc.y < MainCamera.position.y && loc.y > MainCamera.position.y - 1.5  )
                {
                    isfloor = true;
                    cout << "floor detected" << endl;
                }
            }
        }
    }

    glm::vec3 location;
    glm::vec3 normal;
    glm::vec3 penetration;
    bool isfloor;

};


#endif // ITSNOTOK_H
