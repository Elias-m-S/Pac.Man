#pragma once

class Entity {
public:
    explicit Entity(int x, int y, float speed = 1, float dirX = 0, float dirY = 0);
    virtual ~Entity() = default;

    virtual void draw(int tileSize) const = 0;

    void move();
    void setDirection(int dx, int dy);
    void setSpeed(int speed);

    inline int getX() const { return x; }
    inline int getY() const { return y; }
    inline int getSpeed() const { return speed; }
    inline int getDirX() const { return dirX; }
    inline int getDirY() const { return dirY; }



protected:
    int x;
    int y;
    float speed;
    int dirX;
    int dirY;

    void handleTunnelWrap(int mapWidth);

};