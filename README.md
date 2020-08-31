# Mastercube puzzle box

This is a game I'm building with the concept of a puzzle box that is randomized every time you turn it on. The saddest thing, to me, about a great puzzle box, is solving it. It immediately becomes an art piece until you somehow forget how to solve it again. 

## Puzzle description
* When you turn the device on it, it starts a game by randomly choose the 'goal' as a chain of sides in a certain order. 
* Every round starts with the cube requiring it's placed in a neutral position facing upwards (it will complain until you do)
* Every turn is played by rolling the cube onto a side that is still lit (chosen sides become unlit so you know what you've picked already)
* Once all sides are unlit the round is over, the cube will tell you how many places you've chosen correctly, pause for a moment and the game will start over or end depending on the mode you're playing in (mode dictates how many rounds you get to solve the cube.)

## Current developement progress:

| Progress        | Description           
| ------------- |:-------------:
| Done! | Build prototype |
| 85% | Write code for v1 |
| 20% | Build final hardware |
| 0% | Plan artistic casing |
| 0% | Build final game |

The prototype hardware at the moment is an esp8266, 18650 lithium ion battery + charger / boost circuit, 6 Neopixel LEDs, and a MPU9250 motion sensor. I would list out part numbers and links, but I'm probably going to move over to a simpler microcontroller and possibly a cheaper tilt sensor before finalizing everything. This current setup is just very convenient for testing.

## Current Features
* Randomized goal every game
* Side detection with safe gaurds to try and keep side selection consistent and reliable
* Intro animation, side selection animation, end round and end game animations via the Neopixel LEDs
* Hand held, battery operated and rechargeable

## Features I want figured out for V1.0
* Game selection via cube direction upon powering on
* Run on simpler, cheaper hardware
