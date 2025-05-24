#include "Entity.h"

class PacMan : public Entity {
public:
    PacMan(int startX, int startY);
    void draw(int tileSize) const override;

    void addScore(int amount);
    int getScore() const;

private:
    int score;
};
