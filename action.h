//
// Created by Weiju Wang on 7/9/25.
//

#ifndef ACTION_H
#define ACTION_H

/**
 * Actions that a player can take during the game.
 *
 * You can execute this action on a `struct cambio` using `cambio_do_action()`. Arguments for each action are specified
 * in the documentation for that action. Arguments generally supply information that is the player's choice or is a
 * hidden card.
 */
enum action {
    /**
     * End this turn. Then, if the game is not over, draw a card.
     *
     * arg0 is either be the card that was drawn if it is known, or it can be `UNKNOWN_CARD`.
     */
    DRAW,

    /**
     * Discard the card that was just drawn.
     *
     * arg0 is required only if `c->drawn_card == UNKNOWN_CARD`. It should be the discarded card.
     */
    DISCARD,

    /**
     * Trade the card just drawn with a card currently in the player's hand.
     *
     * arg0 is the index of the traded card.
     * arg1 is the discarded card.
     */
    SWAP,

    /**
     * Blind switch any two players' cards.
     *
     * This action switches player [arg0]'s [arg1]th card with player [arg2]'s [arg3]th card.
     */
    BLIND_SWITCH,

    /**
     * The player to move peeks one of their cards.
     *
     * arg0 is the peeked-at card if known, or `UNKNOWN_CARD`.
     * arg1 is the index of the peeked-at card.
     */
    PEEK_OWN,

    /**
     * Peek someone else's card.
     *
     * arg0 is the peeked-at card if known or UNKNOWN_CARD if not known.
     * arg1 is the index of the peeked-at card.
     * arg2 is the index of the player whose card was peeked at. It cannot be 0.
     */
    PEEK_OTHER,

    /**
     * The peek after discarding a black king. After this action the player can decide whether to BLIND_SWITCH.
     *
     * arg0 is the peeked-at card if known or UNKNOWN_CARD if not known.
     * arg1 is the index of the peeked-at card.
     * arg2 is the index of the player whose card was peeked at. It cannot be 0.
     */
    B_KING_PEEK,

    /**
     * Stick a card. For simplicity we assume the stick must be valid.
     *
     * TODO implement false sticking
     *
     * arg0 is the player who stuck the card.
     * arg1 is the index of the card that was stuck.
     * If `arg0 == c->turn`, arg3 is the original index of the card that the player who stuck the card is giving away.
     */
    STICK,

    /**
     * Give away a card after sticking another person's card.
     *
     * arg0 is the index of the card that was given away.
     */
    GIVE_AWAY,

    /**
     * End this turn and have the next player call "Cambio," which begins the last round of play.
     */
    CAMBIO,

    /**
     * The number of distinct actions. This does not account for arguments to the actions.
     */
    NUM_ACTIONS,
};

#endif //ACTION_H
