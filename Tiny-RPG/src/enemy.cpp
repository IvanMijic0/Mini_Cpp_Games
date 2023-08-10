#include "../headers/enemy.h"

Enemy::Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture, float newSpeed) {
    worldPos = pos;
    texture = idle_texture;
    idle = idle_texture;
    run = run_texture;
    width = static_cast<float>(texture.width) / static_cast<float>(maxFrames);
    height = static_cast<float>(texture.height);
    speed = newSpeed;
}

Vector2 Enemy::getScreenPos() {
    return Vector2Subtract(worldPos, target->getWorldPos());
}

void Enemy::tick(float dT) {
    if (!getAlive()) { return; }
    vel = Vector2Subtract(target->getScreenPos(), getScreenPos());

    if (Vector2Length(vel) < radius) { vel = {}; }

    BaseCharacter::tick(dT);

    if (CheckCollisionRecs(
            target->getCollisionRec(),
            getCollisionRec()
    )) {
        target->takeDamage(damagePerSec * dT);
    }
}

void Enemy::setTarget(Character *knight) {
    target = knight;
}

