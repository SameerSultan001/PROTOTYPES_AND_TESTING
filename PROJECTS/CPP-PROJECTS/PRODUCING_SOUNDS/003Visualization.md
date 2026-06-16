# Producing a Square Wave

Basically, what this code aims to achive is to create a square wave of sound. As the name implies, a square wave is a type of wave that litereally looks like a square, instead of those typical smooth sine waves.

![An Image Of A Square Wave](Images/square_wave.png)

## How It Works: The Physics of the Speaker Cone

At the physical level, your sound card is constantly moving the speaker cone back and forth. The values in the `buffer` (the `short` integers) are essentially "position instructions" for that cone.

- **The Resting Point (`0`):** The cone is in its neutral, center position.
- **Max Displacement (`32767` to `-32768`):** The cone is pushed out to its limit or pulled back to its limit.



By filling the buffer with a sine wave, we command the speaker to oscillate back and forth smoothly, creating a pure tone. By changing the frequency over time, we create the siren effect.

## The Problem: "Phase Tearing"
When changing frequency dynamically, many developers make the mistake of using absolute time ($t$) in their sine calculation: `sin(w * t)`. 

This causes the speaker cone to "teleport" between positions because the math doesn't account for where the cone was in the previous sample. This results in harsh, chainsaw-like static noise.



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
