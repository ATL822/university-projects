import card
import hand
import deck


def main():
    game_deck = deck.Deck()

    player_hand = hand.Hand('Player', 0)
    dealer_hand = hand.Hand('Dealer', 0)

    game_deck.shuffle()

    player_hand.add_cards(game_deck.draw_cards(2))
    dealer_hand.add_cards(game_deck.draw_cards(2))

    dealer_hand.long_print()
    print()

    player_turn = True
    while player_turn:
        player_hand.long_print()
        if player_hand.total() > 21:
            player_turn = False

        user_selection = input('[H]it or [S]tay?')
        print()
        match(user_selection):
            case 'H' | 'h':
                player_hand.add_cards(game_deck.draw_cards(1))
            case 'S' | 's':
                player_turn = False
            case _:
                print('Invaid command. Try again.')

        if player_hand.total() > 21:
            player_hand.long_print()
            print('Player Busts! Dealer wins.')
            return
    
    dealer_turn = True
    while dealer_turn:
        if dealer_hand.total() >= 17:
            print('Dealer Stays.')
            dealer_turn = False
        else:
            print('Dealer hits.')
            dealer_hand.add_cards(game_deck.draw_cards(1))
        print(dealer_hand)
        print()
    
        if dealer_hand.total() > 21:
            print('Dealer Busts! Player wins!')
            return
    dealer_hand.long_print()
    print()
    player_hand.long_print()

    if dealer_hand.total() > player_hand.total():
        print('Dealer got more points! Dealer wins.')
    elif dealer_hand.total() == player_hand.total():
        print('The scores are tied! Its a Push.')
    else:
        print('Player got more points! Player wins!')
    return


if __name__ == '__main__':
    main()