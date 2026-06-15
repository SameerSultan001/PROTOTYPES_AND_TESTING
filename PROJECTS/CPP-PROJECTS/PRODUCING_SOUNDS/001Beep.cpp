#include <windows.h> // Built-in (No Downloads Or Linking Required)
#include <iostream>

using namespace std;


int main()
{
	double frequency; // in Hz
	double duration; // in milliseconds
	
	
	cout << "Enter frequency of sound: ";
	cin >> frequency; // eg. 1000 Hz
  
	cout << "Enter duration of sound: ";
	cin >> duration; // eg. 500 milliseconds (0.5 seconds)
	
  Beep(frequency, duration); // 1000 Hz for 500 milliseconds

  return 0;
}
