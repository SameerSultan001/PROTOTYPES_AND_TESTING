//This program aims to create a movable pendulum simulation
#include <iostream>
#include <cmath>
#include "raylib.h"
#include "raymath.h"

constexpr int WIDTH = 900;
constexpr int HEIGHT = 600;
constexpr int FPS = 120;


// Compilation: g++ Pendulum.cpp -o Pendulum $(pkg-config --cflags --libs raylib)

class Circle
{
	
private:
	float radius;
	Vector2 center;
	Color color;
	
public:	
	Circle(Vector2 center, float radius, Color color) : center(center), radius(radius), color(color) {}
	
	Vector2 GetCenter() const { return center; }
	
	float GetRadius() const { return radius; }
	
	void Draw() const
	{
		DrawCircle(center.x, center.y, radius, color);
	}
};




bool DetectCircleCollisionWithMouse(const Circle& circle)
{
	Vector2 mousePosition = GetMousePosition();
	Vector2 center = circle.GetCenter();
	
	float radius = circle.GetRadius();
	
	return ((center.x - mousePosition.x)*(center.x - mousePosition.x)) + (center.y - mousePosition.y)*(center.y - mousePosition.y) <= (radius*radius);
}


int main()
{

	InitWindow(WIDTH, HEIGHT, "Pendulum Simulation");
	SetTargetFPS(FPS);
    
    Vector2 circleCenter = {WIDTH/2, HEIGHT/2};
    
    Circle c1(circleCenter, 100.0f, RED);
    
	
	while(!WindowShouldClose())
	{
    	BeginDrawing();
    	
    	ClearBackground(BLACK);
    	
    	DrawText("Pendulum Simulation", 0, 0, 20, GREEN);
    	
    	c1.Draw();
    	
    	if(DetectCircleCollisionWithMouse(c1))
    	{
    		DrawText("Collision Detected", 0, 450, 20, GREEN);
		}
    	
    	EndDrawing();
	}
    
    CloseWindow();  
    
    return 0;
}
