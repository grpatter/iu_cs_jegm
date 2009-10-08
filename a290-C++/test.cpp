#include <iostream>
#include <cstring>
    using namespace std;

class inventory {
    char item[40];              // name of item
    int onhand;                 // number on hand
    double cost;                // cost of item
    public:
	inventory(char *i, int o, double c)
    {
	strcpy(item, i);
	onhand = o;
	cost = c;
    }
    friend ostream &operator<<(ostream &stream, inventory ob);
    friend istream &operator>>(istream &stream, inventory &ob);
};

ostream &operator<<(ostream &stream, inventory ob)
{
    stream << ob.item << ": " << ob.onhand;
    stream << " on hand at $" << ob.cost << '\n';

    return stream;
}

istream &operator>>(istream &stream, inventory &ob)
{
    cout << "Enter item name: ";
    stream >> ob.item;
    cout << "Enter number on hand: ";
    stream >> ob.onhand;
    cout << "Enter cost: ";
    stream >> ob.cost;

    return stream;
}

int main()
{
    inventory ob("hammer", 4, 12.55);

    cout << ob;

    cin >> ob;

    cout << ob;

    return 0;
}
