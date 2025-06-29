#include "core/game_world/resource/rectangle/rectangle.hpp"

bool Rect::collidesWith(const Rect* other) const
{
    return !(_x + _width < other->_x || _x > other->_x + other->_width ||
                _y + _height < other->_y || _y > other->_y + other->_height);
}

Rect* Rect::clip(const Rect* other) const
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