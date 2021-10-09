# Super Paper Mario
This project is a clone of Super Paper Mario's Pit of 100 trials. It was made for a module called Programming 2. 
It was made using SDL2.

The game was featured on the website of [Digital Arts and Entertainment](https://www.digitalartsandentertainment.be/article/437/Programming+2%3A+2020+post+mortem). 
![Super Paper Mario](SuperPaperMario.gif)

The game consists of a couple levels in the pit of 100 trials. In each level the player must kill all enemies, one of which will drop a key. This key will open the door to the next level. After a couple levels, the player will find a merchant they can buy items from and unlock a new Pixl (a new tool).

## GameObjects
Most objects are derived from [GameObject](/src/GameObject.h). The inheritance tree looks like this:
* [Projectile](/src/Projectile.h)
  * [Bomb](/src/Bomb.h)
  * [CherbilProjectile](src/CherbilProjectile.h)
  * [Coconut](src/Coconut.h)
* [Sentient](/src/Sentient)
  * [Player](/src/Player.h)
  * [Dimentio](/src/Dimentio.h)
  * [Enemy](/src/Enemy.h)
    * [Cherbil](/src/Cherbil.h)
    * [Goomba](/src/Goomba.h)
    * [Spiny](/src/Spiny.h)
    * [Squiglet](/src/Squiglet.h)

## Enemies
As can be seen above, there are a few types of enemies. Each of these enemies have their own abilities and immunities. After killing an enemy, it will drop coins which will automatically be picked up. The player can use these coins to buy items from the merchant once they finds him.
Enemy name | Ability | Can be jumped on
-----------|---------|-----------------
Cherbil | Shoots [CherbilProjectile](/src/CherbilProjectile.h) which will freeze the player | True
Goomba | No ability | True
Spiny | Has a spiked shell | False
Squiglet | Shoots a coconut which will hurt the player | True

Each of these enemies has a spritesheet for their walking animation:

![Goomba walking animation](/Resources/Enemies/Goomba.png)

and their death animation:

![Goomba death animation](/Resources/Enemies/Goomba_Death.png)

## Pixls
The player has access to [Pixls](/src/Pixl.h). Each of these Pixls grants the player 1 ability.
Pixl name | Ability
----------|--------
Cudge | Player slams hammer
Barry | Reflects projectiles and kills enemies
Boomer | Drops bombs

## Items
All enemies have a chance of dropping items. The player can use these items to make it easier to clear the levels.
Item name | Use
----------|----
Key | Opens the door to the next level
Red shroom shake | Restores 10 health
Power block | Has a chance to instantly kill each enemy
Ice Storm | Has a chance to freeze each enemy
