#include <iostream>
#include "raylib.h"


using namespace std;

// Compilation: g++ test2.cpp -o test2 $(pkg-config --cflags --libs raylib)


#define FPS 60

const int screenWidth = 1280;
const int screenHeight = 720;



// To convert from 3D to 2D:
Vector2 ProjectTo2D(Vector3 position3D, float scaling)
{
	Vector2 temp2D;
	
	temp2D.x = (position3D.x*scaling/position3D.z) + screenWidth/2;
	temp2D.y = (position3D.y*scaling/position3D.z) + screenHeight/2;
	
	return temp2D;
}


Vector2 Translate2D(Vector2 position2D, Vector2 translationVector)
{
	Vector2 temp2D = position2D;
	
	temp2D.x += translationVector.x;
	temp2D.y += translationVector.y;
	
	return temp2D;
}


// =====================================================================================================
// Cube Class:


class Cube
{
	
private:
	Vector3 actualPositions[8];			// First 4 represents the front of the cube, starting top left, moving clockwise. Last 4 represent the back of the cube, starting top left, moving clockwise.
	Vector3 relativePositions[8];		// Relative to the camera
	Color color;
	
	float length;					// In the x direction
	float width;					// In the z direction
	float height;					// In the y direction
	
	float scaling;
	
	Vector2 positions2D[8];			// 3D coordinates projected in 2D
	
	
	// ===========================================================================================
	// Helper Functions:
	
	void SetActualPositions(Vector3 topLeft)
	{
		actualPositions[0] = topLeft;
		
		Vector3 temp;		
		temp = topLeft;
		
		temp.x += length;	
		actualPositions[1] = temp;
		
		temp.y -= height;
		actualPositions[2] = temp;
		
		temp.x -= length;
		actualPositions[3] = temp;
		
		temp.z += width;
		actualPositions[7] = temp;
		
		temp.y += height;
		actualPositions[4] = temp;
		
		temp.x += length;
		actualPositions[5] = temp;
		
		temp.y -= height;
		actualPositions[6] = temp;		
	}
	
	
	void SetRelativePositions(Vector3 cameraPosition)
	{
		for(int i = 0; i < 8; i++)
		{
			relativePositions[i].x = actualPositions[i].x - cameraPosition.x;
			relativePositions[i].y = actualPositions[i].y - cameraPosition.y;
			relativePositions[i].z = actualPositions[i].z - cameraPosition.z;
		}		
	}
	
	
	void SetProjectedPositions()
	{
		for(int i = 0; i < 8; i++)
		{
			positions2D[i] = ProjectTo2D(relativePositions[i], scaling);
		}		
	}
	
	
	// ===========================================================================================
	// Constructor:
	
public:
	Cube(Vector3 topLeft, float length, float width, float height, Color color) : color(color) , scaling(320.0f), length(length), width(width), height(height)
	{
		SetActualPositions(topLeft);

	}
	
	// ===========================================================================================
	// Member Functions:
	
	void UpdateCubeDimensionsZ(Vector3 newTopLeft, float newLength, float newWidth, float newHeight)
	{
		length = newLength;
		width = newWidth;
		height = newHeight;
		
		SetActualPositions(newTopLeft);
	}
	
	
	void UpdateCubeZ(Vector3 cameraPosition)
	{
		SetRelativePositions(cameraPosition);
		SetProjectedPositions();
	}
	
	
	void Translate2DZ(Vector2 translationVector)
	{
		for(int i = 0; i < 8; i++)
		{
			positions2D[i] = Translate2D(positions2D[i], translationVector);
		}
	}
	
	// =============================================================================================
	// Drawing:
	
	void DrawCubeZ()
	{
		// Rear Side
		DrawLine(positions2D[4].x, positions2D[4].y, positions2D[5].x, positions2D[5].y, color);
		DrawLine(positions2D[5].x, positions2D[5].y, positions2D[6].x, positions2D[6].y, color);
		DrawLine(positions2D[6].x, positions2D[6].y, positions2D[7].x, positions2D[7].y, color);
		DrawLine(positions2D[7].x, positions2D[7].y, positions2D[4].x, positions2D[4].y, color);
		
		// Joining Front And Rear
		DrawLine(positions2D[0].x, positions2D[0].y, positions2D[4].x, positions2D[4].y, color);
		DrawLine(positions2D[3].x, positions2D[3].y, positions2D[7].x, positions2D[7].y, color);
		DrawLine(positions2D[1].x, positions2D[1].y, positions2D[5].x, positions2D[5].y, color);
		DrawLine(positions2D[2].x, positions2D[2].y, positions2D[6].x, positions2D[6].y, color);
		
		
		// Front Side
		// DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color);                       
		
		DrawLine(positions2D[0].x, positions2D[0].y, positions2D[1].x, positions2D[1].y, RED);
		DrawLine(positions2D[1].x, positions2D[1].y, positions2D[2].x, positions2D[2].y, RED);
		DrawLine(positions2D[2].x, positions2D[2].y, positions2D[3].x, positions2D[3].y, RED);
		DrawLine(positions2D[3].x, positions2D[3].y, positions2D[0].x, positions2D[0].y, RED);
				
	}
};

// ===========================================================================================================
// Camera Class:


class CameraZ
{
	
private:
	Vector3 coordinates3D;
	
public:
	
	CameraZ(Vector3 coordinates3D) : coordinates3D(coordinates3D)
	{
		
	}
	
	
	Vector3 &GetPosition() const
	{
		return coordinates3D;
	}
	
	
	void Update()
	{
		if(IsKeyDown(KEY_D))
		{
			coordinates3D.x++;
		}

		if(IsKeyDown(KEY_A))
		{
			coordinates3D.x--;
		}	
		
		if(IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))
		{
			coordinates3D.y++;
		}	
		
		if(IsKeyDown(KEY_SPACE))
		{
			coordinates3D.y--;
		}		
		
		if(IsKeyDown(KEY_W))
		{
			coordinates3D.z++;
		}
		
		if(IsKeyDown(KEY_S))
		{
			coordinates3D.z--;
		}			
	}
	
};


// ===========================================================================================================

int main()
{

    InitWindow(screenWidth, screenHeight, "3D Cube");
    
    SetTargetFPS(FPS);
    
    Vector3 topLeftPos = {320.0f, -320.0f, 1000.0f}; // Refers to the top left of the front face of the cube
    
    Vector3 origin = {0.0f, 0.0f, 0.0f};
    
    CameraZ camera(origin);
    
    Cube Cube1(topLeftPos, 320, 320, 320, WHITE);
    
    Vector2 screenOffSet = {0.0f, 0.0f};
    
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);
		
		if(IsKeyDown(KEY_LEFT))
		{
			screenOffSet.x--;
		}
		
		if(IsKeyDown(KEY_RIGHT))
		{
			screenOffSet.x++;
		}
		
		if(IsKeyDown(KEY_UP))
		{
			screenOffSet.y--;
		}		
		
		if(IsKeyDown(KEY_DOWN))
		{
			screenOffSet.y++;
		}

		
		camera.Update();
		
		Cube1.UpdateCubeZ(camera.GetPosition());
		
		Cube1.Translate2DZ(screenOffSet);
		
		Cube1.DrawCubeZ();
		
        EndDrawing();
    }

    CloseWindow();

    return 0;	
	
		
}


