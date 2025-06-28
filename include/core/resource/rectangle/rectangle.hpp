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

    bool collidesWith(const Rect* other) const
    {
        return !(_x + _width < other->_x || _x > other->_x + other->_width ||
                 _y + _height < other->_y || _y > other->_y + other->_height);
    }

    Rect* clip(const Rect* other) const
    {
        float x1 = std::max(getX(), other->getX());
        float y1 = std::max(getY(), other->getY());
        float x2 = std::min(getRight(), other->getRight());
        float y2 = std::min(getBottom(), other->getBottom());

        float width = x2 - x1;
        float height = y2 - y1;

        if (width <= 0.0f || height <= 0.0f) {
            return nullptr; // No intersection
        }

        return new Rect(x1, y1, width, height);
    }
};
