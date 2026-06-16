# Producing a Square Wave

Basically, what this code aims to achive is to create a square wave of sound. As the name implies, a square wave is a type of wave that litereally looks like a square, instead of those typical smooth sine waves.


<div align="center">
    <img src="/Images/square_wave.png" width="400" alt="An Image Of A Square Wave">
</div>

## How It Works: The Physics of the Speaker Cone

At the physical level, your sound card is constantly moving the speaker cone back and forth. The values in the `buffer` (the `short` integers) are essentially "position instructions" for that cone.

- **The Resting Point (`0`):** The cone is in its neutral, center position.
- **Max Displacement (`32767` to `-32768`):** The cone is pushed out to its limit or pulled back to its limit.

<br>

<div align="center">
    <img src="/Images/speaker_cone.png" width="500" alt="An Image Of A Square Wave">
</div>

By filling the buffer with a square wave (alternating values of max and min positions), we command the speaker to oscillate back and forth, creating a sound.

## Filling The Buffer
When filling the buffer, we need to place either a max value (32767), or, a min value (-32768). However, we cannot directly place the max value on even buffer indices and min value on the odd buffer indices.

Since we have a sampling rate of 44100 Samples per second, we essentially can update our cone to different positions/coordinates 44100 times in a second.

Now, lets say that we want a frequency of 50 Hz for our square wave. For this, we would want our cone to complete 50 oscillations (min to max to min) in a second. Since there are 44100 samples in a second, 50 oscillations would take place in those 44100 samples. This means that 1 oscillation should take place in 44100/50 = 882 samples.

Now, lets look at these 882 samples. Out of these 882 samples, the first half, that is, 441 samples (882/2), should have a max value, and the next half, that is, the next 441 samples should have a min value. The process would then repeat itself until the very last sample. All of this data processing to set the max and min value inside an array known as "buffer", would take place inside a loop.





## The Solution: Phase Accumulation
To make the siren sound professional, we use a **Phase Accumulator**. 
Instead of calculating the absolute position from time, we calculate a tiny "step" for every single sample. 

1. **Calculate Step:** We determine how much the wave should rotate based on the current frequency.
2. **Accumulate:** We add that tiny step to a running total (`runningPhase`).
3. **Move:** We use that running total to define the cone's position.

This ensures the speaker cone is always moved to the next logical position from where it just was, creating a perfectly continuous, fluid sound.

## Technical Details
* **Language:** C++
* **API:** Win32 `waveOut` API
* **Sample Rate:** 44,100 Hz (CD Quality)
* **Bit Depth:** 16-bit signed integers
* **Synthesis Method:** Phase-accumulated frequency modulation (FM)

## Compilation
Ensure you have the Windows Multimedia library linked:
```bash
# Using g++
g++ main.cpp -o siren.exe -lwinmm
