# Rubik

Another solver for the original 3x3 Rubik's cube

## Installation
```
# Make sure build dependencies are installed
pacman -S glew glfw

git clone https://github.com/usr99/Rubik
cd Rubik/
make
```

## Usage

`./rubik [scramble]`

The scramble argument is optional, it must be written according to the Singmaster notation (https://rubiks.fandom.com/wiki/Notation#Standard_3.C3.973.C3.973_notation)
Slice moves and cube rotations are not accepted.

## How the cube is solved ?
This is a poor implementation of Kociemba's Two-Phase Algorithm (http://kociemba.org/twophase.htm).
I did not implement any symmetry reduction, and the first solution found is returned.

## Solver efficiency
When the program is run for the first time it will generate a `./tables` folder of about 11MB thus this may take a few moments.
The next times it will only loads these tables so this is much faster and the solution (up to 30 moves) will be found within a second.

## Screenshot
![Capture](https://user-images.githubusercontent.com/61255495/166245991-1c72cbd0-db27-46f3-8d85-0dd677180b00.PNG)

