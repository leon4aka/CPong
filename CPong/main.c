#include "raylib.h"
#include <stdio.h>

/*  
    - 2022-02-27
    - Coded by leon4aka with C and Raylib
    - 171 lines of code (without these comments) 
*/

struct Ball
{
    float radius;
    Vector2 position;
    Vector2 speed;
};

typedef struct Ball Ball;

void InitBall(Ball* ballptr, float radius0, float positionX, float positionY, float speed)
{
    ballptr->radius = radius0;
    ballptr->position.x = positionX;
    ballptr->position.y = positionY;
    ballptr->speed.x = speed;
    ballptr->speed.y = speed;
}

void DrawBall(Ball ball)
{
    DrawCircle(ball.position.x, ball.position.y, ball.radius, RED);
}

void MoveBall(Ball* ballptr)
{
    ballptr->position.x += ballptr->speed.x * GetFrameTime();
    ballptr->position.y += ballptr->speed.y * GetFrameTime();
}

void InitPaddle(Rectangle* rectptr, int width, int height, int positionY, int positionX)
{
    Rectangle rec = {positionX, positionY, width, height};
    *rectptr = rec;
}

void DrawPaddle(Rectangle rect)
{
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, BLUE);
}

void MovePaddle(Rectangle* rect, char tORf, float speed)
{
    if (tORf == 't')
        rect->y -= speed * GetFrameTime();

    else if (tORf == 'f')
        rect->y += speed * GetFrameTime();
}

int main()
{
    const int W_WIDTH = 1200;
    const int W_HEIGHT = 800;

    const int W_WIDTH_HALF = W_WIDTH / 2;
    const int W_HEIGHT_HALF = W_HEIGHT / 2;

    const int PADDLES_Y = W_HEIGHT_HALF - 75;
    const int PADDLE1_X = 50;
    const int PADDLE2_X = W_WIDTH - 75;

    InitWindow(W_WIDTH, W_HEIGHT, "CPong ~by leon4aka");

    Ball ball;
    InitBall(&ball, 10.0f, W_WIDTH_HALF, W_HEIGHT_HALF, 250.0f);

    const float paddle_speed = 400.0f;
    Rectangle rectangle_1;
    Rectangle rectangle_2;
    InitPaddle(&rectangle_1, 25, 150, PADDLES_Y, PADDLE1_X);
    InitPaddle(&rectangle_2, 25, 150, PADDLES_Y, PADDLE2_X);

    int score[2] = {0, 0};
    char p1_score[2];
    char p2_score[2];
    bool started = false;

    while(!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ENTER))
            started = true;

        if (started)
        {
            // CALCULATIONS
            if (IsKeyDown(KEY_W))
                MovePaddle(&rectangle_1, 't', paddle_speed);
            else if (IsKeyDown(KEY_S))
                MovePaddle(&rectangle_1, 'f', paddle_speed);

            if (IsKeyDown(KEY_I))
                MovePaddle(&rectangle_2, 't', paddle_speed);
            else if (IsKeyDown(KEY_K))
                MovePaddle(&rectangle_2, 'f', paddle_speed);

            if (rectangle_1.y <= 0)
                rectangle_1.y = 0;

            else if (rectangle_1.y + 150 >= W_HEIGHT)
                rectangle_1.y = W_HEIGHT - 150;

            if (rectangle_2.y <= 0)
                rectangle_2.y = 0;

            else if (rectangle_2.y + 150 >= W_HEIGHT)
                rectangle_2.y = W_HEIGHT - 150;

            MoveBall(&ball);

            if (ball.position.y >= W_HEIGHT)
            {
                ball.position.y = W_HEIGHT;
                ball.speed.y *= -1;
            }

            else if (ball.position.y <= 0)
            {
                ball.position.y = 0;
                ball.speed.y *= -1;
            }

            else if (ball.position.x >= W_WIDTH)
            {
                ball.position.x = W_WIDTH;
                ball.speed.x *= -1;
                score[0]++;
            }

            else if (ball.position.x <= 0)
            {
                ball.position.x = 0;
                ball.speed.x *= -1;
                score[1]++;
            }

            if (CheckCollisionCircleRec(ball.position, ball.radius, rectangle_1))
                ball.speed.x *= -1;

            else if (CheckCollisionCircleRec(ball.position, ball.radius, rectangle_2))
                ball.speed.x *= -1;
        }

        sprintf(p1_score, "%d", score[0]);
        sprintf(p2_score, "%d", score[1]);

        // RENDERING
        BeginDrawing();
        ClearBackground(BLACK);

        if (score[0] >= 3 || score[1] >= 3)
            break;

        DrawText(p1_score, 400, 325, 150, GRAY);
        DrawText(p2_score, 725, 325, 150, GRAY);
        DrawFPS(0, 0);
        DrawLine(W_WIDTH_HALF, 0, W_WIDTH_HALF, W_HEIGHT, GRAY);

        DrawBall(ball);

        DrawPaddle(rectangle_1);
        DrawPaddle(rectangle_2);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
