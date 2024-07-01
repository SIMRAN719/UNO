# Uno Game

## Overview
This project implements a simple console-based Uno card game in C++. It allows multiple players to play Uno with basic rules and special card effects.

## Features

- Players can draw cards from a shuffled deck.
- Players can play cards that match either in color or value with the current top card.
- Special Uno card effects are implemented, including Draw Two, Reverse, Skip, Wild, and Wild Draw Four.
- Players can choose a color when playing a Wild card.
- Players can skip their turn using the Skip (00) card.

## Game Rules

- Players start with 7 cards each from a shuffled deck.
- The first player must play a non-special card (not Wild, Wild Draw Four, Reverse, or Skip) to start the game.
- Players take turns clockwise unless affected by a Reverse card.
- Special card effects:
  - **Draw Two**: Next player draws two cards and forfeits their turn.
  - **Reverse**: Changes the direction of play.
  - **Skip**: Skips the next player's turn.
  - **Wild**: Player chooses the next color.
  - **Wild Draw Four**: Next player draws four cards, player chooses the next color.

## Getting Started

### Prerequisites

- C++ compiler (supporting C++11 or later)

### Installation and Running the Game

1. Clone the repository

2. Compile the code:

3. Run the executable:


## How to Play

- Enter the number of players and their names.
- Follow the prompts to draw cards and play your turn.
- Use numeric input to select cards to play or draw.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your improvements.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
