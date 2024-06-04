#include <iostream>
#include <raylib.h>

using namespace std;

Color Green = Color{ 38, 185, 154, 255 };
Color Dark_Green = Color{ 20, 160, 133, 255 };
Color Light_Green = Color{ 129, 204, 184, 255 };
Color Yellow = Color{ 243, 213, 91, 255 };

int player_score = 0;
int cpu_score = 0;


class Paddle {
protected:

	void LimitMovement() {
		if (y <= 0) {
			y = 0;
		}
		if (y + height >= GetScreenHeight()) {
			y = GetScreenHeight() - height;
		}
	}

public:
	float x, y;
	float width, height;
	int speed;

	void Draw() {
		DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, WHITE);
	}

	void Update() {
		if (IsKeyDown(KEY_UP)) {
			y = y - speed;
		}
		if (IsKeyDown(KEY_DOWN)) {
			y = y + speed;
		}

		LimitMovement();
	}

	void IncreasePaddleSpeed() {
		speed += 1;
	}
};

class CpuPaddle : public Paddle {
public:
	void Update(int ball_y) {
		if (y + height / 2 > ball_y) {
			y = y - speed;
		}
		if (y + height / 2 <= ball_y) {
			y = y + speed;
		}
		LimitMovement();
	}
};


class Ball {
public:
	float x, y;
	int speed_x, speed_y;
	int radius;

	void Draw() {
		DrawCircle(x, y, radius, Yellow);
	}

	void Update(Paddle& player, CpuPaddle& cpu) {
		x += speed_x;
		y += speed_y;

		if (y + radius >= GetScreenHeight() || y - radius <= 0) {
			speed_y *= -1;
		}

		if (x + radius >= GetScreenWidth()) { //cpu wins
			cpu_score++;
			IncreaseSpeed(player, cpu);
			ResetBall();
		}

		if (x - radius <= 0) {
			player_score++;
			IncreaseSpeed(player, cpu);
			ResetBall();
		}
	}

	void ResetBall() {
		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;

		int  speed_choices[2] = { -1,1 };
		speed_x *= speed_choices[GetRandomValue(0, 1)];
		speed_y *= speed_choices[GetRandomValue(0, 1)];
	}

	void IncreaseSpeed(Paddle& player, CpuPaddle& cpu) {
		if (speed_x > 0) {
			speed_x += 1;
		}
		else {
			speed_y -= 1;
		}

		if (speed_y > 0) {
			speed_y += 1;
		}
		else {
			speed_x -= 1;
		}

		player.IncreasePaddleSpeed();
		cpu.IncreasePaddleSpeed();
	}
};


Ball ball;
Paddle player;
CpuPaddle cpu;

int main() {

	cout << "Starting the game" << endl;
	const int screen_windth = 1200;
	const int screen_height = 800;

	InitWindow(screen_windth, screen_height, "My Pong Game!");

	SetTargetFPS(60);

	ball.radius = 20;
	ball.x = screen_windth / 2;
	ball.y = screen_height / 2;
	ball.speed_x = 7;
	ball.speed_y = 7;

	player.width = 25;
	player.height = 120;
	player.x = screen_windth - player.width - 10;
	player.y = screen_height / 2 - player.height / 2;
	player.speed = 6;

	cpu.width = 25;
	cpu.height = 120;
	cpu.x = 10;
	cpu.y = screen_height / 2 - cpu.height / 2;
	cpu.speed = 6;

	while (WindowShouldClose() == false) {
		BeginDrawing();
		//updating
		ball.Update(player, cpu);
		player.Update();
		cpu.Update(ball.y);

		//checking for collisions
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player.x, player.y, player.width, player.height })) {
			ball.speed_x *= -1;

		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height })) {
			ball.speed_x *= -1;
		}

		//drawing
		ClearBackground(Dark_Green);
		DrawRectangle(screen_windth / 2, 0, screen_windth / 2, screen_height, Green);
		DrawCircle(screen_windth / 2, screen_height / 2, 150, Light_Green);
		DrawLine(screen_windth / 2, 0, screen_windth / 2, screen_height, WHITE);
		ball.Draw();
		cpu.Draw();
		player.Draw();
		DrawText(TextFormat("%i", cpu_score), screen_windth / 4 - 20, 20, 80, WHITE);
		DrawText(TextFormat("%i", player_score), 3 * screen_windth / 4 - 20, 20, 80, WHITE);


		EndDrawing();
	}


	CloseWindow();

	return 0;
}