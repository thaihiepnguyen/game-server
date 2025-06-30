#pragma once

#include <cmath>
#include <algorithm>

class Rect
{
private:
    float _x;
    float _y;
    float _width;
    float _height;

public:
    Rect() : _x(0), _y(0), _width(0), _height(0) {}

    Rect(float x, float y, float width, float height)
        : _x(x), _y(y), _width(width), _height(height) {}

    float getX() const { return _x; }
    float getY() const { return _y; }
    float getBottom() const { return _y + _height; }
    float getRight() const { return _x + _width; }
    float getWidth() const { return _width; }
    float getHeight() const { return _height; }
    float getCenterX() const { return _x + _width / 2; }
    float getCenterY() const { return _y + _height / 2; }

    void setX(float x) { _x = x; }
    void setY(float y) { _y = y; }
    void setBottom(float bottom) { _y = bottom - _height; }
    void setRight(float right) { _x = right - _width; }
    void setWidth(float width) { _width = width; }
    void setHeight(float height) { _height = height; }

    bool collidesWith(const Rect *other) const;

    Rect *clip(const Rect *other) const;
};
