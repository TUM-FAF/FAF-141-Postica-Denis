#include "obj.h"

objs::objs(POINT center, const int &speed)
 {

    this -> center = center;
    this -> xSpeed = speed;
    this -> ySpeed = speed;
    clr = RGB(0,125,0);

}

bool objs::Accelerate(const int& dX, const int& dY)
{
    xSpeed = dX;
    ySpeed = dY;
    return TRUE;
}

bool objs::Color(const COLORREF &clr)
{
    this -> clr = clr;
    return TRUE;
}
bool objs::Shape(objs &ob)
{

    return TRUE;
}

bool Circle::Move(const HDC &hdc, const RECT& rect, HBRUSH &hBrush)
{
    hBrush = CreateSolidBrush(clr);
    SelectObject(hdc, hBrush);

    Collision(rect);

    center.x += xSpeed; center.y -= ySpeed;

    if(counter>0)
    {
        Rectangle(hdc, center.x-25, center.y-25, center.x+25, center.y+25 );
        SelectObject(hdc, GetStockObject(WHITE_BRUSH));
        DeleteObject(hBrush);
        return TRUE;
    }

    Ellipse(hdc, center.x-25, center.y-25, center.x+25, center.y+25 );
    SelectObject(hdc, GetStockObject(WHITE_BRUSH));
    DeleteObject(hBrush);
    return TRUE;

}

bool objs::Collision(const RECT &rect)
{
    if(center.x+25 > rect.right-1)
    {
        xSpeed =- abs(xSpeed);
        counter=1;
    }
    if(center.x-25 < rect.left+1)
    {
        xSpeed = abs(xSpeed);
        counter=1;
    }
    if(center.y+25 > rect.bottom-1)
    {
        ySpeed = abs(ySpeed);
    }
    if(center.y-25 < rect.top+1)
    {
        ySpeed = -abs(ySpeed);
    }
    return TRUE;
}

bool Interaction(objs &obj1, objs &obj2) {

    float d;

    d = sqrt( pow(obj1.center.x-obj2.center.x,2) + pow(obj1.center.y-obj2.center.y,2) );

    if ( d < 53 )
    {
    obj1.Accelerate(-obj1.xSpeed, -obj1.ySpeed);
    obj2.Accelerate(-obj2.xSpeed, -obj2.ySpeed);

    obj1.Color(RGB(255,0,0));
    }

    return TRUE;
}
