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



float ConvertPixelToMeter(float pixel)
{
	return pixel*pixelToMeter;
}

float ConvertMeterToPixel(float meter)
{
	return meter/pixelToMeter;
}




class InputManager
{
	
private:
    Vector2 mousePosition;
    Vector2 mouseDelta;
    
    float dt;

    bool mouseLeftPressed;
    bool mouseLeftDown;
    bool mouseLeftReleased;
    
    bool keyPressedSpace;

public:
	InputManager() {}
	
	
    void Update()
    {
    	mousePosition = ::GetMousePosition();
	
	    mouseDelta = ::GetMouseDelta();
	
		dt = ::GetFrameTime();
		
	    mouseLeftPressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
	
	    mouseLeftDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
	
	    mouseLeftReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
	    
	    keyPressedSpace = IsKeyPressed(KEY_SPACE);
	}

    Vector2 GetMousePosition() const { return mousePosition; }

    Vector2 GetMouseDelta() const { return mouseDelta ; }
    
    float GetFrameTime() const { return dt; }

    bool MouseLeftPressed() const { return mouseLeftPressed; }

    bool MouseLeftDown() const { return mouseLeftDown; }

    bool MouseLeftReleased() const { return mouseLeftReleased; }
  
	bool KeyPressedSpace() const { return keyPressedSpace; }
	
};



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




bool DetectCircleCollisionWithMouse(const Circle& circle, Vector2 mousePosition)
{
	Vector2 center = circle.GetCenter();
	
	float radius = circle.GetRadius();
	
	return ((center.x - mousePosition.x)*(center.x - mousePosition.x)) + (center.y - mousePosition.y)*(center.y - mousePosition.y) <= (radius*radius);
}



class Pendulum
{
	
private:
	Vector2 pivotCoordinate;					// the point where the string is attached to the ceiling

	float angularAcceleration = 0.0f;
	float angularVelocity = 0.0f;
		
	float angle = 0.0f;							// of the string, with the vertical, in radians.
	float angleDisplaced = 0.0f;				// angle that the string makes after it has been dragged to a certain point
	float angularFrequency = 0.0f;
	

	
	const float amplitude = M_PI/2;
	const float dampingCoeffecient = 0.0f; 		
	
	float initialEnergy = 0.0f;
	float potentialEnergy = 0.0f;
	float kineticEnergy = 0.0f;
	float totalEnergy = 0.0f;					// K.E + P.E
	
	
	float timeElapsed = 0.0f;
	
	float stringLength = HEIGHT/2;				// this is the length of the string in term of pixels
	float actualStringLength = HEIGHT/2;		// this is in terms of meters
	
	float mass = 1.0f;							// in kg
	Circle circle;								// representing the pendulum bob.
	
	Vector2 dragOffset;		
	bool dragging = false;						// to check if the user want to drag the pendulum
	bool pause = false;							// to stop the pendulum
	bool displayInfo = true;
public:
	Pendulum(Vector2 pivotCoordinate) : circle({WIDTH/2, HEIGHT/2}, 25.0f, RED), pivotCoordinate(pivotCoordinate)
	{
		
	}
	
	
	void Input(InputManager& input)
	{
		if(input.KeyPressedSpace())
		{
			pause = !pause;
		}
		if(DetectCircleCollisionWithMouse(circle, input.GetMousePosition()) && input.MouseLeftDown())
		{
			dragging = true;
			dragOffset = input.GetMousePosition() - circle.GetCenter();
		}
		if(input.MouseLeftReleased())
		{
			dragging = false;
		}
		if(dragging)
		{
			circle.SetCenter(input.GetMousePosition() - dragOffset);
			
			float dx = circle.GetCenter().x - pivotCoordinate.x;
			float dy = circle.GetCenter().y - pivotCoordinate.y;
			
			angleDisplaced = atan2(dx, dy);
			timeElapsed = 0.0f;
			stringLength = Vector2Distance(pivotCoordinate, circle.GetCenter());
			actualStringLength = ConvertPixelToMeter(stringLength);
		
			angle = angleDisplaced;
			angularVelocity = 0.0;
		}				
	}
	
	
	void Update(float dt)
	{	
		if(!dragging)
		{
			if(!pause)
			{	
				angularAcceleration = -(gravity / actualStringLength) * sin(angle) - dampingCoeffecient*angularVelocity;;
				
				angularVelocity += angularAcceleration * dt;
				
				angle += angularVelocity * dt;
				
				timeElapsed += dt;
				
				Vector2 newCenter;
				
				newCenter.x = pivotCoordinate.x + (stringLength*sin(angle));
				newCenter.y = pivotCoordinate.y + (stringLength*cos(angle));
				
				circle.SetCenter(newCenter);
				
				if(displayInfo)
				{
					potentialEnergy = mass * gravity * actualStringLength * (1 - cos(angle));
					kineticEnergy = 0.5f * mass * (actualStringLength * angularVelocity) * (actualStringLength * angularVelocity);
					totalEnergy = potentialEnergy + kineticEnergy;
				}
			}
		}
	}
	
	void Draw() const
	{
		DrawLineEx(pivotCoordinate, circle.GetCenter(), 5.0f, WHITE); 
		
		circle.Draw();		// the bob
		
		if(displayInfo)
		{
			DrawText(("Initial Angle: " + to_string(angleDisplaced) + " rad.").c_str(), 0, 350, 20, GREEN);
			DrawText(("Current Angle: " + to_string(angle) + " rad.").c_str(), 0, 370, 20, GREEN);
			DrawText(("String Length: " + to_string(actualStringLength) + " m.").c_str(), 0, 390, 20, GREEN);
			DrawText(("Damping Coeffecient: " + to_string(dampingCoeffecient) + " rad.").c_str(), 0, 410, 20, GREEN);
			DrawText(("Potential Energy: " + to_string(potentialEnergy) + " J.").c_str(), 0, 430, 20, GREEN);
			DrawText(("Kinetic Energy: " + to_string(kineticEnergy) + " J.").c_str(), 0, 450, 20, GREEN);
			DrawText(("Total Energy: " + to_string(totalEnergy) + " J.").c_str(), 0, 470, 20, GREEN);
		}
	}
};


int main()
{

	InitWindow(WIDTH, HEIGHT, "Pendulum Simulation");
	SetTargetFPS(FPS);
    
    Vector2 pivotCoordinate = {WIDTH/2, 0};
    
    InputManager input;
    
    Pendulum p1(pivotCoordinate);
    
	
	while(!WindowShouldClose())
	{
		input.Update();
    	
    	p1.Input(input);
    	
    	p1.Update(input.GetFrameTime());

    	BeginDrawing();
    	
			ClearBackground(BLACK);
	    	
	    	DrawText("Pendulum Simulation", 0, 0, 20, GREEN);
	 
	    	p1.Draw();
	
    	EndDrawing();
	}
    
    CloseWindow();  
    
    return 0;
}
