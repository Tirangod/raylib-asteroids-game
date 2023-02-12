#include <raylib.h>
#include <rlgl.h>

int main(void) {
    InitWindow(800, 450, "raylib [core] example - basic window");

    Matrix m = rlGetMatrixTransform();
    rlRotatef(45, 0, 0, 0);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}