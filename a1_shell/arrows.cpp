#include <iostream>
#include <termios.h>
#define STDIN_FILENO 0
using namespace std;
int main()
{
// Black magic to prevent Linux from buffering keystrokes.
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

// Once the buffering is turned off, the rest is simple.
    cout << "Enter a character: ";
    char c,d,e;
    cin >> c;
    cin >> d;
    cin >> e;
    string r;
    cout<<"\b \b"<<"\b \b"<<"\b \b"<<"\b \b";
// Using 3 char type, Cause up down right left consist with 3 character
    if ((c==27)&&(d=91)) {
        if (e==65) { cout << "UP";}
        if (e==66) { cout << "DOWN";}
        if (e==67) { cout << "RIGHT";}
        if (e==68) { cout << "LEFT";}
        cout<<endl;
    } else {
      getline(cin, r);
      cout<<r<<endl;
    }
    return 0;
}