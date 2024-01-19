#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private:
    struct ship
    {
        int m_length;
        int m_ID;
        char m_symbol;
        string m_name;
    };
    ship m_ships[5];
    int m_numShips;
    int m_rows;
    int m_cols;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    m_numShips = 0;
}

int GameImpl::rows() const
{
    return (m_rows);
}
int GameImpl::cols() const
{
    return (m_cols);
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    if (m_numShips >= 5)
    {
        return false;
    }
    ship a;
    a.m_ID = m_numShips;
    a.m_symbol = symbol;
    a.m_name = name;
    a.m_length = length;
    m_ships[m_numShips] = a;
    m_numShips++;
    return true;
}

int GameImpl::nShips() const
{
    return (m_numShips);
}

int GameImpl::shipLength(int shipId) const
{
    return (m_ships[shipId].m_length);
}

char GameImpl::shipSymbol(int shipId) const
{
    return (m_ships[shipId].m_symbol); 
}

string GameImpl::shipName(int shipId) const
{
    return (m_ships[shipId].m_name);
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    p1->placeShips(b1);
    p2->placeShips(b2);
    while(b1.allShipsDestroyed() != true || b2.allShipsDestroyed() != true)
    {
        cout << p1->name() << "'s " << "turn. Board for " << p2->name() << ":" << endl;
        if( p1->isHuman() == true)
        {
            b2.display(true);
        }
        else
        {
            b2.display(false);
        }
        ////////////////
        bool shotHit,shipDestroyed;
        int shipId;
        Point player1 = p1->recommendAttack();
        bool valid = b2.attack(player1,shotHit,shipDestroyed,shipId);
        p1->recordAttackResult(player1, valid, shotHit, shipDestroyed, shipId);
        if (valid == false && p1->isHuman() == true)
        {
            cout << p1->name() << " the Human wasted a shot at " << "( " << player1.r << "," << player1.c << ")" << endl;
            goto donep1;
        }
        if (shotHit == true)
        {
            if (shipDestroyed  == true)
            {
                cout << p1->name() <<  " attacked " << "(" << player1.r << "," <<  player1.c << ")" << " and destroyed the " << p1->game().shipName(shipId) << " resulting in: "<< endl;
            }
            else
            {
                cout << p1->name() <<  " attacked " << "(" << player1.r << "," <<  player1.c << ")" << " and hit something, resulting in:" << endl;
            }
        }
        if (shotHit == false)
        {
            cout << p1->name() << " attacked " << "(" << player1.r << "," <<  player1.c << ")" << " and missed, resulting in:" << endl;
        }
        donep1:;
        if( p1->isHuman() == true)
        {
            b2.display(true);
        }
        else
        {
            b2.display(false);
        }
        ///////////////////////
        if (b2.allShipsDestroyed() ==  true)
        {
            cout << p1->name() << " wins!" << endl;
            return p1;
        }
        if (shouldPause == true)
        {
            waitForEnter();
        }
        cout << p2->name() << "'s " << "turn. Board for " << p1->name() << ":" << endl;
        if( p2->isHuman() == true)
        {
            b1.display(true);
        }
        else
        {
            b1.display(false);
        }
        bool shotHit1,shipDestroyed1;
        int shipId1;
        Point player2 = p2->recommendAttack();
        bool valid1 = b1.attack(player2,shotHit1,shipDestroyed1,shipId1);
        p2->recordAttackResult(player2, valid1, shotHit1, shipDestroyed1, shipId1);
        if (valid1 == false && p2->isHuman() == true)
        {
            cout << p2->name() << " the Human wasted a shot at " << "( " << player2.r << "," << player2.c << ")" << endl;
            goto donep2;
        }
        if (shotHit1 == true)
        {
            if (shipDestroyed1  == true)
            {
                cout << p2->name() <<  " attacked " << "(" << player2.r << "," <<  player2.c << ")" << " and destroyed the " << p2->game().shipName(shipId1) << " resulting in: "<< endl;
            }
            else
            {
                cout << p2->name() <<  " attacked " << "(" << player2.r << "," <<  player2.c << ")" << " and hit something, resulting in:" << endl;
            }
        }
        if (shotHit1 == false)
        {
            cout << p2->name() << " attacked " << "(" << player2.r << "," <<  player2.c << ")" << " and missed, resulting in:" << endl;
        }
        donep2:;
        if( p2->isHuman() == true)
        {
            b1.display(true);
        }
        else
        {
            b1.display(false);
        }
        if (shouldPause == true)
        {
            waitForEnter();
        }
        if (b1.allShipsDestroyed() ==  true)
        {
            cout << p2->name() << " wins!" << endl;
            return p2;
        }
    }
    if (b2.allShipsDestroyed() ==  true && p2->isHuman() == true)
    {
        b1.display(false);
        return p1;
    }
    if (b1.allShipsDestroyed() ==  true && p1->isHuman() == true)
    {
        b2.display(false);
        return p2;
    }
    
    return nullptr;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0 && shipId < nShips());    //shipID >= 0 is wrong
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

