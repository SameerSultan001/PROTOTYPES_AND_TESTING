#include <windows.h>
#include <mmsystem.h>
#include <iostream>

#pragma comment(lib, "winmm.lib")
// While compiling, you need to link it via the flag: -lwinmm

const int SAMPLE_RATE = 44100;
const int BITS        = 16;
const int CHANNELS    = 1;


//  ================== THIS PROGRAM CREATES A SQUARE WAVE AND THEN PLAYS IS ===================
int main()
{
	int duration = 5; // 5 seconds (integer for now)
	int numSamples = SAMPLE_RATE * duration;
	
	// "short" occupies 2 Bytes (16 bits). It is also signed, so range is from -32768 to +32767
	short* buffer = new short[numSamples];
	
	
	
	// now, we fill in our buffer with 16 bit integers, ranging from -32768 to +32767:	
	// what we would do is, fill it alternatingly (since we are producing a square wave):
	
	int period = 44; // period = SAMPLE_RATE/Frequency. (Frequency is approx 1000 Hz. Exact for this scenario is: 1002.2727)
	
	for(int i = 0; i < numSamples; i++)
	{
		if((i%period) < (period/2)) // first 22 samples of the wave are "HIGH" (First half of the wave)
		{
			buffer[i] = 32767; // the max position of the speaker's cone/drum/whatever_the_moving_part_is_called
		}
		else // next 22 samples of the wave are "LOW" (Second half of the wave)
		{
			buffer[i] = -32768; // the min position of the speaker's cone/drum/whatever_the_moving_part_is_called
		}
	}
	
	
	
	
	// Playing the buffer: (Just do the following setup, and then play sound from the buffer.)
	
	WAVEFORMATEX wfx    = {};
    wfx.wFormatTag      = WAVE_FORMAT_PCM; // Raw uncompressed audio data
    wfx.nChannels       = CHANNELS; // 1 = Mono, 2 = Stereo
    wfx.nSamplesPerSec  = SAMPLE_RATE; // 44100 Hz
    wfx.wBitsPerSample  = BITS; // 16 bits per sample
    
    wfx.nBlockAlign     = (wfx.nChannels * wfx.wBitsPerSample) / 8; // basically, how many bytes is just a single sample
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign; // Bytes per second (44100 * 2 = 88200 Bytes per second)


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
