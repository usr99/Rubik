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

`./rubik`

The OpenGL version does not accept any argument, the cube is first loaded in the solved state.

## How the cube is solved ?
This is a poor implementation of Kociemba's Two-Phase Algorithm (http://kociemba.org/twophase.htm).
I did not implement any symmetry reduction, and the first solution found is returned.

## Solver efficiency
When the program is run for the first time it will generate a `./tables` folder of about 11MB thus this may take a few moments.
The next times it will only loads these tables so this is much faster and the solution (up to 30 moves) will be found within a second.

## Screenshot
![image](https://github.com/user-attachments/assets/581e3817-ca30-489f-944a-7ca43f2b0d18)
