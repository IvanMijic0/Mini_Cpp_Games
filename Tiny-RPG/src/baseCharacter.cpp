#include "../headers/baseCharacter.h"

BaseCharacter::BaseCharacter() = default;

Vector2 BaseCharacter::getWorldPos() { return worldPos; }

Texture2D BaseCharacter::getTexture() { return texture; }

Texture2D BaseCharacter::getIdleTexture() { return idle; }

Texture2D BaseCharacter::getRunTexture() { return run; }

bool BaseCharacter::getAlive() const { return alive; }

void BaseCharacter::setAlive(bool isAlive) { alive = isAlive; }

void BaseCharacter::undoMovement() { worldPos = worldPosLastFrame; }

void BaseCharacter::tick(float dT) {
    worldPosLastFrame = worldPos;

    // Update animation frame
    runningTime += dT;
    if (runningTime >= updateTime) {
        frame++;
        runningTime = 0.f;
        if (frame > maxFrames) {
            frame = 0;
        }
    }

    if (Vector2Length(vel) != 0.f) {
        // Set worldPos = worldPos + direction
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(vel), speed));
        texture = run;
        rightLeft = vel.x < 0.f ? -1.f : 1.f;
    } else {
        texture = idle;
    }
    vel = {};

    // Draw the character
    DrawTexturePro(
            texture,
            Rectangle{
                    width * rightLeft * static_cast<float>(frame),
                    0.f,
                    width * rightLeft,
                    height
            },
            Rectangle{
                    getScreenPos().x,
                    getScreenPos().y,
                    width * scale,
                    height * scale
            },
            Vector2{},
            0.f,
            WHITE
    );
}

Rectangle BaseCharacter::getCollisionRec() {
    return Rectangle{
            getScreenPos().x,
            getScreenPos().y,
            width * scale,
            height * scale
    };
}