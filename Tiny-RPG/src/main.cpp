#include "raylib.h"
#include "../headers/character.h"
#include "../headers/enemy.h"
#include "../headers/prop.h"
#include <vector>
#include <string>

void unloadTextures(const std::vector<Texture2D> &textures);

int main() {
    // Window dimensions
    const int w_dim{384};

    InitWindow(w_dim, w_dim, "Tiny RPG");
    SetWindowIcon(Image{LoadImage("../assets/characters/weapon_sword.png")});
    SetTargetFPS(60);

    // Map variables
    Texture2D map{LoadTexture("../assets/nature_tileset/OpenWorldMap24x24.png")};
    const float mapScale{4.f};

    // Knight
    Character knight{
            w_dim,
            LoadTexture("../assets/characters/knight_idle_spritesheet.png"),
            LoadTexture("../assets/characters/knight_run_spritesheet.png")
    };

    // Goblin
    Enemy goblin{
            Vector2{800.f, 300.f},
            LoadTexture("../assets/characters/goblin_idle_spritesheet.png"),
            LoadTexture("../assets/characters/goblin_run_spritesheet.png"),
            3.5f
    };

    // Slime
    Enemy slime{
            Vector2{500.f, 700.f},
            LoadTexture("../assets/characters/slime_idle_spritesheet.png"),
            LoadTexture("../assets/characters/slime_run_spritesheet.png"),
            2.f
    };

    Enemy *enemies[]{
            &goblin,
            &slime
    };

    for (Enemy *enemy: enemies) {
        enemy->setTarget(&knight);
    }

    // Props
    Prop props[2]{
            Prop{Vector2{600.f, 300.f}, LoadTexture("../assets/nature_tileset/Rock.png")},
            Prop{Vector2{300.f, 500.f}, LoadTexture("../assets/nature_tileset/Log.png")}
    };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        // Draw the map
        DrawTextureEx(
                map,
                Vector2Negate(knight.getWorldPos()),
                0.f,
                mapScale,
                WHITE
        );

        // Draw props
        for (Prop prop: props) {
            prop.render(knight.getWorldPos());
        }

        if (!knight.getAlive()) {
            DrawText("Game Over!", 55.f, 45.f, 40, WHITE);
            EndDrawing();
            continue;
        } else {
            std::string knightsHealth = "Health: ";
            knightsHealth.append(std::to_string(knight.getHealth()), 0, 5);
            DrawText(knightsHealth.c_str(), 55.f, 45.f, 40, WHITE);
        }

        knight.tick(GetFrameTime());

        // Check map bounds
        if (
                knight.getWorldPos().x < 0.f ||
                knight.getWorldPos().y < 0.f ||
                knight.getWorldPos().x + w_dim > static_cast<float>(map.width) * mapScale ||
                knight.getWorldPos().y + w_dim > static_cast<float>(map.height) * mapScale
                ) {
            knight.undoMovement();
        }

        // Check prop collisions
        for (Prop prop: props) {
            if (CheckCollisionRecs(
                    knight.getCollisionRec(),
                    prop.getCollisionRec(knight.getWorldPos())
            )) {
                knight.undoMovement();
            }
        }

        // Enemy
        for (Enemy *enemy: enemies) {
            enemy->tick(GetFrameTime());
        }

        // Weapon Collision
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (Enemy *enemy: enemies) {
                if (CheckCollisionRecs(
                        enemy->getCollisionRec(),
                        knight.getWeaponCollisionRec()
                )) {
                    enemy->setAlive(false);
                }
            }
        }

        EndDrawing();
    }
    unloadTextures(std::vector<Texture2D>{
            map,
            knight.getTexture(),
            knight.getIdleTexture(),
            knight.getRunTexture(),
            knight.getWeaponTexture(),
            goblin.getTexture(),
            goblin.getIdleTexture(),
            goblin.getRunTexture(),
            props[0].getTexture(),
            props[1].getTexture()
    });
    CloseWindow();
}

void unloadTextures(const std::vector<Texture2D> &textures) {
    for (const Texture2D &texture: textures) {
        UnloadTexture(texture);
    }
}
