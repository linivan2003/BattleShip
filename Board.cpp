#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
      // TODO:  Decide what private members you need.  Here's one that's likely
      //        to be useful:
    const Game& m_game;
    char m_board[MAXROWS][MAXCOLS];
    int m_shipIDsPlaced[5];
    int m_numShipIDsPlaced;
    
};

BoardImpl::BoardImpl(const Game& g)
 : m_game(g)
{
    m_numShipIDsPlaced = 0;
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            m_board[r][c] = '.';
        }
}

void BoardImpl::clear()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            m_board[r][c] = '.';
        }
    for (int i =0; i < m_numShipIDsPlaced;i++)
    {
        m_shipIDsPlaced[i] = -1;
    }
}

void BoardImpl::block()
{
      // Block cells with 50% probability
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
            if (randInt(2) == 0)
            {
                m_board[r][c] = '#';
            }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
            if (m_board[r][c] == '#')
                {
                    m_board[r][c] = '.';
                }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    if ( shipId < 0 )
    {
        return false;
    }
    if(shipId > 4)
    {
        return false;
    }
    if (m_numShipIDsPlaced > 5)
    {
        return false;
    }
    for (int i =0;i < m_numShipIDsPlaced;i++)
    {
        if (shipId == m_shipIDsPlaced[i])
        {
            return false;
        }
    }
    if (shipId > m_game.nShips()-1)
    {
        return (false);
    }
    if (m_game.shipLength(shipId) > m_game.cols()-topOrLeft.c && dir == HORIZONTAL)
    {
        return false;
    }
    if (m_game.shipLength(shipId) > m_game.rows()-topOrLeft.r && dir == VERTICAL)
    {
        return false;
    }
    if (dir == HORIZONTAL)
    {
        for (int c= topOrLeft.c; c < topOrLeft.c + m_game.shipLength(shipId);c++)
        {
           if(m_board[topOrLeft.r][c] != '.')
           {
               return false;
           }
        }
    }
    if (dir == VERTICAL)
    {
        for (int r= topOrLeft.r; r < topOrLeft.r + m_game.shipLength(shipId);r++)
        {
           if(m_board[r][topOrLeft.c] != '.')
           {
               return false;
           }
        }
    }
    if (dir == HORIZONTAL)
    {
        for (int c= topOrLeft.c; c < topOrLeft.c + m_game.shipLength(shipId);c++)
        {
            char a = m_game.shipSymbol(shipId);
            m_board[topOrLeft.r][c] = a;
        }
        m_shipIDsPlaced[m_numShipIDsPlaced] = shipId;
        m_numShipIDsPlaced++;
    }
    if (dir == VERTICAL)
    {
        for (int r= topOrLeft.r; r < topOrLeft.r + m_game.shipLength(shipId);r++)
        {
            char a = m_game.shipSymbol(shipId);
            m_board[r][topOrLeft.c] = a;
        }
        m_shipIDsPlaced[m_numShipIDsPlaced] = shipId;
        m_numShipIDsPlaced++;
    }
    return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    if (shipId < 0)
    {
        return false;
    }
    char a = m_game.shipSymbol(shipId);
    for (int i = 0; i < m_numShipIDsPlaced;i++)
    {
        if (shipId == m_shipIDsPlaced[i])
        {
            if (dir == HORIZONTAL)
            {
                for (int c= topOrLeft.c; c < topOrLeft.c + m_game.shipLength(shipId);c++)
                {
                   if(m_board[topOrLeft.r][c] != a)
                       return false;
                }
            }
            if (dir == VERTICAL)
            {
                for (int r= topOrLeft.r; r < topOrLeft.r + m_game.shipLength(shipId);r++)
                {
                   if(m_board[r][topOrLeft.c] != a)
                       return false;
                }
            }
            if (dir == HORIZONTAL)
            {
                for (int c= topOrLeft.c; c < topOrLeft.c + m_game.shipLength(shipId);c++)
                {
                    m_board[topOrLeft.r][c] = '.';
                }
                m_shipIDsPlaced[i] = m_shipIDsPlaced[m_numShipIDsPlaced-1];
                m_numShipIDsPlaced--;
                return true;
            }
            if (dir == VERTICAL)
            {
                for (int r= topOrLeft.r; r < topOrLeft.r + m_game.shipLength(shipId);r++)
                {
                    m_board[r][topOrLeft.c] = '.';
                }
                m_shipIDsPlaced[i] = m_shipIDsPlaced[m_numShipIDsPlaced-1];
                m_numShipIDsPlaced--;
                return true;
            }
        }
    }
    return false;
}

void BoardImpl::display(bool shotsOnly) const //FIX WITH DIFFERENT SYMBOLS
{
    cout << ' ' << ' ';
    for (int c =0; c < m_game.cols();c++)
    {
        cout << c;
    }
    cout << endl;
    int counter = 0;
    for (int r = 0;r < m_game.rows(); r++)
    {
        cout << counter;
        cout << ' ';
        counter++;
        for (int c=0;c < m_game.cols();c++)
        {
            if (m_board[r][c] == 'X')
            {
                cout << 'X';
                continue;
            }
            if (m_board[r][c] == 'o')
            {
                cout << 'o';
                continue;
            }
            if (m_board[r][c]== '#')
            {
                cout << '#';
                continue;
            }
            if (m_board[r][c] != '.' && shotsOnly == false)
            {
                cout << m_board[r][c];
                continue;
            }
            cout << '.';
        }
        cout << endl;
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    if (p.r > m_game.rows()-1 || p.c > m_game.cols()-1 || p.r < 0 || p.c < 0)
    {
        return false;
    }
    if (m_board[p.r][p.c] == 'o' || m_board[p.r][p.c] == 'X')
    {
        return false;
    }
    if (m_board[p.r][p.c] == '.')
    {
        shotHit = false;
        m_board[p.r][p.c] = 'o';
        return true;
    }
    if (m_board[p.r][p.c] != 'X' || m_board[p.r][p.c] != 'o')
    {
        char a = m_board[p.r][p.c];
        shotHit = true;
        shipDestroyed = true;
        m_board[p.r][p.c] = 'X';
        for (int r = 0; r < m_game.rows();r++)
        {
            if (m_board[r][p.c] == a)
            {
                shipDestroyed = false;
            }
        }
        for (int c = 0; c < m_game.cols();c++)
        {
            if (m_board[p.r][c] == a)
            {
                shipDestroyed = false;
            }
        }
        if (shipDestroyed == true)
        {
            for ( int i = 0; i< m_game.nShips();i++)
            {
                if (m_game.shipSymbol(i) == a)
                {
                    shipId = i;
                    return true;
                }
            }
        }
    }
    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            switch (m_board[r][c]) {
                case '.':
                    break;
                case 'X':
                    break;
                case 'o':
                    break;
                default:
                    return false;
            }
        }
    }
    return (true);
}


//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
