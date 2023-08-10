#include "raylib.h"
#include "raymath.h"
#include "baseCharacter.h"
#include "character.h"

class Enemy : public BaseCharacter {
public:
    Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture, float newSpeed);

    Vector2 getScreenPos() override;

    void tick(float dT) override;

    void setTarget(Character* knight);

private:
    Character *target{};
    float damagePerSec{10.f};
    float radius{25.f};
};
