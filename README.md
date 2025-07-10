# cambio

## Rules

There are many different variants of this game. Below are the rules I implemented in this project.

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

#### Ending the game
At the beginning of your turn, before you draw a card, you may say "Cambio" when you believe you will win the game. After you have called Cambio, your turn is skipped and everyone else gets one more turn. Everyone then turns over their cards, and adds up the value of their cards. Whoever has the lowest total wins.

#### Sticking
If someone places a card in the discard pile, you may "stick" that card by taking one of your or anyone's cards that is the same rank and discarding it on top of the discarded card. It does not have to be your turn, but only one person can stick each discarded card. You must stick a discarded card before the next player draws their card. You are allowed to stick your own card.

If you stick by taking someone else's card, you can optionally give them any one of your cards.

If you try to stick with a card and it turns out it's not the same rank, you have to return that card to where it was originally, and you also have to draw another card as punishment.

After any stick or stick attempt is finished, the flow of play resumes as if the stick didn't happen.

### Card values
The point values of cards 2-10 are the same as their rank. Other cards' point values are specified below.

| Card                  | Value |
|-----------------------|-------|
| Ace                   | 1     |
| Jack/queen/black king | 10    |
| Red king              | -1    |
| Joker                 | 0     |

## Monte Carlo tree search
Because of the large branching factor of Cambio, I used an [open-loop Monte Carlo tree search](https://ai.stackexchange.com/questions/13867/mcts-for-non-deterministic-games-with-very-high-branching-factor-for-chance-node) with [UCT](https://en.wikipedia.org/wiki/Monte_Carlo_tree_search#Exploration_and_exploitation) to determine the best action at any point. Closed-loop (traditional) Monte Carlo tree search is not feasible because the branching factor for each turn is over 1000. For more about open-loop MCTS, read (this paper)[http://www.diego-perez.net/papers/OpenLoopGVG.pdf].

In closed-loop MCTS, you'd model any unknown information, such as a drawn card, as a "chance node." For instance, if there were 10 possible types of cards you could draw at a certain point, you would have 10 branches from that node in MCTS. Open-loop MCTS eliminates this. It only stores actions, not states. For example, sequence of events in which you 1) draw a 7, 2) discard it, 3) peek one of your own cards, and 4) see it is a jack, is the same branch as the sequence where you 1) draw an 8, 2) discard it, 3) peek one of your own cards, and 4) see it is a 2. This is because the sequence of actions -- draw, discard, peek -- is the same. During each simulation phase, we would simply supply the necessary stochastic information, like drawn cards, at random. Compared to closed-loop MCTS, this is much faster and uses less memory.