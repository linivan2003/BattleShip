#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

using namespace std;
//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};
AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( b.placeShip(Point(k,0), k, HORIZONTAL) == 0)
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
  //HumanPlayer
//*********************************************************************
bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player
{
  public:
    HumanPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual bool isHuman() const {return true;}
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

HumanPlayer::HumanPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}
bool HumanPlayer::placeShips(Board& b)
{
    cout << name() << " the Human must place " << game().nShips() << " ships." << endl;
    for (int i =0;i<game().nShips();i++)
    {
        b.display(false);
        cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
        string directionInput;
        getline(cin,directionInput);
        Direction directValue = VERTICAL;
        bool looper = true;
        while (looper == true)
        {
            switch (directionInput[0])
            {
                case 'h':
                    directValue = HORIZONTAL;
                    looper = false;
                    break;
                case 'v':
                    directValue = VERTICAL;
                    looper = false;
                    break;
                default:
                    cout << "Direction must be h or v" << endl;
                    cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) <<"): ";
                    getline(cin,directionInput);
                    break;
            }
        }
        int r = 0;
        int c = 0;
        cout << "Enter row and column of leftmost cell (e.g. 3 5): ";
        string coordinateInput;
        getline(cin,coordinateInput);
        looper = true;
        while (looper == true)
        {
            if (coordinateInput[0]  == '-' || coordinateInput[2] == '-')
            {
                cout << "The ship can not be placed there." << endl;
                cout << "Enter row and column of leftmost cell (e.g. 3 5): ";
                getline(cin,coordinateInput);
                continue;
            }
            if ( coordinateInput.size() > 3 || isdigit(coordinateInput[0]) == false || isdigit(coordinateInput[2]) == false || coordinateInput[1] != ' ')
            {
                cout << "You must enter two integers." << endl;
                cout << "Enter row and column of leftmost cell (e.g. 3 5): ";
                getline(cin,coordinateInput);
                continue;
            }
            else
            {
                r = coordinateInput[0] - 48;
                c = coordinateInput[2] - 48;
                Point p(r,c);
            }
            Point p(r,c);
            if (b.placeShip(p, i, directValue) == true)
            {
                looper = false;
            }
            else
            {
                cout << "The ship can not be placed there." << endl;
                cout << "Enter row and column of leftmost cell (e.g. 3 5): ";
                getline(cin,coordinateInput);
            }
        }
    }
    return true;
}

Point HumanPlayer::recommendAttack()
{
    int r,c;
    bool looper = true;
    cout << "Enter the row and column to attack (e.g., 3 5): ";
    while (looper == true)
    {
        if (getLineWithTwoIntegers(r, c) == false)
        {
            cout << "You must enter two integers." << endl;
            cout << "Enter the row and column to attack (e.g., 3 5): ";
            getLineWithTwoIntegers(r, c);
            continue;
        }
        else
        {
            Point p(r,c);
            return p;
        }
    }
    return m_lastCellAttacked;
}

void HumanPlayer::recordAttackResult(Point  p, bool validShot,
                                     bool shotHit , bool  shipDestroyed ,
                                     int  shipId)
{
    //dont need to do anything
}

void HumanPlayer::recordAttackByOpponent(Point p)
{
     // dont need to do anything
}

//*********************************************************************
//  MediocrePlayer
//*********************************************************************
class MediocrePlayer : public Player
{
  public:
    MediocrePlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    bool recurseAlgorithm(Board &b,int shipId);
    virtual void recordAttackByOpponent(Point p);
    void crossCellCreator(vector<Point>& vector1,Point a);
    Point randomPointPicker();
    void reinitialize();
  private:
    vector<Point> m_attackedCells;
    vector<Point> m_crossCells;
    Point m_lastCellAttacked;
    Point m_initalCellHit;
    int m_initialHit;
    bool m_validShot;
    int m_playerState;
    bool m_shotHit;
    bool m_shipDestroyed;
    int m_shipId;
};
MediocrePlayer::MediocrePlayer(string nm, const Game& g)
: Player(nm, g), m_lastCellAttacked(0, 0),m_initalCellHit(0,0), m_validShot(false), m_shotHit(false), m_shipDestroyed(false), m_shipId(0), m_playerState(1),m_initialHit(0)
{}
bool MediocrePlayer::recurseAlgorithm(Board &b, int shipId)
{
    if (shipId == game().nShips())
    {
        return true;
    }
    for (int r =0 ; r < game().rows();r++)
    {
        for (int c=0; c < game().cols(); c++)
        {
            Point p(r,c);
            if (b.placeShip(p, shipId, VERTICAL) == true || b.placeShip(p, shipId, HORIZONTAL) == true )
            {
                if (recurseAlgorithm(b, shipId + 1) == true)
                {
                    return true;
                }
                else
                {
                    b.unplaceShip(p,shipId,HORIZONTAL);
                    b.unplaceShip(p,shipId,VERTICAL);
                }
            }
        }
    }
    return false;
}
                
void MediocrePlayer::reinitialize()
{
    m_validShot = false;
    m_shotHit = false;
    m_shipDestroyed = false;
    m_shipId = 0;
}
bool checker(vector<Point> vector1, Point a)
{
    for (int i =0; i< vector1.size();i++)
    {
        if (a.r == vector1[i].r && a.c == vector1[i].c)
        {
            return true;
        }
    }
    return false;
}
void MediocrePlayer::crossCellCreator(vector<Point>& vector1,Point a)
{
   // cout << "sucessfully called cross cell creator" << endl;
    int initialr = a.r;
    int initialc = a.c;
    //right
    for (int i = 1; i < 5;i++ )
    {
        Point p(initialr,initialc + i);
        if ( initialc + i > game().cols()-1 || checker(m_attackedCells,p) == true)
        {
            continue;
        }
        else
        {
            //cout << "right" << endl;
            m_crossCells.push_back(p);
        }
    }
    // left
    for (int i = 1; i < 5;i++ )
    {
        Point p(initialr,initialc - i);
        if ( initialc - i < 0 || checker(m_attackedCells,p) == true)
        {
            continue;
        }
        else
        {
           // cout << "left" << endl;
            m_crossCells.push_back(p);
        }
    }
    // down
    for (int i = 1; i < 5;i++ )
    {
        Point p(initialr + i,initialc);
        if ( initialr + i > game().rows()-1 || checker(m_attackedCells,p) == true)
        {
            continue;
        }
        else
        {
            //cout << "down" << endl;
            m_crossCells.push_back(p);
        }
    }
    // up
    for (int i = 1; i < 5;i++ )
    {
        Point p(initialr - i,initialc);
        if ( initialr - i < 0 || checker(m_attackedCells,p) == true)
        {
            continue;
        }
        else
        {
           // cout << "up" << endl;
            m_crossCells.push_back(p);
        }
    }
}
Point MediocrePlayer::randomPointPicker()
{
    int r = randInt(game().rows());
    int c = randInt(game().cols());
    Point p(r,c);
    while (checker(m_attackedCells, p) == true)
    {
        int r = randInt(game().rows());
        int c = randInt(game().cols());
        p.r = r;
        p.c = c;
    }
    return p;
}
bool MediocrePlayer::placeShips(Board& b)
{
    int i = 0;
    while ( i != 49)
    {
        b.block();
        if (recurseAlgorithm(b, 0) == true)
        {
            b.unblock();
            return true;
        }
        else
        {
            i++;
            b.unblock();
        }
    }
    return false;
}

Point MediocrePlayer::recommendAttack()
{
    //cout << "m_shot value" << m_shotHit << endl;
   // cout << "inital hit value" << m_initialHit << endl;
    if (m_shotHit == true && m_initialHit == 0)
    {
        m_initalCellHit = m_lastCellAttacked;
        crossCellCreator(m_crossCells,m_initalCellHit);
        m_initialHit = 1;
    }
    if(m_shotHit == true)
    {
       // cout << "going to state 2" << endl;
        m_playerState = 2;
    }
    if(m_shipDestroyed == true)
    {
        // cout << "shipdestroed and going to state 1" << endl;
        m_crossCells.clear();
        m_initialHit = 0;
        m_playerState = 1;
    }
    if (m_playerState == 2)
    {
        if (m_crossCells.empty() == false)
        {
            int randomIndex = randInt(m_crossCells.size());
            Point p = m_crossCells[randomIndex];
            m_crossCells.erase(m_crossCells.begin() + randomIndex);
            m_attackedCells.push_back(p);
            reinitialize();
            return p;
        }
        else
        {
            //  cout << "going to STATE 1" << endl;
            m_playerState = 1;
        }
    }
    if (m_playerState == 1)
    {
        Point p = randomPointPicker();
        m_attackedCells.push_back(p);
        reinitialize();
        return p;
    }
    return m_lastCellAttacked;
}

void MediocrePlayer::recordAttackResult(Point  p, bool validShot,
                                     bool shotHit , bool  shipDestroyed ,
                                     int  shipId)
{
    if (validShot == true)
    {
       // cout << "valid shot received" << endl;
        m_lastCellAttacked = p;
        m_validShot = validShot;
        if (shotHit == true )
        {
          //  cout << "shothit = true" << endl;
            m_shotHit = true;
            if ( shipDestroyed == true)
            {
                m_shipDestroyed = shipDestroyed;
                m_shipId = shipId;
            }
        }
    }
}

void MediocrePlayer::recordAttackByOpponent(Point /* p */)
{
    
}
// Remember that Mediocre::placeShips(Board& b) must start by calling
// b.block(), and must call b.unblock() just before returning.
class GoodPlayer : public Player
{
  public:
    GoodPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    Point pointPicker();
    Point parityPointPicker();
    void reinitialize();
  private:
    vector<Point> m_attackedCells;
    Point m_lastCellAttacked;
    Point m_initalCellHit;
    int m_initialHit;
    bool m_validShot;
    int m_playerState;
    bool m_shotHit;
    bool m_shipDestroyed;
    int m_shipId;
    int m_smallestShipLength;
    int m_counter;
};
GoodPlayer::GoodPlayer(string nm, const Game& g)
: Player(nm, g), m_lastCellAttacked(0, 0),m_initalCellHit(0,0), m_validShot(false), m_shotHit(false), m_shipDestroyed(false), m_shipId(0), m_playerState(1),m_initialHit(0),m_smallestShipLength(2),m_counter(0)
{}
                
void GoodPlayer::reinitialize()
{
    m_validShot = false;
    m_shotHit = false;
    m_shipDestroyed = false;
    m_shipId = 0;
}
Point GoodPlayer::pointPicker()
{
    int lastr = m_lastCellAttacked.r;
        int lastc = m_lastCellAttacked.c;
        if (m_counter == 0 || m_counter == 1 || m_counter == 2 || m_counter == 3)
        {
            Point left = Point(lastr,lastc-1);
            if( left.r < 0 || left.r > game().rows()-1 || left.c < 0 || left.c >game().cols() - 1  )
            {
                m_counter = 4;
            }
            else
            {
                m_counter++;
                return left;
            }
        }
        if (m_counter == 4)
        {
            lastr = m_initalCellHit.r;
            lastc = m_initalCellHit.c;
        }
        if (m_counter == 4 || m_counter == 5 || m_counter == 6 || m_counter == 7)
        {
            Point right = Point(lastr,lastc + 1);
            if( right.r < 0 || right.r > game().rows()-1 || right.c < 0 || right.c >game().cols()-1)
            {
                m_counter = 8;
            }
            else
            {
                m_counter++;
                return right;
            }
        }
        if (m_counter == 8)
        {
            {
                lastr = m_initalCellHit.r;
                lastc = m_initalCellHit.c;
            }
        }
        if (m_counter == 8 || m_counter == 9 || m_counter == 10 || m_counter == 11)
        {
            Point up = Point(lastr+1,lastc);
            if( up.r < 0 || up.r > game().rows() -1 || up.c < 0 || up.c >game().cols() - 1)
            {
                m_counter = 12;
            }
            else
            {
                m_counter++;
                return up;
            }
        }
        if (m_counter == 12)
        {
            {
                lastr = m_initalCellHit.r;
                lastc = m_initalCellHit.c;
            }
        }
        if (m_counter == 12 || m_counter == 13 || m_counter == 14 || m_counter == 15)
        {
            Point down = Point(lastr-1,lastc);
            if( down.r < 0 || down.r > game().rows() - 1 || down.c < 0 || down.c >game().cols() - 1)
            {
                {
                    m_counter = 16;
                    return m_lastCellAttacked;
                }
            }
            else
            {
                m_counter++;
                return down;
            }
        }
        if (m_counter == 16)
        {
            m_counter =0;
        }
    return m_lastCellAttacked;
}
Point GoodPlayer::parityPointPicker()
{
    if (m_smallestShipLength == 3)
    {
        for(int r = 0; r < 10;  r++)
        {
            if (r == 0 || r == 2 || r ==4 || r ==6 || r == 8)
            {
                for (int c =2; c < 10; c = c+3)
                {
                    Point p(r,c);
                    if (checker(m_attackedCells,p) == true)
                    {
                        continue;
                    }
                    else
                    {
                        return p;
                    }
                }
            }
            if (r == 1 || r == 3 || r ==5 || r ==7 || r == 9)
            {
                for (int c =0; c < 10; c = c+3)
                {
                    Point p(r,c);
                    if (checker(m_attackedCells,p) == true)
                    {
                        continue;
                    }
                    else
                    {
                        return p;
                    }
                }
            }
        }
    }
    if (m_smallestShipLength == 2)
    {
        for(int r = 0; r < 10;  r++)
        {
            if (r == 0 || r == 2 || r ==4 || r ==6 || r == 8)
            {
                for (int c =1; c < 10; c = c+2)
                {
                    Point p(r,c);
                    if (checker(m_attackedCells,p) == true)
                    {
                        continue;
                    }
                    else
                    {
                        return p;
                    }
                }
            }
            if (r == 1 || r == 3 || r ==5 || r ==7 || r == 9)
            {
                for (int c =0; c < 10; c = c+2)
                {
                    Point p(r,c);
                    if (checker(m_attackedCells,p) == true)
                    {
                        continue;
                    }
                    else
                    {
                        return p;
                    }
                }
            }
        }
    }
    return m_lastCellAttacked;
}
bool GoodPlayer::placeShips(Board& b)
{
    Point p(3,5);
    Point p1(8,1);
    Point p2(2,2);
    Point p3(1,6);
    Point p4(9,6);
    b.placeShip(p,0, VERTICAL);
    b.placeShip(p1,1, HORIZONTAL);
    b.placeShip(p2,2, HORIZONTAL);
    b.placeShip(p3,3, HORIZONTAL);
    b.placeShip(p4,4, HORIZONTAL);
    return true;
}

Point GoodPlayer::recommendAttack()
{
    //cout << "m_shot value" << m_shotHit << endl;
   // cout << "inital hit value" << m_initialHit << endl;
    if (m_shotHit == true && m_initialHit == 0)
    {
        m_initalCellHit = m_lastCellAttacked;
        m_initialHit = 1;
    }
    if(m_shotHit == true)
    {
       // cout << "going to state 2" << endl;
        m_playerState = 2;
    }
    if(m_shipDestroyed == true)
    {
        // cout << "shipdestroed and going to state 1" << endl;
        m_initialHit = 0;
        m_counter = 0;
        m_playerState = 1;
        if (m_shipId == 4)
        {
            m_smallestShipLength = 3;
        }
    }
    if (m_playerState == 2)
    {
        Point p = pointPicker();

        m_attackedCells.push_back(p);
        reinitialize();
        return p;
    }
    else
    {
        //  cout << "going to STATE 1" << endl;
        m_playerState = 1;
    }
    if (m_playerState == 1)
    {
        Point p = parityPointPicker();
        m_attackedCells.push_back(p);
        reinitialize();
        return p;
    }
    return m_lastCellAttacked;
}

void GoodPlayer::recordAttackResult(Point  p, bool validShot,
                                     bool shotHit , bool  shipDestroyed ,
                                     int  shipId)
{
    if (validShot == true)
    {
       // cout << "valid shot received" << endl;
        m_lastCellAttacked = p;
        m_validShot = validShot;
        if (shotHit == true )
        {
          //  cout << "shothit = true" << endl;
            m_shotHit = true;
            if ( shipDestroyed == true)
            {
                m_shipDestroyed = shipDestroyed;
                m_shipId = shipId;
            }
        }
    }
}

void GoodPlayer::recordAttackByOpponent(Point /* p */)
{
    
}
//*********************************************************************
//  GoodPlayer
//*********************************************************************
////*********************************************************************
////  createPlayer
////*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}

