# Black Wolf

[![Join the chat at https://gitter.im/Kojirion/blackWolf](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/Kojirion/blackWolf)  
[![Build Status](https://drone.io/github.com/Kojirion/blackWolf/status.png)](https://drone.io/github.com/Kojirion/blackWolf/latest)

Black Wolf is a chess graphical interface, currently designed to play at the [Free Internet Chess Server](http://www.freechess.org).

## Building

Black Wolf depends on the following libraries:

- [SFGUI](https://github.com/TankOs/SFGUI)
- [SFML](https://github.com/SFML/SFML)
- [Thor](https://github.com/Bromeon/Thor)
- [Boost 1.55](http://www.boost.org)

After dependencies have been obtained, Black Wolf may be built by:

```
mkdir build
cd build
cmake ..
make
```

## Playing on FICS

The UI currently only offers basic functionality; in order to effectively seek games, it is necessary to issue commands at the console. For a complete reference, see [FICS help files](http://www.freechess.org/Help/AllFiles.html).  
Some commands to get going:

- ```seek 3 0``` - will post an an for a game with 3 minutes for each player, 0 seconds increment per move
- ```getgame ``` - will pick the first available seek ad. This is the fastest way to get a game.
- ```set seek false``` - this will hide seek ads, which can get overwhelming
- ```set chanoff true``` - this will hide discussions on the various channels

## Credit

Board and piece art by [Peter Wong](http://www.virtualpieces.net).


