import card as c

class Hand():
    def __init__(self, name, size=5):
        self.name = name
        self.cards = []
        self.size = size


    def add_one(self, card):
        if type(card) is c.Card:
            self.cards.append(card)
            self.size += 1
        else:
            print('error: attempting to add non-card to hand')
        return


    def add_cards(self, cards):
        if type(cards) is list:
            for card in cards:
                self.add_one(card)
        else:
            print('error: method \'add cards\' requires a list')
        return

    def remove_last(self):
        del self.cards[-1]
        self.size -= 1
        return


    def remove_one(self, location):
        del self.cards[int(location)]
        self.size -= 1
        return
    

    def total(self):
        return sum(self.cards)


    def clear(self):
        self.cards = []
        return


    def long_print(self, locations=False):
        card_message = ''
        card_locations = ''
        for i, card in enumerate(self.cards):
            card_message = card_message + str(card)
            card_locations = card_locations + f' {i} '
        
        if locations == True:
            message = f'{self.name}\'s Cards: {card_message}' + '\n' + f'Hand Location: {card_locations}' + f'\nTotal Value: {self.total()}' 
        else:
            message = f'{self.name}\'s Cards: {card_message}' + f'\nTotal Value: {self.total()}' 

        print(message)
        return 

    def __str__(self):
        message = ''
        for card in self.cards:
            message = message + str(card)
        return message

    def __len__(self):
        return len(self.cards)


    def __getitem__(self, index):
        return self.cards[int(index)]


if __name__ == '__main__':
    # test __init__
    my_hand = Hand('Player')
        
    # cards for testing
    my_card = c.Card('7', 'Diamond')
    my_card_2 = c.Card('A', 'Spade')
    my_card_3 = c.Card('J', 'Club')

    # test add_one
    my_hand.add_one(my_card)
    my_hand.add_one(my_card_2)
    my_hand.add_one(my_card_3)

    # test __len__ and __getitem__
    for card in my_hand:
        print(card)

    # test remove_last
    my_hand.remove_last()

    # test __str__ and total
    my_hand.long_print(True)
    my_hand.long_print()

    location = input('enter location:')
    # test remove_one
    my_hand.remove_one(location)
    print(my_hand)   

    # test clear
    my_hand.clear()
    print(my_hand)