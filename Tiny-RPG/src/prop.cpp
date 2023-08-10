#include "../headers/prop.h"

Prop::Prop(Vector2 pos, Texture2D tex) :
        texture(tex),
        worldPos(pos) {}

void Prop::render(Vector2 knightPos) {
    DrawTextureEx(
            texture,
            Vector2Subtract(worldPos, knightPos),
            0.f,
            scale,
            WHITE
    );
}

Rectangle Prop::getCollisionRec(Vector2 knightPos) {
    Vector2 screenPos{Vector2Subtract(worldPos, knightPos)};

    return Rectangle{
            screenPos.x,
            screenPos.y,
            static_cast<float>(texture.width) * scale,
            static_cast<float>(texture.height) * scale
    };
}
