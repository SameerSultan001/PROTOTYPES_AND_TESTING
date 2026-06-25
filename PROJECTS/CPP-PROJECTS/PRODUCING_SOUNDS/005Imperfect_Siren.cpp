#include <windows.h>
#include <mmsystem.h>
#include <cmath>
#include <iostream>

#pragma comment(lib, "winmm.lib")
// While compiling, you need to link it via the flag: -lwinmm

using namespace std;




const int SAMPLE_RATE = 44100;
const int BITS        = 16;
const int CHANNELS    = 1;



//  ================== THIS PROGRAM CREATES A SINE WAVE IN THE FORM OF A SIREN, AND THEN PLAYS IT ===================


int main()
{
	double duration = 8; 						// eg. 5 seconds (Should be fixed to 2 decimal places only)
	double frequency = 1000; 					// in Hz (NOT USED)
	double volume = 1; 							// should be between 0 and 1 (0% and 100%).
	double period = SAMPLE_RATE/frequency; 		// period refers to the total number of samples needed to complete a single wave (NOT USED)
	double t = 0;								// representing time in seconds
	double w;									// represents the angular frequency
	double samplesPerWave;						// represents the number of samples for a sound wave with a particular period of a particular frequency, for a complete wave	
	const short amplitude = 32767;				// required to be used because of how "short" and "sample" work together
	int numSamples = SAMPLE_RATE * duration;	// Fixing the duration to be 2 decimal places only would fix the non-integer error.
	
	// "short" occupies 2 Bytes (16 bits). It is also signed, so range is from -32768 to +32767
	short* buffer = new short[numSamples];
	

	
	
// ========================== DATA FOR FREQUENCIES ARRAY =================================================
	
	double* frequencies = new double[numSamples];
	
	double baseFrequency = 1150;				// Hz
	double amplitudeFrequency = 650;			// The maximum UP and DOWN change in Frequencies (600 to 1000 Hz is the range of frequencies)
	double sirenDuration = 2;					// Refers to the time duration, in seconds, for the siren to complete one wave
	double samplesPerSiren = SAMPLE_RATE * sirenDuration;	// since the siren lasts for 2 seconds
	
	
// ========================== FILLING THE FREQUENCIES ARRAY =================================================
	
	
	// Basically, i would want the complete wave of a Siren to last for 2 seconds
		
	for(int i = 0; i < numSamples; i++)			// Farely confident that this clearly fills in the frequency array
	{
		frequencies[i] = baseFrequency + amplitudeFrequency * sin(2 * M_PI * i / samplesPerSiren);
	}
	
	 							
// ========================== FILLING THE BUFFER ARRAY ======================================================
	
	
	// now, we fill in our buffer with 16 bit integers, ranging from -32768 to +32767, according to whatever sin(wt) produces:	
	
		
	samplesPerWave = SAMPLE_RATE/(frequencies[0]); // Period of a wave in samples
	w = 2 * M_PI / samplesPerWave;	
	
	for(int i = 0; i < numSamples; i++)
	{
		
		buffer[i] = (short) amplitude * volume * sin(w * t);			// since angular frequency = w = 2(pi)f
		
		t++;
		
		if(t >= samplesPerWave)
		{
			if(i == numSamples - 1)
			{
				break;
			}
			
			samplesPerWave = SAMPLE_RATE/(frequencies[i + 1]);
			w = 2 * M_PI / samplesPerWave;	
			t = 1;
		}
	}
	
	
	
	
// ========================== PLAYING THE SOUND FROM THE BUFFER ARRAY ======================================================	
	
	
	// Playing the buffer: (Just do the following setup, and then play sound from the buffer.)
	
	WAVEFORMATEX wfx    = {};
    wfx.wFormatTag      = WAVE_FORMAT_PCM; // Raw uncompressed audio data
    wfx.nChannels       = CHANNELS; // 1 = Mono, 2 = Stereo
    wfx.nSamplesPerSec  = SAMPLE_RATE; // 44100 Hz
    wfx.wBitsPerSample  = BITS; // 16 bits per sample
    
    wfx.nBlockAlign     = (wfx.nChannels * wfx.wBitsPerSample) / 8; // basically, how many bytes is just a single sample (2 Bytes)
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign; // Bytes per second (44100 * 2 = 88200 Bytes per second = 86.1 KB/sec)




    HWAVEOUT hWaveOut;
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);




    WAVEHDR waveHdr        = {}; // a c structure
    waveHdr.lpData         = (LPSTR)buffer; // basically a kind of type casting, since, lpData is of the type "LPSTR".
    waveHdr.dwBufferLength = numSamples * sizeof(short); // total size of the audio, in bytes.



    waveOutPrepareHeader(hWaveOut, &waveHdr, sizeof(WAVEHDR)); // This locks the buffer into physical RAM, so automatic paging doesn't change stuff.
    waveOutWrite(hWaveOut, &waveHdr, sizeof(WAVEHDR)); // Launches the data to the audio driver. Does not wait for audio to complete inoder to move to the next line of code.


    while (!(waveHdr.dwFlags & WHDR_DONE)) Sleep(10); // acts as a guard, needed so that buffer isn't deleted before the whole audio file is read completely.
	// the above while loop guard prevents the code from moving forward until the whole 5 second audio isn't read completely.
	
	
		
    waveOutUnprepareHeader(hWaveOut, &waveHdr, sizeof(WAVEHDR)); // Unlocks the RAM.
    waveOutClose(hWaveOut); // Closes the connection to the sound card.
    
    
    
    
    delete[] buffer;
    
	return 0;
}
