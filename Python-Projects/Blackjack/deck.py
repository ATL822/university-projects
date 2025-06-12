import card as c
import random

class Deck():
    def __init__(self):
        self.cards = []
        self.size = 0
        self.reset()
        return
    
    def reset(self):
        self.cards = []
        self.size = 0
        for suit in c.Card.suits:
            for name in c.Card.names:
                self.cards.append(c.Card(name, suit))
                self.size += 1
        return

    # NOTE: top is 0, bottom is end of list
    def add_to_top(self, card):
        if type(card) is not c.Card:
            print('error adding non-card item to deck')
            return

        if card in self.cards:
            print('cannot add dupliate card to deck')
        else:
            print('card added to deck')
            self.cards.insert(0, card)
            self.size += 1
        return

    # NOTE: top is 0, bottom is end of list
    def add_to_bottom(self, card):
        if type(card) is not c.Card:
            print('error adding non-card item to deck')
            return

        if card in self.cards:
            print('cannot add dupliate card to deck')
        else:
            print('card added to deck')
            self.cards.append(card)
            self.size += 1
        return

    def add_to_location(self, card, index):
        if type(card) is not c.Card:
            print('error adding non-card item to deck')
            return

        if card in self.cards:
            print('cannot add dupliate card to deck')
        else:
            print('card added to deck')
            self.cards.insert(index, card)
            self.size += 1
        return
    
    # NOTE: top is 0, bottom is end of list
    def discard_top(self):
        del self.cards[0]
        self.size -= 1
        return

    def draw_cards(self, number_of_cards):
        if self.size < number_of_cards:
            print(f'not enough cards in deck to draw {number_of_cards}')
            return

        cards = []
        for i in range(number_of_cards):
            cards.append(self.cards[0])
            self.discard_top()
        return cards

    def shuffle(self):
        random.shuffle(self.cards)
        return

    def __len__(self):
        return len(self.cards)

    def __getitem__(self, index):
        return self.cards[int(index)]


if __name__ == '__main__':
    # test __init__
    my_deck = Deck()
    
    # test __len__ and __getitem__
    cards_in_deck = ''
    for i, card in enumerate(my_deck):
        cards_in_deck = cards_in_deck + str(card)
    print(cards_in_deck)

    # discardtop 
    my_deck.discard_top()

    cards_in_deck = ''
    for i, card in enumerate(my_deck):
        cards_in_deck = cards_in_deck + str(card)
    print(cards_in_deck)
    
    # drawcards
    drawn_cards = my_deck.draw_cards(3)
    print(my_deck.size)

    cards_in_deck = ''
    for i, card in enumerate(my_deck):
        cards_in_deck = cards_in_deck + str(card)
    print(cards_in_deck)
    
    # addtop 
    my_deck.add_to_top(drawn_cards[0])
    my_deck.add_to_top(drawn_cards[0])

    cards_in_deck = ''
    for i, card in enumerate(my_deck):
        cards_in_deck = cards_in_deck + str(card)
    print(cards_in_deck)

    # addbottom 
    my_deck.add_to_bottom(drawn_cards[1])
    my_deck.add_to_bottom(drawn_cards[1])

    cards_in_deck = ''
    for i, card in enumerate(my_deck):
        cards_in_deck = cards_in_deck + str(card)
    print(cards_in_deck)

    # addlocation 
    my_deck.add_to_location(drawn_cards[2], 10)
    my_deck.add_to_location(drawn_cards[2], 10)

    cards_in_deck = ''
    for i, card in enumerate(my_deck):
        cards_in_deck = cards_in_deck + str(card)
    print(cards_in_deck)

    #shuffle
    my_deck.shuffle()

    cards_in_deck = ''
    for i, card in enumerate(my_deck):
        cards_in_deck = cards_in_deck + str(card)
    print(cards_in_deck)

    #shuffle
    my_deck.reset()

    cards_in_deck = ''
    for i, card in enumerate(my_deck):
        cards_in_deck = cards_in_deck + str(card)
    print(cards_in_deck)
    
