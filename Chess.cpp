#include "raylib.h"
#include "Chess.h"



int main() {

    InitWindow(screenWidth, screenHeight, "五子棋游戏");

    SetTargetFPS(60);


    while(!WindowShouldClose()){
        BeginDrawing();

        ClearBackground(boardColor);

        chess.Chief();

        EndDrawing();

    }


    CloseWindow();

    return 0;
}