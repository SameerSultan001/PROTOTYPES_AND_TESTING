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
A buffer is simply an array of short integers (16 bits).

When filling the buffer, we need to place either a max value (32767), or, a min value (-32768) at each array index. We shouldn't directly place max values on even buffer indices and min values on the odd buffer indices (basically, alternating between max and min between each buffer index) to simply create a square wave of any desired frequency. Doing so, however, would lead to the production of sound, with a square wave, of about 22500 Hz if we set our sampling rate to 44100 samples per second.

Since we have a sampling rate of 44100 samples per second, essentially, we can update our cone to different positions/coordinates 44100 times in a second.

Now, lets say that we desire a frequency of 50 Hz for our square wave. For this, we would want our cone to complete 50 oscillations (min to max to min) in a second. Since there are 44100 samples in a second, 50 oscillations would take place within those 44100 samples. This means that 1 oscillation should take place in 44100/50 = 882 samples.

Now, lets us look at these 882 samples only. Out of these 882 samples, the first half, that is, the first 441 samples (882/2), should contain a max value, and the next half, that is, the next 441 samples, should contain a min value. The process would then repeat itself until the very last sample, for all the samples inside the buffer array. This process of setting max and min short integer values into an array, would take place inside a loop.





## Technical Details
* **Language:** C++
* **API:** Win32 `waveOut` API
* **Sample Rate:** 44,100 Hz (CD Quality)
* **Bit Depth:** 16-bit signed integers

## Compilation
Ensure you have the Windows Multimedia library linked:
```bash
# Using g++
g++ Square_Wave.cpp -o Square_Wave.exe -lwinmm
