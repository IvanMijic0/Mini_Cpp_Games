#include "raylib.h"
#include <vector>

struct AnimData {
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(const int *w_dim, const AnimData &data);

void updateAnimData(AnimData &data, float dT, int maxFrame);

void unLoadTextures(const std::vector<Texture2D> &textures);

void reset(const Texture2D &bg, float &bgX);

void DrawBackground(Texture2D &bg, float posX);

void HandleBackground(Texture2D &bg, Texture2D &mg, Texture2D &fg, float &bgX, float &mgX, float &fgX, float dT);

int main() {
    // Window dimensions
    const int w_dim[2] = {512, 380};

    InitWindow(w_dim[0], w_dim[1], "Dapper Dasher!");
    SetTargetFPS(60);

    bool coll{};

    // Scarfy variables
    Texture2D scarfy{LoadTexture("../textures/scarfy.png")};
    AnimData scarfyData{
            Rectangle{
                    0.0,
                    0.0,
                    float(scarfy.width) / 6,
                    float(scarfy.height)
            },
            Vector2{
                    float(w_dim[0] * .5 - scarfyData.rec.width * .5),
                    float(w_dim[1]) - scarfyData.rec.height
            },
            int{},
            float{1.0 / 12.0},
            float{}
    };
    // Acceleration due to gravity (pixel/sec)/sec
    const int grav{1'000};
    // Jump velocity (pixels/sec)
    const int jumpVel{-600};
    float vel{10};
    bool isInAir;

    // Nebula variables
    Texture2D nebula{LoadTexture("../textures/12_nebula_spritesheet.png")};
    const int nebSize{3};
    int nebPos{};
    AnimData nebulae[nebSize] = {};

    for (auto &neb: nebulae) {
        neb.rec.x = neb.rec.y = 0.0;
        neb.rec.width = float(nebula.width) / 8;
        neb.rec.height = float(nebula.height) / 8;
        neb.pos.x = float(w_dim[0] + nebPos);
        neb.pos.y = float(w_dim[1]) - nebulae[0].rec.height;
        neb.frame = 0;
        neb.updateTime = 0.0;
        neb.runningTime = 1.0 / 16.0;
        nebPos += 300;
    }
    float finishLine{nebulae[nebSize - 1].pos.x};

    // Nebula x velocity (pixels/sec)
    float nebVel{-200};

    // Background variables
    Texture2D bg{LoadTexture("../textures/far-buildings.png")};
    Texture2D mg{LoadTexture("../textures/back-buildings.png")};
    Texture2D fg{LoadTexture("../textures/foreground.png")};
    float bgX{}, mgX{}, fgX{};

    while (!WindowShouldClose()) {
        const float dT{GetFrameTime()};
        // Start Drawing
        BeginDrawing();
        ClearBackground(WHITE);

        HandleBackground(bg, mg, fg, bgX, mgX, fgX, dT);

        if (isOnGround(w_dim, scarfyData)) {
            // Rectangle is on the ground
            vel = 0;
            isInAir = false;
        } else {
            // Rectangle is in the air
            vel += grav * dT;
            isInAir = true;
        }
        // Check for jumping
        if (IsKeyPressed(KEY_SPACE) && !isInAir) {
            vel += jumpVel;
        }

        // Update nebula position
        for (auto &neb: nebulae) {
            neb.pos.x += nebVel * dT;
            neb.runningTime += dT;
        }
        // Update finish line
        finishLine += nebVel * dT;
        // Update Scarfy position
        scarfyData.pos.y += vel * dT;

        // Nebula animation
        for (auto &neb: nebulae) {
            updateAnimData(neb, dT, 7);
        }
        // Scarfy animation
        if (!isInAir) {
            updateAnimData(scarfyData, dT, 5);
        }

        // Collision
        for (AnimData &neb: nebulae) {
            float pad{50};

            if (CheckCollisionRecs(
                    Rectangle{
                            neb.pos.x + pad,
                            neb.pos.y + pad,
                            neb.rec.width - pad * 2,
                            neb.rec.height - pad * 2
                    },
                    Rectangle{
                            scarfyData.pos.x,
                            scarfyData.pos.y,
                            scarfyData.rec.width,
                            scarfyData.rec.height
                    }
            )) {
                coll = true;
            }
        }

        if (coll) {
            // Game over
            DrawText("Game Over!", int(w_dim[0] * .3), int(w_dim[1] * .5), 40, WHITE);
        } else if (scarfyData.pos.x >= finishLine) {
            // Game won
            DrawText("You Won!", int(w_dim[0] * .3), int(w_dim[1] * .5), 40, WHITE);
        } else {
            // Draw nebula
            for (auto &neb: nebulae) {
                DrawTextureRec(nebula, neb.rec, neb.pos, WHITE);
            }
            // Draw Scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        // Stop Drawing
        EndDrawing();
    }

    unLoadTextures(std::vector<Texture2D>{scarfy, nebula, bg, mg, fg}
    );

    CloseWindow();
}

void HandleBackground(Texture2D &bg, Texture2D &mg, Texture2D &fg, float &bgX, float &mgX, float &fgX, const float dT) {
    bgX -= 20 * dT;
    mgX -= 40 * dT;
    fgX -= 80 * dT;

    reset(bg, bgX);
    reset(mg, mgX);
    reset(fg, fgX);

    DrawBackground(bg, bgX);
    DrawBackground(mg, mgX);
    DrawBackground(fg, fgX);
}

void DrawBackground(Texture2D &bg, float posX) {
    DrawTextureEx(bg, Vector2{posX, 0.0}, 0.0, 2.0, WHITE);
    DrawTextureEx(bg, Vector2{posX + float(bg.width) * 2, 0.0}, 0.0, 2.0, WHITE);
}

void reset(const Texture2D &bg, float &bgX) {
    if (bgX <= float(-bg.width) * 2) {
        bgX = 0.0;
    }
}

void updateAnimData(AnimData &data, const float dT, int maxFrame) {
    data.runningTime += dT;
    if (data.runningTime >= data.updateTime) {
        data.runningTime = 0.0;
        data.frame++;
        // Update Scarfy animation frame
        data.rec.x = data.rec.width * float(data.frame);
        if (data.frame > maxFrame) {
            data.frame = 0;
        }
    }
}

bool isOnGround(const int *w_dim, const AnimData &data) {
    return data.pos.y >= float(w_dim[1]) - data.rec.height;
}

void unLoadTextures(const std::vector<Texture2D> &textures) {
    for (const Texture2D &texture: textures) {
        UnloadTexture(texture);
    }
}
