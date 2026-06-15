#include <windows.h>
#include <iostream>
#include <vector>
#include <fstream>
// Note: All of the above header files donot need to be downloaded. They are built in.

using namespace std;



int main()
{
	// Note: vectors are dynamically expanding arrays. Will expand and shrink as data is added or removed
	
	vector <double> frequencies; // in Hz
	vector <double> durations; // in milliseconds
	
	string InFileName;
	
	cout << "Enter File Name To Open (with extention): ";
	cin >> InFileName;	// eg. "beeps.txt"
	
	
	ifstream InFile(InFileName.c_str()); // to read from a file named "beeps.txt"
	
	
	if(!InFile)
	{
		cout << "Error. Could Not Open File." << endl;
		return 1;
	}
	
	
	double tempFrequency;
	double tempDuration;
	
	while(true)
	{	
		InFile >> tempFrequency; // in Hz
		InFile >> tempDuration; // get it in seconds. Convert to milliseconds later.
		
		
		frequencies.push_back(tempFrequency);
		durations.push_back(tempDuration*1000); // to convert seconds to milliseconds
		
		if(InFile.eof())
		{
			break;
		}		
	}
	
	
	int size = frequencies.size();
	
	cout << "Total Beeps: " << size << endl << endl;
	
	for(int i = 0; i < size; i++)
	{
		cout << i+1 << ") ";
		cout << "Playing " << frequencies[i] << " Hz for " << durations[i]/1000 << " seconds." << endl;
		Beep(frequencies[i], durations[i]);
	}

	
    return 0;
}
