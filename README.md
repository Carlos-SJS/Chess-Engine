# Chess Engine

This is a relatively simple chess engine, started as a project for advanced programming class. This engine is written in C++, and is based on some well known strategies in the field of chess programming.

## Technical details

This engine uses many diferent techniques in the field of chess programming, with the main ones being:

- **Bitboards:** Bitboards are a way to represent a chess board making use of a 64-bit integer (unsigned long long in c++), this lets us make use of the different binary operator that c++ provides for us, helping to massivley improve the performance, mainly of out generator function, whic is the one in charge of generating all the moves in a chess position.
- **Alpha-Beta prunning:** This is the technique used by the engine to search it's move, is basically a min-max algorithm, but using some techniques to be able to prune the search tree, and reduce considerably the search scope.
- **Opening book:** This engine, uses a openning book to be able to improve the speed to generate moves in the opening phase of the game, but more important, using an opening book, helps the engine to get a better position in the early game, aswell as playing diferent positions each time without repeating the same moves each time a position gets played in the early game. This opening book was constructed using all games played in 2021 by grandmasters (from the [FICS database](https://www.ficsgames.org)), and was parsed to be used as a map (using STD maps) to eficiently consult the available openings for a given position.

There are many improvements to be made here, aswell as many other techniques that could be implemented in the future to improve the performance of the engine.

The Game interface was creating using [SDL2](https://www.libsdl.org) library, and runs in a separate thread to the engine (using pthread library), to ensure the game window never get's lagged by the engine itself.

I cannot go into full detail about the implementation and technical details of the engine, since it is a quite large and somewhat complex program.

## Interface
The interface, as mentioned before, was created using SDL2, it is quite simple, but it gets the job done.

![Game Window](https://imgur.com/rHuYJ4r.gif)

To interact with the engine, and make a move, the user has to click on the pice to move, and all the possible legal moves for the selected pice will be higligted so the user can then chose one of thos moves by clicking on them, then the move will be performed and it will be the turn of the engine (The engine can take a bit of time to chose it's move).

Additionally a console window will appear when running the engine that will display some debug information aswell as the PNG notation for the current position on the board.

![Console Window](https://imgur.com/bfXaHrO.png)

## Video explanation
A video explanation of the program can be found [here](link.to.video) (spanish).
