#ifndef ITSNOTOK_H
#define ITSNOTOK_H
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <vector>
#include <Newton.h>
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


#endif // ITSNOTOK_H
