# CSCI6555_COMPUTER_ANIMATION_THIRD_LAB-Public

Physics-Based Motion Control System
CS6555 – Computer Animation, Fall 2025
Assignment 3
1. Project Description
This project implements a physics-based motion control system for rigid bodies using basic physical principles. A set of spherical objects move under the influence of gravity, collide with each other, and interact with the environment. The system demonstrates motion generation, collision detection, and collision response in real time.
The animation is rendered using OpenGL and GLUT, and the motion is updated continuously using a timer-based animation loop.

2. Physical Model
Objects
Each object is modeled as a rigid sphere.
Collision detection is implemented using a bounding sphere approximation.
Forces
Gravity is applied along the negative Y-axis.
Motion is updated using Euler integration.
Collisions
Ball–ball collisions:
Detected using distance between sphere centers.
Velocity is reflected along the collision normal.
Ball–environment collisions:
Floor collision reflects Y velocity.
Wall collisions reflect X or Z velocity.
A coefficient of restitution (energy loss) is used to simulate inelastic collisions.
Simplifications
All balls are assumed to have equal mass.
Rotational dynamics are not modeled, since spheres are rotationally symmetric.

3. Input
The system uses:
Initial positions for multiple balls
Initial velocities
Physical parameters:
Gravity
Time step
Coefficient of restitution
These values are defined directly in the source code.

4. Output
A real-time animated 3D scene showing:
Multiple balls falling under gravity
Balls bouncing on the floor
Balls colliding with each other
Balls colliding with walls
A recorded video of the animation is provided for viewing.

5. How to Compile and Run (macOS)
Requirements
macOS
Xcode Command Line Tools
OpenGL / GLUT (FreeGLUT or system GLUT)
Compile
g++ lab3.cpp -o lab3 -framework OpenGL -framework GLUT
Run
./lab3

6. Files Included
lab3.cpp – Source code for the physics-based motion control system
README.md – Project description and instructions
Video file (.mov or .mp4) – Recorded animation output

7. Notes
The animation uses a fixed camera and lighting setup.
The simulation runs continuously using a GLUT timer.
The project satisfies the core requirements of Assignment 3, including gravity, collision detection, collision response, and animation.

8. Possible Extensions
Future improvements could include:
More accurate collision response with mass differences
Rotational dynamics and angular velocity
User interaction with objects
More advanced integration methods
Articulated or deformable objects
