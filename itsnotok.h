#ifndef ITSNOTOK_H
#define ITSNOTOK_H
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <vector>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

using namespace std;
GLFWwindow *window;
btBroadphaseInterface* broadphase = new btDbvtBroadphase();
btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();
btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
GLuint buffer;
GLuint varray;
vector<glm::vec4> posvec;
GLuint program;
GLuint MVPloc;
glm::mat4 MVP;
vector<unsigned short> indices;
vector<GLuint> ebuffer;

#endif // ITSNOTOK_H
