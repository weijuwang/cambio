# cambio

## Rules

There are many different variants of this game. Below are the rules I implemented in this project. It is worth reading them because there are many different variants with small differences that can affect strategy significantly.

### Goal

Each player is dealt 4 cards. With a few exceptions, players may not look at or rearrange their cards. At the end of the game, you win if the sum of the values of your cards is lower than everyone else's. Playing certain cards throughout the game allow you to peek at and trade cards so you can make informed decisions and try to get lower cards.

### Gameplay

#### Beginning
When you are dealt your four cards, you arrange them in a 2x2 square and are allowed to see the bottom two. This is your only opportunity to do so. (I have implemented this in the code as an array of 4 cards where the third and fourth cards are the known bottom cards.)

#### On your turn
Draw a card to start your turn. You have two options:
1) Discard the card you just drew.
2) Replace one of the cards you already had with the card you just drew, and then discard the replaced card.

If you chose the first option and the card you discarded was one of the ones below, you can choose to take the associated action. Taking the action is not required.

| Card          | Action                                                                                                       |
|---------------|--------------------------------------------------------------------------------------------------------------|
| 7 or 8        | Look at one of your own cards.                                                                               |
| 9 or 10       | Look at one card of another player.                                                                          |
| Jack or queen | Swap one of any player's card with one of any other player's card.                                           |
| Black king    | Look at one card of another player. If you wish, you can then swap that card with any one of your own cards. |

If the draw deck is empty after drawing, shuffle the discard pile and add the cards to the draw deck.

#### Ending the game
At the beginning of your turn, before you draw a card, you may say "Cambio" when you believe you will win the game. From this point on, none of your cards can be traded or rearranged -- whatever you have is set until the end of the game. After you have called Cambio, your turn is skipped and everyone else gets one more turn. Everyone then turns over their cards, and adds up the value of their cards. Whoever has the lowest total wins.

#### Sticking
If someone places a card in the discard pile, you may "stick" that card by taking one of your or anyone's cards that is the same rank and discarding it on top of the discarded card. It does not have to be your turn, but only one person can stick each discarded card. You must stick a discarded card before the next player draws their card. You are allowed to stick your own card.

If you stick by taking someone else's card, you can optionally give them any one of your cards.

If you try to stick with a card and it turns out it's not the same rank, you have to return that card to where it was originally, and you also have to draw another card as punishment. If this happens, anyone else -- including yourself -- can try to stick again with the same rules until a correct stick attempt is made.

After any stick or stick attempt is finished, the flow of play resumes as if the stick didn't happen.

### Card values
The point values of cards 2-10 are the same as their rank. Other cards' point values are specified below.

| Card                  | Value |
|-----------------------|-------|
| Ace                   | 1     |
| Jack/queen/black king | 10    |
| Red king              | -1    |
| Joker                 | 0     |

## REPL
The command-line interface uses extremely short commands designed to minimize the number of keystrokes. There are three types of commands, each of which are given a sub-section below.

### Game commands
These are the most important; they tell the CLI that something happened in-game. The game state will automatically update based on the information you tell the CLI.

Each command name is one character. If arguments are given, the first one begins immediately after the name of the command with no space required. Arguments are separated by spaces.

When an argument is a card value, use the following format:

| Card       | Format |
|------------|--------|
| Ace        | a      |
| 2          | 2      |
| 3          | 3      |
| ...        | ...    |
| Jack       | j      |
| Queen      | q      |
| Black king | bk     |
| Red king   | rk     |
| Joker      | 0      |

When an argument is a player, type the index of that player. You are always player 0; the person who goes after you is player 1, and the person after that player 2, etc.

When an argument is a specific card in someone's hand, type the index of that card. At the beginning of the game, a player's cards are numbered 0-3 -- the top left, top right, bottom left, and bottom right cards, respectively. This numbering of cards doesn't necessarily match how the cards are arranged in a real game and you must keep track of which indices correspond to which cards.

The commands are below. Arguments in brackets are required; arguments in parentheses are optional.

| Command name | Arguments                     | Description                                                                                                                |
|--------------|-------------------------------|----------------------------------------------------------------------------------------------------------------------------|
| `+`          | `(cardDrawn)`                 | Ends the current turn and then draws a card for the next player whose turn it is if the game is not over.                  |
| `-`          | `(cardDiscarded)`             | Discards the card that was just drawn. `cardDiscarded` is required if `cardDrawn` was not specified when drawing the card. |
| `x`           | `[position]` `[cardDiscarded]` |                                                                                                                            |
|              |                               |                                                                                                                            |
|              |                               |                                                                                                                            |
|              |                               |                                                                                                                            |
|              |                               |                                                                                                                            |
|              |                               |                                                                                                                            |
|              |                               |                                                                                                                            |


###

## Monte Carlo tree search
Because of the large branching factor of Cambio, I used an [open-loop Monte Carlo tree search](https://ai.stackexchange.com/questions/13867/mcts-for-non-deterministic-games-with-very-high-branching-factor-for-chance-node) with [UCT](https://en.wikipedia.org/wiki/Monte_Carlo_tree_search#Exploration_and_exploitation) to determine the best action at any point. Closed-loop (traditional) Monte Carlo tree search is not feasible because the branching factor for each turn is over 1000. For more about open-loop MCTS, read (this paper)[http://www.diego-perez.net/papers/OpenLoopGVG.pdf].

In closed-loop MCTS, you'd model any unknown information, such as a drawn card, as a "chance node." For instance, if there were 10 possible types of cards you could draw at a certain point, you would have 10 branches from that node in MCTS. Open-loop MCTS eliminates this. It only stores actions, not states. For example, sequence of events in which you 1) draw a 7, 2) discard it, 3) peek one of your own cards, and 4) see it is a jack, is the same branch as the sequence where you 1) draw an 8, 2) discard it, 3) peek one of your own cards, and 4) see it is a 2. This is because the sequence of actions -- draw, discard, peek -- is the same. During each simulation phase, we would simply supply the necessary stochastic information, like drawn cards, at random. Compared to closed-loop MCTS, this is much faster and uses less memory.