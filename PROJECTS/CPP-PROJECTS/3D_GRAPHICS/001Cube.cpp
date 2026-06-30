#include <iostream>
#include "raylib.h"


using namespace std;

// Compilation: g++ Cube.cpp -o Cube $(pkg-config --cflags --libs raylib)


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





class Cube
{
	
private:
	Vector3 positions[8];			// First 4 represents the front of the cube, starting top left, moving clockwise. Last 4 represent the back of the cube, starting top left, moving clockwise.
	Color color;
	float scaling;
	float length;					// In the x direction
	float width;					// In the z direction
	float height;					// In the y direction
	
	Vector2 positions2D[8];
	
	
	// ===========================================================================================
	// Helper Functions:
	
	void SetCube(Vector3 topLeft)
	{
		positions[0] = topLeft;
		
		Vector3 temp;		
		temp = topLeft;
		
		temp.x += length;	
		positions[1] = temp;
		
		temp.y -= height;
		positions[2] = temp;
		
		temp.x -= length;
		positions[3] = temp;
		
		temp.z += width;
		positions[7] = temp;
		
		temp.y += height;
		positions[4] = temp;
		
		temp.x += length;
		positions[5] = temp;
		
		temp.y -= height;
		positions[6] = temp;
		
		
		for(int i = 0; i < 8; i++)
		{
			positions2D[i] = ProjectTo2D(positions[i], scaling);
		}
	}
	
	// ===========================================================================================
	// Constructor:
	
public:
	Cube(Vector3 topLeft, float length, float width, float height, Color color) : color(color) , scaling(320.0f), length(length), width(width), height(height)
	{
		SetCube(topLeft);

	}
	
	// ===========================================================================================
	// Member Functions:
	
	void UpdateCubeZ(Vector3 newTopLeft, float newLength, float newWidth, float newHeight)
	{
		length = newLength;
		width = newWidth;
		height = newHeight;
		
		SetCube(newTopLeft);
	}
	
	void UpdateCubeZ(Vector3 newTopLeft)
	{
		SetCube(newTopLeft);
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



int main()
{


    InitWindow(screenWidth, screenHeight, "3D Cube");
    
    SetTargetFPS(FPS);
    
    Vector3 topLeftPos = {320.0f, -320.0f, 1000.0f}; // Refers to the top left of the front face of the cube
    
    Cube Cube1(topLeftPos, 320, 320, 320, WHITE);
    
    Vector2 screenOffSet = {0.0f, 0.0f};
    
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

		if(IsKeyDown(KEY_D))
		{
			topLeftPos.x--;
		}

		if(IsKeyDown(KEY_A))
		{
			topLeftPos.x++;
		}		
		
		if(IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))
		{
			topLeftPos.y--;
		}	
		
		if(IsKeyDown(KEY_SPACE))
		{
			topLeftPos.y++;
		}		
		
		if(IsKeyDown(KEY_W))
		{
			topLeftPos.z--;
		}
		
		if(IsKeyDown(KEY_S))
		{
			topLeftPos.z++;
		}		
		
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

			
		Cube1.UpdateCubeZ(topLeftPos);
		
		Cube1.Translate2DZ(screenOffSet);
		
		Cube1.DrawCubeZ();
		
        EndDrawing();
    }

    CloseWindow();

    return 0;	
	
		
}





