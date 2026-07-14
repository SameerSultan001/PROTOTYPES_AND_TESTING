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

const float pixelToMeter = 1.0f/6200.0f;	// Exact connection to the real screen size (in meters) with the number of pixels.


// Compilation: g++ Pendulum.cpp -o Pendulum $(pkg-config --cflags --libs raylib)


// Utility Functions ========================================================================================================

float ConvertPixelToMeter(float pixel)
{
	return pixel*pixelToMeter;
}

float ConvertMeterToPixel(float meter)
{
	return meter/pixelToMeter;
}


// InputManager Class ========================================================================================================

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
	
	// Getters:
	
    Vector2 GetMousePosition() const { return mousePosition; }

    Vector2 GetMouseDelta() const { return mouseDelta ; }
    
    float GetFrameTime() const { return dt; }

    bool MouseLeftPressed() const { return mouseLeftPressed; }

    bool MouseLeftDown() const { return mouseLeftDown; }

    bool MouseLeftReleased() const { return mouseLeftReleased; }
  
	bool KeyPressedSpace() const { return keyPressedSpace; }
	
};


// Circle Class ==============================================================================================================

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
	
	void SetColor(Color newColor) { color = newColor; }
	
	
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


// Collision Detection =======================================================================================================

bool DetectCircleCollisionWithMouse(const Circle& circle, Vector2 mousePosition)
{
	Vector2 center = circle.GetCenter();
	
	float radius = circle.GetRadius();
	
	return ((center.x - mousePosition.x)*(center.x - mousePosition.x)) + (center.y - mousePosition.y)*(center.y - mousePosition.y) <= (radius*radius);
}


// Pendulum Class ============================================================================================================

class Pendulum
{
	
private:
	Vector2 pivotCoordinate;			// the point where the string is attached to the ceiling.
	
	float bobRadius;
	Color bobColor;
	
	Circle circle;						// representing the pendulum bob.
	
	
	float angularAcceleration;			// how fast the angular velocity changes with respect to time.
	float angularVelocity;				// how fast the angle of the pendulum, with the vertical, changes with respect to time.
		
	float angle;						// of the string, with the vertical, in radians.
	float angleDisplaced;				// angle that the string makes after it has been dragged to a certain point (initial angle).
	
	float mass;							// in kg.
	
	float dampingCoeffecient; 			// how much resistance the pendulum faces in its motion.
	
	float potentialEnergy;
	float kineticEnergy;
	float totalEnergy;					// K.E + P.E
		
	float timeElapsed;					// total time since the pendulum has been released from initial position.
	
	
	
	float stringLength;					// this is the length of the string in term of pixels.
	float actualStringLength;			// this is in terms of meters.
	float stringThickness;				// this is in terms of pixels.
	Color stringColor;
	
	float scaling;						// to scale the pixel version of the string length
	
	Vector2 dragOffset;					// used when dragging the pendulum, since we won't always grab it from its center.
		
	bool dragging;						// to check if the user wants to drag the pendulum.
	bool pause;							// to stop the pendulum.
	bool displayStatus;					// to check if the user want to display information about the pendulum.
	
	Vector2 displayCoordinate;			// top left corner of the display information box.
	float displayFontSize;				// size of the font to be displayed (in pixels).
	Color displayFontColor;				// color of the text that would need to be displayed.
	
	
	
	void UpdateEnergy()
	{
		potentialEnergy = mass * gravity * actualStringLength * (1 - cos(angle));
		kineticEnergy = 0.5f * mass * (actualStringLength * angularVelocity) * (actualStringLength * angularVelocity);
		totalEnergy = potentialEnergy + kineticEnergy;		
	}
	
	void SimulatePhysics(float dt)
	{
		angularAcceleration = -(gravity / actualStringLength) * sin(angle) - (dampingCoeffecient * angularVelocity);		
		angularVelocity += angularAcceleration * dt;
		angle += angularVelocity * dt;
		timeElapsed += dt;		
	}
	
	void UpdateBobPosition()
	{
		Vector2 newCenter;
		
		newCenter.x = pivotCoordinate.x + (stringLength*sin(angle));
		newCenter.y = pivotCoordinate.y + (stringLength*cos(angle));
		
		circle.SetCenter(newCenter);		
	}
	
	void DisplayInfo() const
	{
		DrawText(("Initial Angle: " + to_string(angleDisplaced) + " rad.").c_str(), displayCoordinate.x, displayCoordinate.y, displayFontSize, displayFontColor);
		DrawText(("String Length: " + to_string(actualStringLength) + " m.").c_str(), displayCoordinate.x, displayCoordinate.y + (displayFontSize), displayFontSize, displayFontColor);
		DrawText(("Damping Coeffecient: " + to_string(dampingCoeffecient) + ".").c_str(), displayCoordinate.x, displayCoordinate.y + (2 * displayFontSize), displayFontSize, displayFontColor);
		DrawText(("Current Angle: " + to_string(angle) + " rad.").c_str(), displayCoordinate.x, displayCoordinate.y + (3 * displayFontSize), displayFontSize, displayFontColor);
		DrawText(("Potential Energy: " + to_string(potentialEnergy) + " J.").c_str(), displayCoordinate.x, displayCoordinate.y + (4 * displayFontSize), displayFontSize, displayFontColor);
		DrawText(("Kinetic Energy: " + to_string(kineticEnergy) + " J.").c_str(), displayCoordinate.x, displayCoordinate.y + (5 * displayFontSize), displayFontSize, displayFontColor);
		DrawText(("Total Energy: " + to_string(totalEnergy) + " J.").c_str(), displayCoordinate.x, displayCoordinate.y + (6 * displayFontSize), displayFontSize, displayFontColor);
	}
	
	
public:
	Pendulum(Vector2 pivotCoordinate, float stringLength) : bobRadius(25.0f), bobColor(RED), circle({WIDTH/2, HEIGHT/2}, bobRadius, bobColor), pivotCoordinate(pivotCoordinate)
	{
		angularAcceleration = 0.0f;
		angularVelocity = 0.0f;
			
		angle = 0.0f;
		angleDisplaced = 0.0f;
		
		dampingCoeffecient = 0.0f; 		
		
		potentialEnergy = 0.0f;
		kineticEnergy = 0.0f;
		totalEnergy = 0.0f;
					
		timeElapsed = 0.0f;
		
		scaling = 0.1f;		// so screen value represents the actual pendulum scaled down to 1/10 th of its origin value
		
		this->stringLength = stringLength * scaling;
		actualStringLength = ConvertPixelToMeter(this->stringLength) / scaling;
		stringThickness = 5.0f;
		stringColor = WHITE;
		
		mass = 1.0f;	
	
		dragging = false;
		pause = false;
		displayStatus = true;
		
		displayCoordinate = {0, HEIGHT/2};
		displayFontSize = 20.0f;
		displayFontColor = GREEN;
	}
	
	// Setters ===============================================================================================================
	
	void SetDisplayStatus(bool status)
	{
		displayStatus = status;
	}
	
	void SetDisplayInfo(Vector2 coordinate, float fontSize, Color color)
	{
		displayCoordinate = coordinate;
		displayFontSize = fontSize;
		displayFontColor = color;
	}
	
	void SetPivotCoordinate(Vector2 newPivotCoordinate)
	{
		pivotCoordinate = newPivotCoordinate;
	}
	
	void SetBobInfo(float newRadius, Color newColor)
	{
		bobRadius = newRadius;
		bobColor = newColor;
		circle.SetRadius(newRadius);
		circle.SetColor(newColor);
	}
	
	void SetMass(float newMass)
	{
		mass = newMass;
	}
	
	void SetDampingCoeffecient(float newDampingCoeffecient)
	{
		dampingCoeffecient = newDampingCoeffecient;
	}

	
	// Getters ===============================================================================================================
	
	Vector2 GetBobCenter() const
	{
		return circle.GetCenter();
	}
	
	
	// Base Functions ========================================================================================================
	
	// Input:
	void Input(InputManager& input)
	{
		if(input.KeyPressedSpace())
		{
			pause = !pause;
		}
		if(DetectCircleCollisionWithMouse(circle, input.GetMousePosition()) && input.MouseLeftPressed())
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
			actualStringLength = ConvertPixelToMeter(stringLength) / scaling;
			
			angle = angleDisplaced;
			angularVelocity = 0.0;
			
			if(displayStatus)
			{
				UpdateEnergy();
			}
		}				
	}
	
	
	// Update:
	void Update(float dt)
	{	
		if(dragging) { return; }	// donot update anything while dragging
		if(pause) { return; }		// donot update anything while paused
		
		SimulatePhysics(dt);		
		UpdateBobPosition();
		
		if(displayStatus)
		{
			UpdateEnergy();
		}		
	}
	
	
	// Draw:
	void Draw() const
	{
		DrawLineEx(pivotCoordinate, circle.GetCenter(), stringThickness, stringColor); 
		
		circle.Draw();		// the bob
		
		if(displayStatus)
		{
			DisplayInfo();
		}
	}
};


// main ======================================================================================================================

int main()
{
	InitWindow(WIDTH, HEIGHT, "Pendulum Simulation");
	SetTargetFPS(FPS);
    
    Vector2 pivotCoordinate = {WIDTH/2, 0};
    
    InputManager input;
    
    Pendulum p1(pivotCoordinate, ConvertMeterToPixel(0.1));	// 10 cm. Since scaling = 0.1, actual displayed size on screen is 1 cm.
    
    p1.SetDisplayInfo((Vector2){15.0f, HEIGHT*0.2}, 20.0f, GREEN);
    
	
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
