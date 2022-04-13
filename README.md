# BuildingEscape

Exploration game with interactive objects and escape conditions

Game flow:
Escape room by triggering the door open and making it stay open using AActors

Game Mechanics:
Object collison to trigger door open.
Linear Interpolation to change between start vale and end value for door rotation
Grab Component to grab and release
Input binding of RMB to detect and Grab objects
Detect object of particular class using Line Trace to pickup particular objects
Pickup and move objects using UPhysicsHandleComponent in Unreal Engine
Weigh actors of class and open door by Iterating through Valid Actors to check condition.


Developed with Unreal Engine 4
