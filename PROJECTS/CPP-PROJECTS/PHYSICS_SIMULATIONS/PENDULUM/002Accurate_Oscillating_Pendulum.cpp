//This program aims to create a movable pendulum simulation
#include <iostream>
#include <cmath>
#include <string>
#include "raylib.h"
#include "raymath.h"

using namespace std;


constexpr int WIDTH = 1920;
constexpr int HEIGHT = 980;
constexpr int FPS = 120;

const float gravity = 9.81;
const float sqrtGravity = 3.132;

const float pixelToMeter = 1.0f/6200.0f;
//const float pixelToMeter = 1.0f;

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
	
	void SetCenter(Vector2 newCenter) { center = newCenter; }
	
	void SetRadius(float newRadius) { radius = newRadius; }
	
	
	void Move(Vector2 delta)
	{
		center.x += delta.x;
		center.y += delta.y;
	}
	
	
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



class Pendulum
{
	
private:
	Vector2 pivotCoordinate;	// the point where the string is attached

	float angularAcceleration = 0.0;
	float angularVelocity = 0.0;
		
	float angle = 0.0;					// of the string, with the vertical, in radians.
	float angleDisplaced = 0.0f;		// angle that the string makes after it has been dragged to a certain point
	float angularFrequency;
	

	
	const float amplitude = M_PI/2;
	const float decayCoeffecient = 0.03; // 0.05
	
	float timeElapsed = 0.0f;
	float dt = 0.0;
	
	float stringLength = HEIGHT/2;			// this is in term of pixels
	float actualStringLength = HEIGHT/2;		// this is in terms of meters
	
	Circle circle;				// representing the pendulum bob.
	
	Vector2 dragOffset;		
	bool dragging = false;		// to check if the user want to drag the pendulum
public:
	Pendulum(Vector2 pivotCoordinate) : circle({WIDTH/2, HEIGHT/2}, 25.0f, RED), pivotCoordinate(pivotCoordinate)
	{
		
	}
	
	void Update()
	{		
		if(DetectCircleCollisionWithMouse(circle) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			dragging = true;
			dragOffset = GetMousePosition() - circle.GetCenter();
		}
		if(IsMouseButtonUp(MOUSE_LEFT_BUTTON))
		{
			dragging = false;
		}
		if(dragging)
		{
			circle.SetCenter(GetMousePosition() - dragOffset);
			
			float dx = circle.GetCenter().x - pivotCoordinate.x;
			float dy = circle.GetCenter().y - pivotCoordinate.y;
			
			angleDisplaced = atan2(dx, dy);
			timeElapsed = 0.0f;
			stringLength = Vector2Distance(pivotCoordinate, circle.GetCenter());
			actualStringLength = stringLength*pixelToMeter;
			
			//angularAcceleration = -(gravity / actualStringLength) * sin(angleDisplaced); //added this
			//angularFrequency = sqrt(gravity/actualStringLength);
			angle = angleDisplaced;
			angularVelocity = 0.0;
		}
		else
		{
			dt = GetFrameTime();	// added
			
			//angle = angleDisplaced * exp(-decayCoeffecient * timeElapsed) * cos(angularFrequency * timeElapsed);
			angularAcceleration = -(gravity / actualStringLength) * sin(angle) - decayCoeffecient*angularVelocity;;
			
			angularVelocity += angularAcceleration * dt;	// added
			
			angle += angularVelocity * dt;		// added
			
			//angularVelocity *= decayCoeffecient;
			
			
			
			timeElapsed += dt;
			
			Vector2 newCenter;
			
			newCenter.x = pivotCoordinate.x + (stringLength*sin(angle));
			newCenter.y = pivotCoordinate.y + (stringLength*cos(angle));
			
			circle.SetCenter(newCenter);
		}
	}
	
	void Draw() const
	{
		DrawLineEx(pivotCoordinate, circle.GetCenter(), 5.0f, WHITE); 
		
		circle.Draw();		// the bob
		
		
		DrawText(("Angle Displaced: " + to_string(angleDisplaced)).c_str(), 0, 350, 20, GREEN);
		DrawText(("String Length: " + to_string(actualStringLength)).c_str(), 0, 370, 20, GREEN);
	}
};


int main()
{

	InitWindow(WIDTH, HEIGHT, "Pendulum Simulation");
	SetTargetFPS(FPS);
    
    Vector2 pivotCoordinate = {WIDTH/2, 0};
    
    
    Pendulum p1(pivotCoordinate);
    
	
	while(!WindowShouldClose())
	{
    	BeginDrawing();
    	
    	ClearBackground(BLACK);
    	
    	DrawText("Pendulum Simulation", 0, 0, 20, GREEN);
    	
    	p1.Update();
    	
    	p1.Draw();

    	
    	EndDrawing();
	}
    
    CloseWindow();  
    
    return 0;
}
