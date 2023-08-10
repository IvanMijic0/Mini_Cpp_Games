#include "../headers/character.h"

Character::Character(int w_dim, Texture2D idle_texture, Texture2D run_texture) : w_dim(w_dim) {
    texture = idle_texture;
    idle = idle_texture;
    run = run_texture;
    width = static_cast<float>(texture.width) / static_cast<float>(maxFrames);
    height = static_cast<float>(texture.height);
}

Texture2D Character::getWeaponTexture() { return weapon; }

Rectangle Character::getWeaponCollisionRec() { return weaponCollisionRec; }

float Character::getHealth() const { return health; }

Vector2 Character::getScreenPos() {
    return Vector2{
            static_cast<float>(w_dim) * .5f - scale * (.5f * width),
            static_cast<float>(w_dim) * .5f - scale * (.5f * height)
    };
}

void Character::tick(float dT) {
    if (!getAlive()) return;

    if (IsKeyDown(KEY_A)) vel.x -= 1.f;
    if (IsKeyDown(KEY_D)) vel.x += 1.f;
    if (IsKeyDown(KEY_W)) vel.y -= 1.f;
    if (IsKeyDown(KEY_S)) vel.y += 1.f;

    BaseCharacter::tick(dT);

    if (rightLeft > 0.f) {
        origin = Vector2{0.f, static_cast<float>(weapon.height) * scale};
        offset = Vector2{35.f, 55.f};
        weaponCollisionRec = Rectangle{
                getScreenPos().x + offset.x,
                getScreenPos().y + offset.y - static_cast<float>(weapon.height) * scale,
                static_cast<float>(weapon.width) * scale,
                static_cast<float>(weapon.height) * scale
        };
        rot = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? 35.f : -0.f;
    } else {
        origin = Vector2{static_cast<float>(weapon.width) * scale, static_cast<float>(weapon.height) * scale};
        offset = Vector2{25.f, 55.f};
        weaponCollisionRec = Rectangle{
                getScreenPos().x + offset.x - static_cast<float>(weapon.width) * scale,
                getScreenPos().y + offset.y - static_cast<float>(weapon.height) * scale,
                static_cast<float>(weapon.width) * scale,
                static_cast<float>(weapon.height) * scale
        };
        rot = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? -35.f : -0.f;
    }

    // Draw sword
    DrawTexturePro(
            weapon,
            Rectangle{
                    0.f,
                    0.f,
                    static_cast<float>(weapon.width) * rightLeft,
                    static_cast<float>(weapon.height)
            },
            Rectangle{
                    getScreenPos().x + offset.x,
                    getScreenPos().y + offset.y,
                    static_cast<float>(weapon.width) * scale,
                    static_cast<float>(weapon.height) * scale
            },
            origin,
            rot,
            WHITE
    );
}

void Character::takeDamage(float damage) {
    health -= damage;
    if (health <= 0.f) {
        setAlive(false);
    }
}


