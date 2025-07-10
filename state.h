//
// Created by Weiju Wang on 7/9/25.
//

#ifndef STATE
#define STATE

/**
 * Transition between states, i.e. an action that a player can take.
 */
enum transition {
    /**
     * Used in contexts where an action is optional.
     */
    NO_ACTION,

    /**
     * Draw a card.
     */
    DRAW,

    /**
     * Discard the card that was just drawn.
     */
    DISCARD,

    /**
     * Trade the card with a card currently in the player's hand.
     */
    SWAP,

    /**
     * Blind switch any two players' cards.
     */
    BLIND_SWITCH,

    /**
     * Peek an unseen card.
     */
    PEEK,

    /**
     * Stick a card.
     */
    STICK,

    /**
     * Give away one of our cards after sticking another person's card.
     */
    GIVE_AWAY,

    /**
     * Call "Cambio," which begins the last round of play.
     */
    CAMBIO,

    /**
     * The number of distinct actions. This does not account for arguments to the actions.
     */
    NUM_ACTIONS,
};

#endif //STATE
