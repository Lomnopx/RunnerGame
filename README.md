PART 1

During this project I tried to use as much c++ as possible. I ended up using some blueprint for some parts for example like showing the score value from the character c++ class and calling the save score function.
All main systems are made using c++ with some minor assistance from blueprint mainly to call c++ functions or portray values in a c++ class.

Features: 
* Spawning platforms over a pre-defined area
* Spawning enemies (The obstacles are my enterpretation of enemies)
* Increasing difficulty over time (By increasing character movespeed)
* Accumulated points which increase as time progresses and is saved in a high score list shown at startup (Points recieved are based on movement speed and also increase by time)
* 3 lives in each game
* Projectile to player collision (Not sure waht kind of projectiles are meant to be in, I used colision with player and obstacles to remove lives when colliding)
* Basic keyboard controls


PART 2

* When passing an obstacle there is a 20% chance that the next obstacle will not spawn
* An ai player also plays the game, they have a high chance of running into obstacles, player and ai share lives and score.

Examples of effective c++ use and consepts

* Utilizing the already existing character movement in unreal engine instead of wasting time making a simular system.
* Object-oriented design by making actors responsible for their behaviors.
* Descriptive variable and function names.
* Writing clear and concise code.
* Avoiding unnecessary complexity.

Bugs

* world generation is tied to the players distance from the last chunk. this means that a new platform and enemy spawn when you are at the end of thelast platform, the ai player is able to move outside this bounds and fall of the map
* The ai movement system doesn't utilize the same system as the player, thus making them keep the same speed very hard.
