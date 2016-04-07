#ifndef _objs.h_
#define _objs.h_

#include <iostream>
#include <windows.h>
#include <cmath>
#define ID_TIMER 333

using namespace std;

class objs
{

protected:
    COLORREF clr;
public:
    POINT center;
    int xSpeed,ySpeed;
    int counter=0;

    objs(POINT center,const int &speed);
    bool Accelerate(const int& deltaSpeedX, const int& deltaSpeedY);
    bool Color(const COLORREF &clr);
    bool Collision(const RECT &rect);
    bool Shape(objs &ob);
    virtual bool Move(const HDC &hdc, const RECT& rect,HBRUSH &hBrush) = 0;
};

class Circle : public objs {
public:
    Circle(POINT center,const int &speed): objs(center,speed)
    {

    }
    bool Move(const HDC &hdc,const RECT& rect,HBRUSH &hBrush);
};

bool Interaction(objs &alfa,objs &beta);
#endif
