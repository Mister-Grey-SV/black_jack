# Black Jack

Console implementation of the classic Blackjack game written in C++.

This project was created as my first large educational project while learning C++.  
The goal was to practice:

- structures
- enums and enum class
- dynamic memory
- arrays, vectors and std::array
- file operations
- splitting code into multiple translation units
- random number generation
- game state management

The project started as a small exercise and eventually grew into a complete Blackjack implementation with casino-style mechanics.

## Features

- 6-deck shoe
- Random shuffling
- Cut card and automatic reshuffle
- Multiple player boxes (1-7)
- Split
- Double
- Insurance
- Even Money
- Surrender
- Blackjack payout 3:2
- Persistent player balance between sessions
- Statistics tracking

## Rules

Current implementation uses rules similar to many real casinos:

- Dealer receives one visible card initially.
- Dealer draws until reaching 17 or more.
- Blackjack pays 3:2.
- Insurance is available when dealer shows an Ace.
- Split is limited to one split per hand.
- All boxes use the same bet size.

## Technologies

- C++17
- STL
    - std::vector
    - std::array
    - std::mt19937
- Visual Studio Community

## Project structure

 - main.cpp // application entry point
 - constants.h // enums, structures and constants
 - game.cpp // game logic
 - visual.cpp // console rendering
 - shuffling.cpp // deck and shoe generation
 - save.cpp // save/load player data
 - input.cpp // input validation
 - navigation.cpp // menus
