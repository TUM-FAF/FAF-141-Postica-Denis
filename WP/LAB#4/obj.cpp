#include "obj.h"

Objects::Objects(POINT center, const int &speed)
 {

    this -> center = center;
    this -> xSpeed = speed;
    this -> ySpeed = speed;
    clr = RGB(0,125,0);

}

bool Objects::Accelerate(const int& dX, const int& dY)
{
    xSpeed = dX;
    ySpeed = dY;
    return TRUE;
}

bool Objects::Color(const COLORREF &clr)
{
    this -> clr = clr;
    return TRUE;
}
bool Objects::Shape(Objects &ob)
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

bool Objects::Collision(const RECT &rect)
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

bool Interaction(Objects &alfa, Objects &beta) {

    float distance;
    POINT totalSpeed;

    distance = sqrt( pow(alfa.center.x-beta.center.x,2) + pow(alfa.center.y-beta.center.y,2) );

    if ( distance < 53 )
    {
    totalSpeed.x = (abs(alfa.xSpeed) + abs(beta.xSpeed))/2;
    totalSpeed.y = (abs(alfa.ySpeed) + abs(beta.ySpeed))/2;

    alfa.Accelerate(-alfa.xSpeed*totalSpeed.x/abs(alfa.xSpeed), -alfa.ySpeed*totalSpeed.y/abs(alfa.ySpeed));
    beta.Accelerate(-beta.xSpeed*totalSpeed.x/abs(beta.xSpeed), -beta.ySpeed*totalSpeed.y/abs(beta.ySpeed));

    alfa.Color(RGB(255,0,0));
    }

    return TRUE;
}
