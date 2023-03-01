/**
 * Content:
 * 0. Includes
 * 1. Defining some basic constants
 * 2. Config declaration
 * 3. Utility misc funcs
 * 4. Utility drawing funcs
 * 5. Memory pool
*/

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define NEW(t) (t*)malloc(sizeof(t))
#define MAKE(t, n) (t*)calloc(n, sizeof(t))

#define VEC_DOWN    (Vector2){ 0, 1}
#define VEC_UP      (Vector2){ 0,-1}
#define VEC_RIGHT   (Vector2){ 1, 0}
#define VEC_LEFT    (Vector2){-1, 0}
#define VEC_ONE     (Vector2){ 1, 1}
#define VEC_ZERO    (Vector2){ 0, 0}

#define Vec2(x, y) (Vector2){x, y}

#define TAU (2 * PI)

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800

static float dt;
static int asteroids_number;

static char STRING_BUFFER[32];

typedef struct {
    int pixelScale;
    int asteroidsCorners;
    int asteroidsNumber;
    int bulletsNumber;
} Config;

static Config config;

char* Ftoa(float f) {
    sprintf(STRING_BUFFER, "%f", f);
    return STRING_BUFFER;
}

char* Itoa(int i) {
    itoa(i, STRING_BUFFER, 10);
    return STRING_BUFFER;
}

void DebugVector(Vector2 v) {
    printf("Vector2(%f, %f)\n", v.x, v.y);
}


Color GetRandomColor() {
    return (Color){
        GetRandomValue(0, 255),
        GetRandomValue(0, 255),
        GetRandomValue(0, 255),
        255
    };
}

void Line(Vector2 start, Vector2 end, Color color) {
    int x0 = start.x;
    int y0 = start.y;
    int x1 = end.x;
    int y1 = end.y;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    while (true) {
        DrawRectangle(
            x0 * config.pixelScale, 
            y0 * config.pixelScale, 
            config.pixelScale, 
            config.pixelScale, 
            color
        );
        if (x0 == x1 && y0 == y1) 
            break;
        int e2 = 2 * err;
        if (e2 > -dy) { 
            err -= dy; 
            x0 += sx; 
        }
        if (e2 < dx) { 
            err += dx; 
            y0 += sy; 
        }
    }
}

void DrawPoints(int count, Vector2 points[count], Color color) {
    for (int i = 0; i < count - 1; i++) {
        Line(points[i], points[i+1], color);
    }
    Line(points[count - 1], points[0], color);
}

void MovePoints(int count, Vector2 points[count], Vector2 moveVec) {
    for (int i = 0; i < count; i++) {
        points[i] = Vector2Add(points[i], moveVec);
    }
}

void RotatePointsAround(int count, Vector2 points[count], Vector2 center, float angle) {
    float r = 0;
    float a = TAU / count;
    for (int i = 0; i < count; i++) {
        angle += a;
        r = Vector2Distance(center, points[i]);
        if (a >= TAU)
            a -= TAU;
        points[i].x = center.x + r * cosf(angle);
        points[i].y = center.y + r * sinf(angle);
    }
}


typedef struct {
    bool active;
    float damage;
    float speed;
    Vector2 direction;
    Vector2 position;
} Bullet;

Bullet* NewBullet(float damage, float speed, Vector2 direction, Vector2 position) {
    Bullet *bullet = NEW(Bullet);
    bullet->active = true;
    bullet->damage = damage;
    bullet->speed = speed;
    bullet->direction = direction;
    bullet->position = position;
    return bullet;
}

void MoveBullet(Bullet *bullet) {
    Vector2 moveVec = Vector2Scale(bullet->direction, bullet->speed);
    bullet->position = Vector2Add(bullet->position, moveVec);
}

void DrawBullet(Bullet *bullet) {
    if (!bullet->active)
        return;

    Line(bullet->position, bullet->position, BLACK);
}

void ResetBullet(Bullet *bullet) {

}

Bullet** InitBullets() {
    Bullet **bullets = MAKE(Bullet*, config.asteroidsNumber);
    for (int i = 0; i < config.bulletsNumber; i++) {
        bullets[i] = NewBullet(1, 50 ...);
    }
    return asteroids;
}

typedef struct {
    //TODO
} Cannon;


#define PLAYER_HEALTH 100
#define RELOADING_TIME 0.1 //in seconds

typedef struct Player Player;

typedef void (*ControllerCallback)(Player*);

void PlayerMouseController(Player*);

static int scores;

struct Player {
    float hp;
    float cooldown;
    float moveSpeed;
    float angle;
    Vector2 position;
    ControllerCallback controllerCallback;
    Vector2 points[3];
};

Player *NewPlayer(int x, int y, float hp, float moveSpeed) {
    Player *player = NEW(Player);
    player->hp = PLAYER_HEALTH;
    player->cooldown = 0;
    player->moveSpeed = moveSpeed;
    player->angle = 0;
    player->position = Vec2(x, y);
    player->controllerCallback = PlayerMouseController;
    player->points[0] = Vec2(x, y - 4);
    player->points[1] = Vec2(x - 2, y + 2);
    player->points[2] = Vec2(x + 2, y + 2);
    return player;
}

void MovePlayer(Player *player, Vector2 moveDir) {
    Vector2 moveVec = Vector2Scale(Vector2Normalize(moveDir), player->moveSpeed * dt);
    MovePoints(3, player->points, moveVec);
    player->position = Vector2Add(player->position, moveVec);
}

void DrawPlayer(Player *player) {
    DrawPoints(3, player->points, BLACK);
}

void UpdatePlayer(Player *player) {
    player->controllerCallback(player);
}

void ResetPlayer(Player *player) {
    player->hp = PLAYER_HEALTH;
    player->angle = 0;
    player->cooldown = 0;
    player->position = Vec2(
        SCREEN_WIDTH / config.pixelScale / 2,
        SCREEN_HEIGHT / config.pixelScale / 2
    );
}

void PlayerShot(Player *player) {
    printf("shot\n");
}

void PlayerMouseController(Player *player) {
    float angle = atan2f(
        player->position.y * config.pixelScale - (float)GetMouseY(),
        player->position.x * config.pixelScale - (float)GetMouseX()
    );
    if (angle < 0)
        angle += TAU;

    RotatePointsAround(3, player->points, player->position, angle + PI/3);

    Vector2 moveVec;

    if (IsKeyDown(KEY_W))
        moveVec.y = -1;
    else if (IsKeyDown(KEY_S))
        moveVec.y = 1;
    else
        moveVec.y = 0;

    if (IsKeyDown(KEY_A))
        moveVec.x = -1;
    else if (IsKeyDown(KEY_D))
        moveVec.x = 1;
    else
        moveVec.x = 0;
    
    MovePlayer(player, Vector2Normalize(moveVec));

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (player->cooldown <= 0) {
            player->cooldown = RELOADING_TIME;
            PlayerShot(player);
        }
    }

    if (player->cooldown >= 0) {
        player->cooldown -= dt;
    }
}

void FirstPlayerKeyboardController(Player *player) {
    //...    
}

void SecondPlayerKeyboardController(Player *player) {
    //...
}



typedef struct {
    int id;
    float hp;
    bool alive;
    int radius;
    float angle;
    float angularSpeed;
    float moveSpeed;
    Color color;
    Vector2 moveDir;
    Vector2 center;
    float *radiuses;
    Vector2 *points;
} Asteroid;

Asteroid* NewAsteroid(int x, int y, float hp, int minRadius, int maxRadius, float angularSpeed, float moveSpeed) {
    Asteroid *asteroid = NEW(Asteroid);
    asteroid->id = asteroids_number;
    asteroid->hp = hp;
    asteroid->radius = maxRadius;
    asteroid->angle = 0;
    asteroid->angularSpeed = angularSpeed;
    asteroid->moveSpeed = moveSpeed;
    asteroid->color = BLACK;
    asteroid->moveDir = VEC_ONE;
    asteroid->center = Vec2(x, y);
    asteroid->radiuses = MAKE(float, config.asteroidsCorners);
    asteroid->points = MAKE(Vector2, config.asteroidsCorners);
    asteroids_number++;

    int r = 0;
    float a = 0;
    float f = 2 * PI / config.asteroidsCorners;
    for (int i = 0; i < config.asteroidsCorners; i++) {
        a += f;
        r = GetRandomValue(minRadius, maxRadius);
        asteroid->radiuses[i] = r;
        asteroid->points[i].x = x + r * cosf(a);
        asteroid->points[i].y = y + r * sinf(a);
    }
    return asteroid;
}

void DrawAsteroid(Asteroid *asteroid) {
    if (asteroid->hp <= 0)
        return;

    DrawText(
        Itoa(asteroid->id),
        asteroid->center.x * config.pixelScale - 10, 
        asteroid->center.y * config.pixelScale - 10, 
        24, 
        BLACK
    );

    DrawPoints(config.asteroidsCorners, asteroid->points, asteroid->color);
    //Debug
    DrawCircleLines(
        asteroid->center.x * config.pixelScale, 
        asteroid->center.y * config.pixelScale, 
        asteroid->radius * config.pixelScale - 10, 
        RED
    );
}

void MoveAsteroid(Asteroid *asteroid) {
    Vector2 moveVec = Vector2Scale(asteroid->moveDir, asteroid->moveSpeed * dt);
    asteroid->center = Vector2Add(asteroid->center, moveVec);
    MovePoints(config.asteroidsCorners, asteroid->points, moveVec);
}

void RotateAsteroid(Asteroid *asteroid) {
    asteroid->angle += asteroid->angularSpeed * dt;
    if (asteroid->angle >= TAU)
        asteroid->angle = 0;
    
    RotatePointsAround(config.asteroidsCorners, asteroid->points, asteroid->center, asteroid->angle);
} 

void UpdateAsteroid(Asteroid *asteroid) {
    if (!asteroid->alive)
        return;
    MoveAsteroid(asteroid);
    RotateAsteroid(asteroid);
}

Asteroid** InitAsteroids() {
    Asteroid **asteroids = MAKE(Asteroid*, config.asteroidsNumber);
    for (int i = 0; i < config.asteroidsNumber; i++) {
        asteroids[i] = NewAsteroid(
            GetRandomValue(0, SCREEN_WIDTH / config.pixelScale),
            GetRandomValue(0, SCREEN_HEIGHT / config.pixelScale),
            100, 3, 8, 2, 20
        );
    }
    return asteroids;
}

void DebugAsteroid(Asteroid *asteroid) {
    printf("=== Asteroid ===\n");
    printf("Id(%d)\n", asteroid->id);
    printf("Center(%f, %f)\n", asteroid->center.x, asteroid->center.y);
    printf("Radius(%d)\n", asteroid->radius);
    printf("Points:\n");
    for (int i = 0; i < config.asteroidsCorners; i++) {
        int x = asteroid->points[i].x;
        int y = asteroid->points[i].y;
        printf("   Point(%d, %d)=(%d, %d)\n", x, y, x * config.pixelScale, y * config.pixelScale);
    }
}

int main() {
    config.asteroidsCorners = 10;
    config.asteroidsNumber = 32;
    config.bulletsNumber = 128;
    config.pixelScale = 8;

    Asteroid **astros = InitAsteroids();
    Bullet **bullets = InitBullets();

    Player *player = NewPlayer(SCREEN_WIDTH / (2 * config.pixelScale), SCREEN_HEIGHT / (2 * config.pixelScale), 100, 50);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroids");

    Asteroid *current;
    while (!WindowShouldClose()) {
        dt = GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawPlayer(player);
        UpdatePlayer(player);
        
        for (int j = 0; j < config.asteroidsNumber; j++) {
            current = astros[j];

            DrawAsteroid(current);
            UpdateAsteroid(current);

            DrawText(Ftoa(player->cooldown), 200, 10, 22, GREEN);
        }
        
        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
