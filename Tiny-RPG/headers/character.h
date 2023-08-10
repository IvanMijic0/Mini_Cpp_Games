#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"
#include "raymath.h"
#include "baseCharacter.h"

class Character : public BaseCharacter {
public:
    Character(int w_dim, Texture2D idle_texture, Texture2D run_texture);

    Texture2D getWeaponTexture();

    Vector2 getScreenPos() override;

    Rectangle getWeaponCollisionRec();

    void tick(float dT) override;

    [[nodiscard]] float getHealth() const;

    void takeDamage(float damage);

private:
    int w_dim{};
    float rot{};
    float health{100.f};
    Texture2D weapon{LoadTexture("../assets/characters/weapon_sword.png")};
    Rectangle weaponCollisionRec{};
    Vector2 origin{};
    Vector2 offset{};
};

#endif