#ifndef ENTITY_H
#define ENTITY_H
#include <iostream>
#include "itsnotok.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
using namespace std;
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
        xymod = true;
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
        indexdata.erase(indexdata.begin(),indexdata.end());
        bool pushed = false;
        for(int i = 0;i < vertexdata.size();i++)
        {
            pushed = false;
            for(int j =0;j < vertexdata.size();j++)
            {
                if(vertexdata[i] == vertexdata[j])
                {

                    indexdata.push_back((unsigned short)j + posvec.size());
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
        cout << "rendering entity" << endl;

        posvec.insert(posvec.end(),vertexdata.begin(),vertexdata.end());

        indices.insert(indices.end(),indexdata.begin(),indexdata.end());
    }

private:
    int arraycurrent;
    bool setup = false;
    int health;
    float originalx,originaly,originalz;
    float x,y,z;
    vector<glm::vec4> vertexdata;
    vector<unsigned short> indexdata;
    bool isadded = false;
    bool xymod= false;
};

#endif // ENTITY_H
