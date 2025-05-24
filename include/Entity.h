#pragma once

class Entity {
public:
    Entity(int x, int y);
    virtual ~Entity() = default;

    virtual void draw(int tileSize) const = 0;

    void move(int dx, int dy);
    int getX() const;
    int getY() const;

protected:
    int x;
    int y;
};
