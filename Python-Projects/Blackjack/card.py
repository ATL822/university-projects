class Card():
    suits = ['Club', 'Spade', 'Heart', 'Diamond']
    names = ['A', '2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K']

    def __init__(self, name, suit):
        self.name = name
        self.suit = suit
        match(suit):
            case 'Club':
                self.suit = '♣'
            case 'Spade':
                self.suit = '♠'
            case 'Heart':
                self.suit = '♡'
            case 'Diamond':
                self.suit = '♢'

        match(name):
            case '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' | '10':
                self.value = int(name)
            case 'J' | 'Q' | 'K':
                self.value = 10
            case 'A':
                self.value = 1


    def __add__(self, other_item):
        if type(other_item) is Card:
            return self.value + other_item.value
        elif type(other_item) is int:
            return self.value + other_item
        elif type(other_item) is str:
            return str(self) + other_item


    def __radd__(self, other_item):
        return (self.value + other_item)
    

    def __str__(self):
        return f'{self.suit}{self.name} '
    

if __name__ == '__main__':
    my_card = Card('7', 'Diamond')
    my_card_2 = Card('A', 'Spade')
    my_card_3 = Card('J', 'Club')
    print(my_card, my_card_2, my_card_3)

    card_sum = my_card + my_card_2 + my_card_3
    print(card_sum)
    