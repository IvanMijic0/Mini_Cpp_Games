#include "raylib.h"
#include "raymath.h"

class Prop {
public:
    Prop(Vector2 pos, Texture2D tex);

    Texture2D getTexture() { return texture; }

    void render(Vector2 knightPos);

    Rectangle getCollisionRec(Vector2 knightPos);

private:
    Texture2D texture{};
    Vector2 worldPos{};
    float scale{4.f};
};