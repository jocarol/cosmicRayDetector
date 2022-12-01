# C Cosmic Ray Detector
## What does this code do
![image](https://i.imgur.com/S4QiYAV.gif)
This code detects cosmic bitflips in RAM. It allows the user to enter a percentage of the free RAM to allocate, then it allocates that much RAM and sets it to 0. It then iterates through the memory, checking each byte for a value of 1. If it finds one, it prints out the location of the flipped bit and sets it back to 0.

## What is a cosmic bit flip?
![Particle Cascade](https://www.scienceabc.com/wp-content/uploads/2021/11/ParticleCascade.jpg)
A cosmic bitflip is a phenomenon where a single bit of memory in a RAM (Random Access Memory) chip is flipped from 0 to 1, or vice versa, due to radiation. This occurs when high-energy particles, such as cosmic rays or gamma rays, interact with the RAM chip and cause an ionizing effect. This ionizing effect causes electron movement, which can cause a bit to flip.

![image](https://user-images.githubusercontent.com/24550725/205127735-5d4f8931-0a5f-4ad8-a155-1ead4afbda45.png)

Cosmic bitflips are relatively rare, but can still be problematic due to the fact that they can cause data corruption and system crashes. This is because RAM stores data as binary bits, and any change in the data stored in RAM can cause errors.

To prevent cosmic bitflips, RAM manufacturers use error-correcting codes (ECCs) to detect and correct errors. ECCs use redundant data that is written along with the original data, and are used to detect and correct any errors when the data is read back from the RAM. If a bitflip is detected, the ECC can correct it, thus preventing data corruption.

This project aim at detecting such cosmic events by scanning a zeroed memory range for 1s at low level. This is why this project is written in C.

## How to compile
Clone this repository and navigate to the directory.

```bash
git clone https://github.com/jocarol/cosmicRayDetector
cd cosmicRayDetector
```

To compile the code, run the following command:

```bash
make all
```

Then to run the code:

```bash
./scanner
```
