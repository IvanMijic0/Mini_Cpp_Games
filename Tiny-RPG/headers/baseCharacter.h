#ifndef BASE_CHARACTER_H
#define BASE_CHARACTER_H

#include "raylib.h"
#include "raymath.h"

class BaseCharacter {
public:
    BaseCharacter();

    Vector2 getWorldPos();

    virtual Vector2 getScreenPos() = 0;

    Texture2D getTexture();

    Texture2D getIdleTexture();

    Texture2D getRunTexture();

    [[nodiscard]] bool getAlive() const;

    void setAlive(bool isAlive);

    void undoMovement();

    virtual void tick(float dT);

    Rectangle getCollisionRec();

protected:
    Texture2D texture{};
    Texture2D idle{};
    Texture2D run{};
    Vector2 worldPos{};
    Vector2 worldPosLastFrame{};
    Vector2 vel{};
    // 1 -> facing right, -1 -> facing left
    float rightLeft{1.f};
    // Anim variables
    float runningTime{};
    float updateTime{1.f / 12.f};
    float speed{4.f};
    float width{};
    float height{};
    float scale{4.f};
    int frame{};
    int maxFrames{6};

private:
    bool alive{true};

};

#endif