#include <cmath>
#include <iostream>
#include <raylib.h>
#include <thread>
#include <vector>

int CELL_SIZE = 4;
typedef std::vector<std::vector<bool>> Grid;

Grid newGrid, grid;

void draw() {
    for (int r = 0; r < (int)grid.size(); r++) {
        unsigned char red = 255 * ((float)r / grid.size());
        for (int c = 0; c < (int)grid[0].size(); c++) {
            if (!grid[r][c])
                continue;

            unsigned char g = 255 * ((float)c / grid.size());

            unsigned char b = 127;

            auto color = CLITERAL(Color){red, g, b, 255};
            DrawRectangle(c * CELL_SIZE, r * CELL_SIZE, CELL_SIZE, CELL_SIZE,
                          color);

            // DrawRectangleLines(c * CELL_SIZE, r * CELL_SIZE, CELL_SIZE,
            //                    CELL_SIZE, WHITE);
        }
    }
}

void flip(int r, int c, int nR, int nC) {
    newGrid[r][c] = false;
    newGrid[nR][nC] = true;
}

void fall(int r, int c) {
    if (r >= (int)grid.size() - 1)
        return;

    if (!grid[r + 1][c])
        flip(r, c, r + 1, c);
    else if (GetRandomValue(-10, 10) > 0) {
        if (c - 1 >= 0 && !grid[r + 1][c - 1])
            flip(r, c, r + 1, c - 1);
        else if (c + 1 < (int)grid[0].size() && !grid[r + 1][c + 1])
            flip(r, c, r + 1, c + 1);
    } else {
        if (c + 1 < (int)grid[0].size() && !grid[r + 1][c + 1])
            flip(r, c, r + 1, c + 1);
        else if (c - 1 >= 0 && !grid[r + 1][c - 1])
            flip(r, c, r + 1, c - 1);
    }
}

void update() {
    newGrid = grid;
    for (int r = 0; r < (int)grid.size(); r++) {
        for (int c = 0; c < (int)grid[0].size(); c++) {
            if (grid[r][c])
                fall(r, c);
        }
    }
    grid = newGrid;
}

void handleInput() {
    if (IsWindowResized()) {
        int rows = GetScreenHeight() / CELL_SIZE;
        int cols = GetScreenWidth() / CELL_SIZE;
        grid = Grid(rows, std::vector<bool>(cols, 0));
    }

    int burstRadius = 10;
    bool pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    if (GetGestureDetected() == GESTURE_DRAG ||
        GetGestureDetected() == GESTURE_HOLD) {
        int x = GetMouseX() / CELL_SIZE;
        int y = GetMouseY() / CELL_SIZE;
        for (int i = -burstRadius; i < burstRadius; i++) {
            if (y + i >= (int)grid.size() || y + i < 0)
                continue;
            for (int j = -burstRadius; j < burstRadius; j++) {
                if (x + j >= (int)grid[0].size() || x + j < 0 ||
                    grid[y + i][x + j])
                    continue;
                if ((i * i) + (j * j) < burstRadius * burstRadius)
                    grid[y + i][x + j] = true;
            }
        }
    }
}

int main() {
    SetConfigFlags(ConfigFlags::FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Sand Simulation");
    SetTargetFPS(60);
    SetRandomSeed(0xaabbccff);
    float r = 20;
    while (!WindowShouldClose()) {
        handleInput();
        BeginDrawing();
        ClearBackground(BLACK);
        draw();
        EndDrawing();
        update();
    }

    CloseWindow();
    return 0;
}
