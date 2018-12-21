#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <thread>
#include <future>
#include <mutex>
#include <list>
#include <fstream>
#include <sstream>
#include <random>

#include <Windows.h>

using namespace std;
using namespace std::chrono;

// g++ -std=c++11 A1.cpp -o A1

class ExecutionTimer {

    private: 
    
    typename std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
    typename std::chrono::time_point<std::chrono::high_resolution_clock> m_delta;
    typename std::chrono::time_point<std::chrono::high_resolution_clock> m_end;

    public: 

    void start() {
        m_start = std::chrono::high_resolution_clock::now();
    }

    void end() {
        m_end = std::chrono::high_resolution_clock::now();
    }

    void delta() {
        m_delta = std::chrono::high_resolution_clock::now();
    }

    microseconds getDuration() {
        return duration_cast<microseconds>(m_end - m_start);
    }

    microseconds getDelta() {
        return duration_cast<microseconds>(m_delta - m_start);
    }

    string getString() {

        microseconds ms = getDuration();
        hours   hh = duration_cast<hours>(ms);
        minutes mm = duration_cast<minutes>(ms % chrono::hours(1));
        seconds ss = duration_cast<seconds>(ms % chrono::minutes(1));
        milliseconds msec = duration_cast<milliseconds>(ms % chrono::seconds(1));
        microseconds mss = duration_cast<microseconds>(ms % chrono::milliseconds(1));

        std::ostringstream ostr;
        if (hh.count()>0) {
            ostr << hh.count() << "h ";
            ostr << mm.count() << "min ";
            ostr << ss.count() << "s ";
            ostr << msec.count() << "ml";
        } else if (mm.count()>0) {
            ostr << mm.count() << "min ";
            ostr << ss.count() << "s ";
            ostr << msec.count() << "ml";
        } else if (ss.count()>0) {
            ostr << ss.count() << "s ";
            ostr << msec.count() << "ml";
        } else if (msec.count()>0) {
            ostr << msec.count() << "ml ";
            ostr << mss.count() << "ms";
        } else {
            ostr << mss.count() << "ms";
        }
        return ostr.str();
    }

    static string toString(microseconds ms) {

        hours   hh = duration_cast<hours>(ms);
        minutes mm = duration_cast<minutes>(ms % chrono::hours(1));
        seconds ss = duration_cast<seconds>(ms % chrono::minutes(1));
        milliseconds msec = duration_cast<milliseconds>(ms % chrono::seconds(1));
        microseconds mss = duration_cast<microseconds>(ms % chrono::milliseconds(1));

        std::ostringstream ostr;
        if (hh.count()>0) {
            ostr << hh.count() << "h ";
            ostr << mm.count() << "min ";
            ostr << ss.count() << "s ";
            ostr << msec.count() << "ml";
        } else if (mm.count()>0) {
            ostr << mm.count() << "min ";
            ostr << ss.count() << "s ";
            ostr << msec.count() << "ml";
        } else if (ss.count()>0) {
            ostr << ss.count() << "s ";
            ostr << msec.count() << "ml";
        } else if (msec.count()>0) {
            ostr << msec.count() << "ml ";
            ostr << mss.count() << "ms";
        } else {
            ostr << mss.count() << "ms";
        }
        return ostr.str();
    }

};

int function_(int t) {
    std::cerr << "T2" << std::endl;
    // Get Start Time
	system_clock::time_point start = system_clock::now();
    int r = 0;

    for (int i = 0; i < 100000000000000; i++) {
        auto end = system_clock::now();
        auto diff = duration_cast < std::chrono::milliseconds > (end - start).count();
        // std::cout << "diff= " << diff << std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(t));
        if (diff > 900) {
            //cout << "NbIteration: " << i+1 << endl;
            // this_thread::sleep_for(chrono::milliseconds(20));
            // break;
            r = i+1;
            break;
        }
        //doSomething();
        // this_thread::sleep_for(chrono::milliseconds(10));
        // cout << "Iteration " << i << endl;
        //this_thread::sleep_for(chrono::milliseconds(1));
    }
    // r = 10000;
    return r;
}

void testTrhreads() {
    // Get Start Time
	system_clock::time_point start = system_clock::now();

    std::future<int> result = std::async(std::launch::async, function_, 1);
    std::future<int> result2 = std::async(std::launch::async, function_, 3);
    std::future<int> result3 = std::async(std::launch::async, function_, 1);
    std::cerr << "T1" << std::endl;
    int r = result.get();
    int r2 = result2.get();
    int r3 = result3.get();
    std::cerr << "r = " << r << std::endl;
    std::cerr << "r2 = " << r2 << std::endl;
    std::cerr << "r3 = " << r3 << std::endl;
    std::cerr << "r+r2 = " << r+r2+r3 << std::endl;

    // Get End Time
	auto end = system_clock::now();
 
	auto diff = duration_cast < std::chrono::milliseconds > (end - start).count();
	std::cerr << "Total Time Taken = " << diff << " ml" << std::endl;
}

class Position {
    public:
    int x;
    int y;

    Position(){}
    Position(int inX, int inY) {
        x = inX;
        y = inY;
    }

    bool equals(Position pos) {
        bool out = false;
        if(x==pos.x
        && y==pos.y) {
            out = true;
        }
        return out;
    }
    string toString() {
        ostringstream ostr;
        ostr << "(" << x << "," << y << ")";
        return ostr.str();
    }
};


class Orientation {
    public:
    int orientation;
    static const Orientation horizontal;
    static const Orientation vertical;

    Orientation(void) {
        orientation = 0;
        return;
    };
    Orientation(int value) {
        orientation = value;
        return;
    };

    void rotate(int d) { 
        if (d>0) {
            for(int n = 0; n < d; n++) {
                int i = orientation & 8;
                orientation <<= 1;
                if (i>0) orientation+=1;
                if (orientation>=16) orientation-=16;
            }
        } else if (d<0) {
            for(int n = d ; n < 0; n++) {
                int i = orientation & 1;
                orientation >>= 1;
                if (i>0) orientation+=8;
            }
        }
    } 

    bool isOppose() { 
        bool out = false;
        if (((orientation & horizontal.orientation) == horizontal.orientation) 
                || ((orientation & vertical.orientation) == vertical.orientation) ) {
            // cerr << "orientation = " << (orientation)  << endl;
            // cerr << "horizontal.orientation = " << (Orientation::horizontal.orientation)  << endl;
            // cerr << "orientation & horizontal.orientation = " << (orientation & Orientation::horizontal.orientation)  << endl;
            out = true;
        }
        return out;
    } 

    bool isNotOppose() { 
        return !isOppose();
    } 

    Orientation getSensHoraireAngle() {
        Orientation out(0);
        if (orientation==6) {  // v>
            out.orientation = 2;
        } else if (orientation==12) { // <v
            out.orientation = 4;
        } else if (orientation==9) { // <^
            out.orientation = 8;
        } else if (orientation==3) { // ^>
            out.orientation = 1;
        }
        return out;
    }

    Orientation getSensHoraireFlanc() {
        Orientation out(0);
        if (orientation==11) {  // <^>
            out.orientation = 8;
        } else if (orientation==14) { // <v>
            out.orientation = 2;
        } else if (orientation==7) { // ^v>
            out.orientation = 1;
        } else if (orientation==13) { // ^v>
            out.orientation = 4;
        }
        return out;
    }

        Orientation getSensHoraireAngle2() {
        Orientation out(0);
        if (orientation==6) {  // v>
            out.orientation = 4;
        } else if (orientation==12) { // <v
            out.orientation = 8;
        } else if (orientation==9) { // <^
            out.orientation = 1;
        } else if (orientation==3) { // ^>
            out.orientation = 2;
        }
        return out;
    }

    Orientation getSensHoraireFlanc2() {
        Orientation out(0);
        if (orientation==11) {  // <^>
            out.orientation = 2;
        } else if (orientation==14) { // <v>
            out.orientation = 8;
        } else if (orientation==7) { // ^v>
            out.orientation = 4;
        } else if (orientation==13) { // ^v>
            out.orientation = 1;
        }
        return out;
    }

    string toString() const {
        ostringstream ostr;
        ostr << "[" << orientation << "]";
        return ostr.str();
    }

    bool equals(Orientation value) {
        bool out = false;
        if(orientation == value.orientation) {
            out = true;
        }
        return out;
    }

    bool notEquals(Orientation value) {
        bool out = false;
        if(orientation != value.orientation) {
            out = true;
        }
        return out;
    }

    bool exist() {
        bool out = false;
        if(orientation>0) {
            out = true;
        }
        return out;
    }

    bool contains(Orientation value) {
        bool out = false;
        if(orientation & value.orientation) {
            out = true;
        }
        return out;
    }

    void add(Orientation value) {
        orientation+=value.orientation;
    }

    void set(Orientation value) {
        orientation=value.orientation;
    }

    static Orientation getOrientation(string value) {
        Orientation out(0);
        if ("U" == value) {
            out.orientation = 1;
        } else if ("R" == value) {
            out.orientation = 2;
        } else if ("D" == value) {
            out.orientation = 4;
        } else if ("L" == value) {
            out.orientation = 8;
        }
        return out;
    }

    static string getString(Orientation inOrient) {
        string out;
        if (inOrient.orientation == 1) {
            out = "U";
        } else if (inOrient.orientation == 2) {
            out = "R";
        } else if (inOrient.orientation == 4) {
            out = "D";
        } else if (inOrient.orientation == 8) {
            out = "L";
        }
        return out;
    }

    int nbSens() {
        // countSetBits
        int count = 0;
        int orient = orientation;
        while (orient > 0) { 
            count += orient & 1; 
            orient >>= 1; 
        } 
        return count; 
    }

    list<int> getListeSens() {
        list<int> listeSens;
        listeSens.push_back(bitExtracted(orientation, 1, 4));
        listeSens.push_back(bitExtracted(orientation, 1, 3));
        listeSens.push_back(bitExtracted(orientation, 1, 2));
        listeSens.push_back(bitExtracted(orientation, 1, 1));
        return listeSens; 
    }

    // Function to extract k bits from p position 
    // and returns the extracted value as integer 
    // @see https://www.geeksforgeeks.org/extract-k-bits-given-position-number/
    int bitExtracted(int number, int k, int p) { 
        return (((1 << k) - 1) & (number >> (p - 1))); 
    } 

};

const Orientation Orientation::horizontal(10);
const Orientation Orientation::vertical(5);

class Cellule {
    public:
    bool isPlateforme = false;
    string caractere;
    Position position;

    // U 1, R 2, D 4, L 8
    Orientation fleche;
    Orientation contact;
    Orientation portal;
    Orientation axes;
    array<Orientation, 10> traces;

    Orientation flecheAjoute;
    Orientation flecheFusionne;

    string toString() {
        string out;
        if (isPlateforme
        && flecheAjoute.exist()) {
            if (flecheAjoute.orientation == 1) {
                out = "^";
            } else if (flecheAjoute.orientation == 2) {
                out = ">";
            } else if (flecheAjoute.orientation == 4) {
                out = "v";
            } else if (flecheAjoute.orientation == 8) {
                out = "<";
            }
        } else {
            out = caractere;
        }
        return out;
    }

    void printWindows() {
        HANDLE  HCmd = GetStdHandle(STD_OUTPUT_HANDLE); //creer un handle de sortie
        SetConsoleTextAttribute(HCmd, 0|FOREGROUND_INTENSITY);  
        if (isPlateforme
        && flecheAjoute.exist()) {
            SetConsoleTextAttribute(HCmd, 2|FOREGROUND_INTENSITY);
            if (flecheAjoute.orientation == 1) {
                cerr << "^";
            } else if (flecheAjoute.orientation == 2) {
                cerr << ">";
            } else if (flecheAjoute.orientation == 4) {
                cerr << "v";
            } else if (flecheAjoute.orientation == 8) {
                cerr << "<";
            }
        } else {
            cerr << caractere;
        }
        SetConsoleTextAttribute(HCmd, 7);  // couleur rouge
    }


    void mettreAJourFlecheFusionne() {
        if (flecheAjoute.exist()) {
            flecheFusionne.set(flecheAjoute);
        } else if (fleche.exist()) {
            flecheFusionne.set(fleche);
        } else {
            flecheFusionne.orientation == 0;;
        }
    }

    string toInfo() {
        ostringstream ostr;
        if (isPlateforme) {
            ostr << "cel.pos" << position.toString() << ".axes" << axes.toString() << ".contact" << contact.toString() << ".portal" << portal.toString() << ".fleche" << fleche.toString();
            ostr << ".flecheAjoute" << flecheAjoute.toString();
            ostr << ".flecheFusionne" << flecheFusionne.toString();
        }
        return ostr.str();
    }

    int nbSens() {
        return axes.nbSens()+1; 
    }

};

class Robot {

    private:
    bool empty;

    public:
    int id;
    Position position;
    Orientation sens; // U 1, R 2, D 4, L 8
    bool isFonctionne = true;

    Orientation sensOrigine;
    Position positionOrigine;

    static const int tableWidth = 19;
    static const int tableHeight = 10;

    Robot(void) {empty=true;return;};
    Robot(int inId) {
        empty=false;
        isFonctionne = true;
        id=inId;
        return;
    };
    ~Robot(void) {empty=true;return;};
    bool isEmpty() {
        return empty;
    };

    void reset() {
        sens.orientation = sensOrigine.orientation;
        position.x = positionOrigine.x;
        position.y = positionOrigine.y;
        isFonctionne = true;
    }

    void setPosition(int x, int y) {
        position.x = x;
        position.y = y;
        positionOrigine.x = x;
        positionOrigine.y = y;
    }

    void setOrientation(int inSens) {
        sens.orientation = inSens;
        sensOrigine.orientation = inSens;
    }

    string toString() {
        ostringstream ostr;
        string strFonctionne = (isFonctionne) ? "Fonctionne" : "Disfonctionne";
        ostr << "robot[" << id << "].pos" << position.toString() << ".sens" << sens.toString() << "." << strFonctionne ;
        return ostr.str();
    }

    // Actions

    bool isPivotable(Orientation orient, bool debug) {
        bool out = false;
        if (orient.exist()
                && orient.notEquals(sens)) {
            out = true;
        }
        return out;
    }

    void pivoter(Orientation orient, bool debug) {
            if (debug) cerr << "robot[" << id << "].PIVOTE vers orient";
            sens = orient;
            if (debug) cerr << sens.toString() << endl;
    }

    Position avancer(bool debug) {
        Position pos = position;
        // U 1, R 2, D 4, L 8
            if(sens.orientation==2) {
                if (position.x==tableWidth-1) {
                    position.x=0;
                } else {
                    position.x++;
                }
            } else if(sens.orientation==8) {
                if (position.x==0) {
                    position.x=tableWidth-1;
                } else {
                    position.x--;
                }
            } else if(sens.orientation==1) {
                if (position.y==0) {
                    position.y=tableHeight-1;
                } else {
                    position.y--;
                }
            } else if(sens.orientation==4) {
                if (position.y==tableHeight-1) {
                    position.y=0;
                } else {
                    position.y++;
                }
            }
            if (pos.equals(position)) {
                isFonctionne = false;
                if (debug) cerr << "robot[" << id << "].BUGGUE en restant imobile." << endl;
            }
        return position;
    }

    bool determinerFonctionnement(Cellule cellule, bool debug) {

            if (!cellule.isPlateforme) {
                isFonctionne = false;
                if (debug) {
                    cerr << "robot[" << id << "].TOMBE dans le vide." << endl;
                }
            }


            if (isFonctionne && cellule.traces[id].contains(sens)) {
                isFonctionne = false;
                if (debug) {
                    cerr << "robot[" << id << "].EXPLOSE sur re-sens. ";
                    cerr << "cel" << cellule.position.toString() << ".trace" <<cellule.traces[id].toString();
                    cerr << " inclus robot.sens" << sens.toString() << endl;
                }
            }


            return isFonctionne;
        }

};

class Plateau {

    public:

        static const int tableWidth = 19;
        static const int tableHeight = 10;
        int id;
        string name;
        array<array<Cellule, tableHeight>, tableWidth> grille;
        int robotCount;
        array<Robot, 10> robots;

        bool isDebugSimulation = false;
        // Commande commande;

        Plateau() {}
        Plateau(string str) { name = str;}
        ~Plateau() {}

        void tableauPrintWindows() {
            for (int y = 0; y < tableHeight; y++) {
                for (int x = 0; x < tableWidth; x++) {
                    grille[x][y].printWindows();
                }
                cerr << endl;
            }
        }

        string tableauToString() {
            ostringstream ostr;
            for (int y = 0; y < tableHeight; y++) {
                for (int x = 0; x < tableWidth; x++) {
                    ostr << grille[x][y].toString() ;
                }
                ostr << endl;
            }
            return ostr.str();
        }

        string printInfo() {
            ostringstream ostr;
            for (int y = 0; y < tableHeight; y++) {
                for (int x = 0; x < tableWidth; x++) {
                    if (grille[x][y].isPlateforme) {
                        ostr << grille[x][y].toInfo() << endl;
                    }
                }
            }
            return ostr.str();
        }

        int nbPlateformes() {
            int out = 0;
            for (int y = 0; y < tableHeight; y++) {
                for (int x = 0; x < tableWidth; x++) {
                    if (grille[x][y].isPlateforme) {
                        out++;
                    }
                }
            }
            return out;
        }

        string print() {
            ostringstream ostr;
            cerr << "plateau(" << id << ").name=" << name << endl;
                tableauPrintWindows();
             cerr << nbPlateformes() << " plateformes" << endl
                << printInfo()
                << "robotCount=" << robotCount << endl;
                for (int i = 0; i < robotCount; i++) {
                    ostr << robots[i].toString() << endl;
                }
        }

        string toString() {
            ostringstream ostr;
            ostr << "plateau(" << id << ").name=" << name << endl
                << tableauToString()
                << nbPlateformes() << " plateformes" << endl
                << printInfo()
                << "robotCount=" << robotCount << endl;
                for (int i = 0; i < robotCount; i++) {
                    ostr << robots[i].toString() << endl;
                }
            return ostr.str();
        }

        void initAxesCellule (Position inPos) {
            int x = inPos.x;
            int y = inPos.y;

            // Init
            grille[x][y].contact.orientation=0;
            grille[x][y].fleche.orientation=0;
            grille[x][y].portal.orientation=0;

            // contact
            if (grille[x][y].caractere == ".") {
                if (x>0) {
                    if (grille[x-1][y].caractere == ".") {
                        grille[x][y].contact.orientation+=8;
                    }
                }
                if (x<tableWidth-1) {
                    if (grille[x+1][y].caractere == ".") {
                        grille[x][y].contact.orientation+=2;
                    }
                }
                if (y>0) {
                    if (grille[x][y-1].caractere == ".") {
                        grille[x][y].contact.orientation+=1;
                    }
                }
                if (y<tableHeight-1) {
                    if (grille[x][y+1].caractere == ".") {
                        grille[x][y].contact.orientation+=4;
                    }
                }
            } else { 
                // fleche
                grille[x][y].fleche = Orientation::getOrientation(grille[x][y].caractere);
            }

            // portail
            if (x==0) {
                if (grille[tableWidth-1][y].caractere != "#") {
                    grille[x][y].portal.orientation+=8;
                }
            }
            if (x==tableWidth-1) {
                if (grille[0][y].caractere != "#") {
                    grille[x][y].portal.orientation+=2;
                }
            }
            if (y==0) {
                if (grille[x][tableHeight-1].caractere != "#") {
                    grille[x][y].portal.orientation+=1;
                }
            }
            if (y==tableHeight-1) {
                if (grille[x][0].caractere != "#") {
                    grille[x][y].portal.orientation+=4;
                }
            }

            // Axes
            if (grille[x][y].fleche.exist()) {
                grille[x][y].axes.set(grille[x][y].fleche);
            } else {
                grille[x][y].axes.add(grille[x][y].contact);
                grille[x][y].axes.add(grille[x][y].portal);
            }

        }

        static void reInitPlateau(Plateau &plateau) {
            for (int y = 0; y < tableHeight; y++) {
                for (int x = 0; x < tableWidth; x++) {
                    if (plateau.grille[x][y].caractere != "#") {
                        plateau.grille[x][y].axes.orientation = 0;
                        Orientation trace;
                        trace.orientation = 0;
                        plateau.grille[x][y].traces = {trace};
                        if (plateau.grille[x][y].flecheAjoute.exist()) {
                            plateau.grille[x][y].axes.set(plateau.grille[x][y].flecheAjoute);
                            cerr << "flecheAjoute " << plateau.grille[x][y].flecheAjoute.toString() << endl;
                            plateau.grille[x][y].flecheFusionne.set(plateau.grille[x][y].flecheAjoute);
                            cerr << "flecheFusionne " << plateau.grille[x][y].flecheFusionne.toString() << endl;
                        } else if (plateau.grille[x][y].fleche.exist()) {
                            plateau.grille[x][y].axes.set(plateau.grille[x][y].fleche);
                            plateau.grille[x][y].flecheFusionne.set(plateau.grille[x][y].fleche);
                        } else {
                            plateau.grille[x][y].axes.add(plateau.grille[x][y].contact);
                            plateau.grille[x][y].axes.add(plateau.grille[x][y].portal);
                            plateau.grille[x][y].flecheFusionne.orientation == 0;;
                        }
                    }
                }
            }
            for (int i = 0; i < plateau.robotCount; i++) {
                plateau.robots[i].reset();
                Position pos = plateau.robots[i].position;
                Orientation trace;
                trace.orientation=plateau.robots[i].sens.orientation;
                plateau.grille[pos.x][pos.y].traces[i]=trace;
            }
        }

        void initTraceDepart () {
            // cerr << name << ".initTraceDepart:DEBUT" <<  endl;
            for (int i = 0; i < robotCount; i++) {
                Position pos = robots[i].position;
                // cerr << name << ".robot" << robots[i].position.toString() <<  endl;
                Orientation trace;
                trace.orientation=robots[i].sens.orientation;
                grille[pos.x][pos.y].traces[i]=trace;
            }
            // cerr << name << ".initTraceDepart:DEBUT" <<  endl;
        }

        void init() {
            for (int y = 0; y < tableHeight; y++) {
                for (int x = 0; x < tableWidth; x++) {
                    // if (x==0 && y == 0) cerr << name << ".init:DEBUT" <<  endl;
                    if (grille[x][y].caractere != "#") {
                        grille[x][y].isPlateforme = true;
                        Orientation trace;
                        trace.orientation = 0;
                        grille[x][y].traces = {trace};
                        grille[x][y].position = Position(x, y);
                        grille[x][y].flecheAjoute.orientation = 0;
                        initAxesCellule(grille[x][y].position);
                        grille[x][y].flecheFusionne.set(grille[x][y].fleche);
                    }
                    // if (x==18 && y == 9) cerr << name << ".init:FIN" <<  endl;
                }
            }
            initTraceDepart();
        }

        void clear() {
            for (int y = 0; y < tableHeight; y++) {
                for (int x = 0; x < tableWidth; x++) {
                    grille[x][y].isPlateforme = false;
                    Orientation trace;
                    grille[x][y].traces = {trace};
                }
            }
            for (int i = 0; i < robotCount; i++) {
                Robot robots[i];
            }
        }

        void tracer(Robot &inRobot) {
            Position pos = inRobot.position;
            grille[pos.x][pos.y].traces[inRobot.id].add(inRobot.sens);
        }

};

// getGrille
array<array<Cellule, 10>, 19> getGrille(string file) {
    std::fstream fs;
	string line;
    array<array<Cellule, 10>, 19> grille;
    fs.open(file);
    if(fs.is_open()) {
        bool isfind = false;
        int numLine = 0;
        while(fs.good() && !isfind) {
            getline(fs,line);
            int numChar = 0;
            for(std::string::iterator it = line.begin(); it != line.end(); ++it) {
                Cellule cellule;
                cellule.caractere = *it;
                grille[numChar][numLine]=cellule;
                numChar++;
            }
            numLine++;
        }
    }
    fs.close();
    return grille;
}

// U 1, R 2, D 4, L 8



// U 1, R 2, D 4, L 8

class Util {

    public:
    static double calculerCombinaison(list<Cellule> inListeCellules) {
        double out = 1;
        list<Cellule>::iterator itListeCellules;
        for (itListeCellules=inListeCellules.begin(); itListeCellules!=inListeCellules.end(); ++itListeCellules) {
            // cerr << itListeCellules->nbSens() << endl;
            out = out * itListeCellules->nbSens();
        }
        return out;
    }

    static list<Cellule> recupererListeCellulesPlateforme(Plateau &plateau) {
        list<Cellule> listeCellules;
         for (int y = 0; y < plateau.tableHeight; y++) {
            for (int x = 0; x < plateau.tableWidth; x++) {
                if (plateau.grille[x][y].isPlateforme
                    && !plateau.grille[x][y].fleche.exist()) {
                        listeCellules.push_back(plateau.grille[x][y]);
                    }
            }
        }
        return listeCellules;
    }

    static list<Cellule> recupererListeCellulesBord(Plateau &plateau) {
        list<Cellule> listeCellules;
         for (int y = 0; y < plateau.tableHeight; y++) {
            for (int x = 0; x < plateau.tableWidth; x++) {
                if (plateau.grille[x][y].isPlateforme
                    && !plateau.grille[x][y].fleche.exist()
                    && plateau.grille[x][y].nbSens() <= 3  ) {
                        listeCellules.push_back(plateau.grille[x][y]);
                    }
            }
        }
        return listeCellules;
    }

    static void printListeCellules(list<Cellule> inListeCellules) {
        cerr << "listeCellules " << inListeCellules.size() << " plateformes" << endl;
        list<Cellule>::iterator itListeCellules;
        for (itListeCellules=inListeCellules.begin(); itListeCellules!=inListeCellules.end(); ++itListeCellules) {
            cerr << itListeCellules->toInfo() << ".nbSens=" << itListeCellules->nbSens() << endl;
        }
    }

    static void printListeSens(list<Cellule> inListeCellules) {
        cerr << "listeCellules " << inListeCellules.size() << " plateformes" << endl;
        list<Cellule>::iterator itListeCellules;
        for (itListeCellules=inListeCellules.begin(); itListeCellules!=inListeCellules.end(); ++itListeCellules) {
            cerr << itListeCellules->toInfo() << ".nbSens=" << itListeCellules->nbSens() << endl;
            list<int> listeSens = itListeCellules->axes.getListeSens();
            list<int>::iterator itListeSens;
            for (itListeSens=listeSens.begin(); itListeSens!=listeSens.end(); ++itListeSens) {
                cerr << *itListeSens << endl;
            }
        }
    }

    // static void rC(list<Cellule> inListeCellules) {
    //     list<Cellule>::iterator itListeCellules;
    //     for (itListeCellules=inListeCellules.begin(); itListeCellules!=inListeCellules.end(); ++itListeCellules) {
    //         cerr << itListeCellules->toInfo() << ".nbSens=" << itListeCellules->nbSens() << endl;
    //     }
    // }

    // static void rC(Orientation orient) {
    //     list<Cellule>::iterator itListeCellules;
    //     for (itListeCellules=inListeCellules.begin(); itListeCellules!=inListeCellules.end(); ++itListeCellules) {
    //         cerr << itListeCellules->toInfo() << ".nbSens=" << itListeCellules->nbSens() << endl;
    //     }
    // }

};


void recupererNbCombinaison(Plateau inPlateau) {

    list<Cellule> liste = Util::recupererListeCellulesPlateforme(inPlateau);
    // Util::printListeCellules(liste);
    double nbCombinaisons = Util::calculerCombinaison(liste);
    cerr << "plateau[" << inPlateau.name <<  "].nbCellules=" << liste. size() << ".combinaisonsPlateforme=" << nbCombinaisons << endl;

    liste = Util::recupererListeCellulesBord(inPlateau);
    // Util::printListeCellules(liste);
    nbCombinaisons = Util::calculerCombinaison(liste);
    cerr << "plateau[" << inPlateau.name <<  "].nbCellules=" << liste. size() << ".combinaisonsPlateforme=" << nbCombinaisons << endl;
    


}

class Simulateur {

    public:

    static int simuler(Plateau &plateau, bool debug) {
        plateau.isDebugSimulation = debug;
        int score = 0;
        for (int i = 0; i < plateau.robotCount; i++) {
            if (debug) cerr << plateau.robots[i].toString() << endl;
            int scoreRobot = 0;
            while(plateau.robots[i].isFonctionne) {
                
                Position pos = plateau.robots[i].position;

                // Pivoter
                if (plateau.robots[i].isPivotable(plateau.grille[pos.x][pos.y].flecheFusionne, debug)) {
                    plateau.robots[i].pivoter(plateau.grille[pos.x][pos.y].flecheFusionne, debug);
                    if (plateau.robots[i].determinerFonctionnement(plateau.grille[pos.x][pos.y], debug)) {
                        plateau.tracer(plateau.robots[i]);
                    } else {
                        if (debug) cerr << "robot[" << plateau.robots[i].id << "].MARQUE " << scoreRobot << " points" << endl;
                    }
                }

                // Avancer
                if(plateau.robots[i].isFonctionne) {
                    if (debug) cerr << "robot[" << plateau.robots[i].id << "].pos" << pos.toString() << "->pos";
                    pos = plateau.robots[i].avancer(debug);
                    if (debug) cerr << pos.toString() << endl;
                    score++;
                    scoreRobot++;
                    if (plateau.robots[i].determinerFonctionnement(plateau.grille[pos.x][pos.y], debug)) {
                        plateau.tracer(plateau.robots[i]);
                    } else {
                        if (debug) cerr << "robot[" << plateau.robots[i].id << "].MARQUE " << scoreRobot << " points" << endl;
                    }
                }
            }
        }
        for (int i = 0; i < plateau.robotCount; i++) {
            plateau.robots[i].reset();
        }
        return score;
    }

};

void analyser() {

    // Plateau simple = initPlateforme3x3();
    // cerr << simple.toString() << endl;
    // list<Cellule> liste = Util::recupererListeCellulesBord(simple);
    // Util::printListeCellules(liste);
    // double nbCombinaisons = Util::calculerCombinaison(liste);
    // cerr << nbCombinaisons << " combinaisons" << endl;

    // ExecutionTimer timer;
    // timer.start();
    // system_clock::time_point start = system_clock::now();
    // for (double y = 0; y < nbCombinaisons; y++) {
    //     auto end = system_clock::now();
    //     auto diff = duration_cast < std::chrono::milliseconds > (end - start).count();
    //     if (diff > 10000) {
    //         cerr << "no_comb=" << y << " : " << (y/nbCombinaisons*1000) << "%o" << endl;
    //         start = system_clock::now();
    //     }
    // }
    // timer.end();
    // cerr << "Time taken: " << timer.getString() << endl << endl;
}


void analyser2() {


    // Plateau simple = initPlateforme3x3();
    // cerr << simple.toString() << endl;
    // list<Cellule> liste = Util::recupererListeCellulesBord(simple);

    // Util::printListeSens(liste);
    // // Util::printListeCellules(liste);
    // double nbCombinaisons = Util::calculerCombinaison(liste);
    // cerr << "plateau[" << simple.name <<  "].nbCellules=" << liste.size() << ".combinaisonsBord=" << nbCombinaisons << endl;

}

list<Plateau*> initPlateaux() {

    list<Plateau*> listePlateaux;

    Plateau *plateau;
    Robot *robot;

    int idPlateau = 0;

    // Simple
    plateau = new Plateau("Simple");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\simple.txt");
    plateau->robotCount=1;
    int i = 0;
    robot = new Robot(i);
    robot->setPosition(3, 4);
    robot->setOrientation(2);
    robot->isFonctionne = true;
    plateau->robots[0]=*robot;
    plateau->init();
    listePlateaux.push_back(plateau);

    // SimpleDouble
    plateau = new Plateau("SimpleDouble");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\Simple_double.txt");
    plateau->robotCount=2;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(3, 2);
    robot->setOrientation(2);
    robot->isFonctionne = true;
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(14, 7);
    robot->setOrientation(8);
    robot->isFonctionne = true;
    plateau->robots[i]=*robot;
    plateau->init();
    listePlateaux.push_back(plateau);

    // Robot_buggue
    plateau = new Plateau("Robot_buggue");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\Robot_buggue.txt");
    plateau->robotCount=1;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(3, 4);
    robot->setOrientation(8);
    robot->isFonctionne = true;
    plateau->robots[i]=*robot;
    plateau->init();
    listePlateaux.push_back(plateau);

    // DoubleRobotsBuggués
    plateau = new Plateau("DoubleRobotsBuggués");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\DoubleRobotsBuggues.txt");
    plateau->robotCount=2;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(3, 4);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(14, 4);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    plateau->init();
    listePlateaux.push_back(plateau);

    // Plateforme3x3
    plateau = new Plateau("Plateforme3x3");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\Plateforme3x3.txt");
    plateau->robotCount=1;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(9, 5);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    plateau->init();
    listePlateaux.push_back(plateau);

    // RondPoint
    plateau = new Plateau("RondPoint");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\RondPoint.txt");
    plateau->robotCount=2;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(1, 1);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(15, 6);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    plateau->init();
    listePlateaux.push_back(plateau);

    // NeTombezPas
    plateau = new Plateau("NeTombezPas");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\NeTombezPas.txt");
    plateau->robotCount=1;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(1, 1);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    plateau->init();
    listePlateaux.push_back(plateau);

    // Portal
    plateau = new Plateau("Portal");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\Portal.txt");
    plateau->robotCount=1;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(10, 4);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    plateau->init();
    listePlateaux.push_back(plateau);

    // CodinGame
    plateau = new Plateau("CodinGame");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\CodinGame.txt");
    plateau->robotCount=4;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(7, 1);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 1);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(14, 5);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(7, 8);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    plateau->init();
    listePlateaux.push_back(plateau);

    // Plateformes3x3Multiple
    plateau = new Plateau("Plateformes3x3Multiple");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\Plateformes3x3Multiple.txt");
    plateau->robotCount=8;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(1, 1);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(6, 1);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(11, 1);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(14, 2);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(14, 6);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(1, 7);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(6, 7);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(11, 7);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    plateau->init();
    listePlateaux.push_back(plateau);

    // Plateforme9x9Quantique
    plateau = new Plateau("Plateforme9x9Quantique");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\Plateforme9x9Quantique.txt");
    plateau->robotCount=4;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(2, 2);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(16, 2);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(2, 7);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(16, 7);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    plateau->init();
    listePlateaux.push_back(plateau);

    // UnLongChemin
    plateau = new Plateau("UnLongChemin");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\UnLongChemin.txt");
    plateau->robotCount=1;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(0, 0);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    plateau->init();
    listePlateaux.push_back(plateau);

    // ChoixDifficile
    plateau = new Plateau("ChoixDifficile");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\ChoixDifficile.txt");
    plateau->robotCount=5;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(0, 0);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(0, 2);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(2, 4);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(2, 6);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(0, 8);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    plateau->init();
    listePlateaux.push_back(plateau);

    // LeMeilleurChemin
    plateau = new Plateau("LeMeilleurChemin");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\LeMeilleurChemin.txt");
    i = 0;
    robot = new Robot(i);
    robot->setPosition(1, 1);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 1);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(1, 8);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 8);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);

    // Hypersonic
    plateau = new Plateau("Hypersonic");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\Hypersonic.txt");
    i = 0;
    robot = new Robot(i);
    robot->setPosition(1, 0);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 0);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(1, 8);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 8);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);

    // HypersonicDeluxe
    plateau = new Plateau("HypersonicDeluxe");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\HypersonicDeluxe.txt");
    i = 0;
    robot = new Robot(i);
    robot->setPosition(1, 0);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 0);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(1, 8);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 8);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);


    // LesAnneauxDeSaturne
    plateau = new Plateau("LesAnneauxDeSaturne");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\LesAnneauxDeSaturne.txt");
    i = 0;
    robot = new Robot(i);
    robot->setPosition(1, 1);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(9, 1);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 2);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(1, 3);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 6);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(1, 7);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(9, 8);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 8);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);

    // GL_HF
    plateau = new Plateau("GL_HF");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\GL_HF.txt");
    i = 0;
    robot = new Robot(i);
    robot->setPosition(5, 0);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(13, 0);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 0);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(3, 2);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(13, 4);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 4);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(1, 5);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 5);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(5, 9);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(13, 9);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);

    // Croix
    plateau = new Plateau("Croix");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\Croix.txt");
    i = 0;
    robot = new Robot(i);
    robot->setPosition(1, 1);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(8, 1);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(10, 1);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 1);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(8, 3);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(10, 3);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(7, 7);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(11, 7);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(1, 8);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 8);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);

    // ADroite
    plateau = new Plateau("ADroite");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\ADroite.txt");
    i = 0;
    robot = new Robot(i);
    robot->setPosition(0, 2);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(0, 3);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(0, 7);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(0, 8);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);

    // WingsOfLiberty
    plateau = new Plateau("WingsOfLiberty");
    plateau->grille = getGrille("grilles\\WingsOfLiberty.txt");
    plateau->id = idPlateau++;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(1, 2);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(9, 2);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 2);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(5, 8);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(13, 8);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);

    // SensHoraire
    plateau = new Plateau("SensHoraire");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\SensHoraire.txt");
    i = 0;
    robot = new Robot(i);
    robot->setPosition(1, 1);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 1);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(4, 4);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(14, 5);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(1, 8);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 8);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);

    // Cellules
    plateau = new Plateau("Cellules");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\Cellules.txt");
    i = 0;
    robot = new Robot(i);
    robot->setPosition(1, 1);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 1);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(1, 8);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 8);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);

    // Bouclier
    plateau = new Plateau("Bouclier");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\Bouclier.txt");
    i = 0;
    robot = new Robot(i);
    robot->setPosition(8, 4);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(10, 4);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(4, 7);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(14, 7);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);

    // StarCraft
    plateau = new Plateau("StarCraft");
    plateau->id = idPlateau++;
    plateau->grille = getGrille("grilles\\StarCraft.txt");
    i = 0;
    robot = new Robot(i);
    robot->setPosition(2, 2);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(12, 2);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(2, 3);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(6, 5);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(6, 6);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(12, 6);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);

    // Xel
    plateau = new Plateau("Xel");
    plateau->grille = getGrille("grilles\\Xel.txt");
    plateau->id = idPlateau++;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(1, 0);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(18, 0);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(1, 9);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(18, 9);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);

    // 4Portes
    plateau = new Plateau("4Portes");
    plateau->grille = getGrille("grilles\\4Portes.txt");
    plateau->id = idPlateau++;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(8, 1);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(10, 1);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(8, 8);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(10, 8);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);

    // Confusion
    plateau = new Plateau("Confusion");
    plateau->grille = getGrille("grilles\\Confusion.txt");
    plateau->id = idPlateau++;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(18, 0);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(9, 4);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(9, 5);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(0, 9);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);

    // Bunker
    plateau = new Plateau("Bunker");
    plateau->grille = getGrille("grilles\\Bunker.txt");
    plateau->id = idPlateau++;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(1, 2);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 2);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(1, 7);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 7);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);

    // Divise
    plateau = new Plateau("Divise");
    plateau->grille = getGrille("grilles\\Divise.txt");
    plateau->id = idPlateau++;
    i = 0;
    robot = new Robot(i);
    robot->setPosition(1, 0);
    robot->setOrientation(2);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 0);
    robot->setOrientation(4);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(1, 8);
    robot->setOrientation(1);
    plateau->robots[i]=*robot;
    i++;
    robot = new Robot(i);
    robot->setPosition(17, 8);
    robot->setOrientation(8);
    plateau->robots[i]=*robot;
    plateau->robotCount=i+1;
    plateau->init();
    listePlateaux.push_back(plateau);

    return listePlateaux;

}

struct Strat {
    int id;
    string name;
    Strat(int inId, string inName) {
        id= inId;
        name = inName;
    }
};

class Commande {

    private:
    string commande;

    public:
    string getCommande() {
        return commande;
    }

    void add(Position inPos, Orientation inOrient) {
        ostringstream ostr;
        if (commande != "") {
            ostr << " ";
        }
        ostr << inPos.x << " " << inPos.y << " " << Orientation::getString(inOrient);
        commande += ostr.str();
    }

};

class Score {
    
    public:
    int id;
    string name;
    int score;
    string time;
    Commande commande;

    Score(Strat strat, int inScore, string inTime, Commande inCommande) {
        id= strat.id;
        name = strat.name;
        score = inScore;
        time = inTime;
        commande = inCommande;
    }

    string toString() {
        ostringstream ostr;
        ostr << "strat(" << name << ").time[" << time << "]"<< ".score=" << score << ".commande=\"" << commande.getCommande() << "\"";
        return ostr.str();
    }

};

struct ScoreComparator {
	bool operator ()(const Score & score1, const Score & score2) {
		if(score1.score > score2.score) {
			return true;
        } else {
            return false;
        }
	}
};

class Stats {

    public:
    int id;
    int plateauId;
    string plateauName;
    list<Score> listeScores;

    string toString() {
        ostringstream ostr;
        listeScores.sort(ScoreComparator());
        for (auto &score : listeScores) {
            ostr << "plateau(" << plateauId << "-" << plateauName << ")." << score.toString() << endl;
        }
        return ostr.str();
    }

    Stats() {}
    Stats(Plateau *plateau) {
        plateauId = plateau->id;
        plateauName = plateau->name;
    }

    Score getScoresIndex(int value) {
        if (listeScores.size() > value) {
            list<Score>::iterator it = std::next(listeScores.begin(), value);
            return *it;
        }
    }

};

class Base {

    public:
    int id;
    string name;
    Plateau plateau;
    Commande commande;

    string toString() {
        ostringstream ostr;
        ostr << "base(" << name << ")";
        return ostr.str();
    }

    Base() {}
    Base(Plateau &inPlateau) {
        plateau = inPlateau;
    }

    poserImpasses() {
        for (int y = 0; y < plateau.tableHeight; y++) {
            for (int x = 0; x < plateau.tableWidth; x++) {
                if (plateau.grille[x][y].isPlateforme
                        && !plateau.grille[x][y].fleche.exist()
                        && plateau.grille[x][y].axes.nbSens() == 1) {
                    // cerr << "poserImpasse" << endl;
                    // cerr << plateau.grille[x][y].toInfo() << endl;
                    plateau.grille[x][y].flecheAjoute.set(plateau.grille[x][y].axes);
                    plateau.grille[x][y].mettreAJourFlecheFusionne();
                    Position pos(x, y);
                    commande.add(pos, plateau.grille[x][y].flecheAjoute);
                    // cerr << plateau.grille[x][y].toInfo() << endl;
                }
            }
        }
    }

    poserImpassesEtAngles() {
        for (int y = 0; y < plateau.tableHeight; y++) {
            for (int x = 0; x < plateau.tableWidth; x++) {
                if (plateau.grille[x][y].isPlateforme
                        && !plateau.grille[x][y].fleche.exist()) {
                    // Impasses
                    if (plateau.grille[x][y].axes.nbSens() == 1) {
                        // cerr << "poserImpasse" << endl;
                        // cerr << plateau.grille[x][y].toInfo() << endl;
                        plateau.grille[x][y].flecheAjoute.set(plateau.grille[x][y].axes);
                        plateau.grille[x][y].mettreAJourFlecheFusionne();
                        Position pos(x, y);
                        commande.add(pos, plateau.grille[x][y].flecheAjoute);
                        // cerr << plateau.grille[x][y].toInfo() << endl;
                    }
                    // Angles
                    if (plateau.grille[x][y].axes.nbSens() == 2) {
                        if (plateau.grille[x][y].axes.isNotOppose() ) {
                            // cerr << "plateau." << plateau.name << ".isNotOppose " ;
                            // cerr << plateau.grille[x][y].toInfo() << endl;

                            Orientation o(plateau.grille[x][y].axes.getSensHoraireAngle());

                            plateau.grille[x][y].flecheAjoute.set(o);
                            plateau.grille[x][y].mettreAJourFlecheFusionne();
                            Position pos(x, y);
                            commande.add(pos, plateau.grille[x][y].flecheAjoute);
                            // cerr << plateau.grille[x][y].toInfo() << endl;
                        }
                    }
                }
            }
        }
    }

    poserImpassesEtAngles2() {
        for (int y = 0; y < plateau.tableHeight; y++) {
            for (int x = 0; x < plateau.tableWidth; x++) {
                if (plateau.grille[x][y].isPlateforme
                        && !plateau.grille[x][y].fleche.exist()) {
                    // Impasses
                    if (plateau.grille[x][y].axes.nbSens() == 1) {
                        // cerr << "poserImpasse" << endl;
                        // cerr << plateau.grille[x][y].toInfo() << endl;
                        plateau.grille[x][y].flecheAjoute.set(plateau.grille[x][y].axes);
                        plateau.grille[x][y].mettreAJourFlecheFusionne();
                        Position pos(x, y);
                        commande.add(pos, plateau.grille[x][y].flecheAjoute);
                        // cerr << plateau.grille[x][y].toInfo() << endl;
                    }
                    // Angles
                    if (plateau.grille[x][y].axes.nbSens() == 2) {
                        if (plateau.grille[x][y].axes.isNotOppose() ) {
                            // cerr << "plateau." << plateau.name << ".isNotOppose " ;
                            // cerr << plateau.grille[x][y].toInfo() << endl;

                            Orientation o(plateau.grille[x][y].axes.getSensHoraireAngle2());

                            plateau.grille[x][y].flecheAjoute.set(o);
                            plateau.grille[x][y].mettreAJourFlecheFusionne();
                            Position pos(x, y);
                            commande.add(pos, plateau.grille[x][y].flecheAjoute);
                            // cerr << plateau.grille[x][y].toInfo() << endl;
                        }
                    }
                }
            }
        }
    }

    poserImpassesEtBords1() {
        for (int y = 0; y < plateau.tableHeight; y++) {
            for (int x = 0; x < plateau.tableWidth; x++) {
                if (plateau.grille[x][y].isPlateforme
                        && !plateau.grille[x][y].fleche.exist()) {
                    // Impasses
                    if (plateau.grille[x][y].axes.nbSens() == 1) {
                        // cerr << "poserImpasse" << endl;
                        // cerr << plateau.grille[x][y].toInfo() << endl;
                        plateau.grille[x][y].flecheAjoute.set(plateau.grille[x][y].axes);
                        plateau.grille[x][y].mettreAJourFlecheFusionne();
                        Position pos(x, y);
                        commande.add(pos, plateau.grille[x][y].flecheAjoute);
                        // cerr << plateau.grille[x][y].toInfo() << endl;
                    }
                    // Angles
                    if (plateau.grille[x][y].axes.nbSens() == 2) {
                        if (plateau.grille[x][y].axes.isNotOppose() ) {
                            // cerr << "plateau." << plateau.name << ".isNotOppose " ;
                            // cerr << plateau.grille[x][y].toInfo() << endl;

                            Orientation o(plateau.grille[x][y].axes.getSensHoraireAngle());

                            plateau.grille[x][y].flecheAjoute.set(o);
                            plateau.grille[x][y].mettreAJourFlecheFusionne();
                            Position pos(x, y);
                            commande.add(pos, plateau.grille[x][y].flecheAjoute);
                            // cerr << plateau.grille[x][y].toInfo() << endl;
                        }
                    }
                    // Flancs
                    if (plateau.grille[x][y].axes.nbSens() == 3) {
                        // if (plateau.grille[x][y].axes.isNotOppose() ) {
                            // cerr << "plateau." << plateau.name << ".nbSens3 " ;
                            // cerr << plateau.grille[x][y].toInfo() << endl;

                            Orientation o(plateau.grille[x][y].axes.getSensHoraireFlanc());

                            plateau.grille[x][y].flecheAjoute.set(o);
                            plateau.grille[x][y].mettreAJourFlecheFusionne();
                            Position pos(x, y);
                            commande.add(pos, plateau.grille[x][y].flecheAjoute);
                            // cerr << plateau.grille[x][y].toInfo() << endl;
                        // }
                    }

                }
            }
        }
    }

        poserImpassesEtBords2() {
        for (int y = 0; y < plateau.tableHeight; y++) {
            for (int x = 0; x < plateau.tableWidth; x++) {
                if (plateau.grille[x][y].isPlateforme
                        && !plateau.grille[x][y].fleche.exist()) {
                    // Impasses
                    if (plateau.grille[x][y].axes.nbSens() == 1) {
                        // cerr << "poserImpasse" << endl;
                        // cerr << plateau.grille[x][y].toInfo() << endl;
                        plateau.grille[x][y].flecheAjoute.set(plateau.grille[x][y].axes);
                        plateau.grille[x][y].mettreAJourFlecheFusionne();
                        Position pos(x, y);
                        commande.add(pos, plateau.grille[x][y].flecheAjoute);
                        // cerr << plateau.grille[x][y].toInfo() << endl;
                    }
                    // Angles
                    if (plateau.grille[x][y].axes.nbSens() == 2) {
                        if (plateau.grille[x][y].axes.isNotOppose() ) {
                            // cerr << "plateau." << plateau.name << ".isNotOppose " ;
                            // cerr << plateau.grille[x][y].toInfo() << endl;

                            Orientation o(plateau.grille[x][y].axes.getSensHoraireAngle2());

                            plateau.grille[x][y].flecheAjoute.set(o);
                            plateau.grille[x][y].mettreAJourFlecheFusionne();
                            Position pos(x, y);
                            commande.add(pos, plateau.grille[x][y].flecheAjoute);
                            // cerr << plateau.grille[x][y].toInfo() << endl;
                        }
                    }
                    // Flancs
                    if (plateau.grille[x][y].axes.nbSens() == 3) {
                        // if (plateau.grille[x][y].axes.isNotOppose() ) {
                            // cerr << "plateau." << plateau.name << ".nbSens3 " ;
                            // cerr << plateau.grille[x][y].toInfo() << endl;

                            Orientation o(plateau.grille[x][y].axes.getSensHoraireFlanc2());

                            plateau.grille[x][y].flecheAjoute.set(o);
                            plateau.grille[x][y].mettreAJourFlecheFusionne();
                            Position pos(x, y);
                            commande.add(pos, plateau.grille[x][y].flecheAjoute);
                            // cerr << plateau.grille[x][y].toInfo() << endl;
                        // }
                    }

                }
            }
        }
    }


};

#include <cstdlib>

class Random {

    public:
    int id;
    string name;
    list<Cellule> listeCellules;
    
    static list<Cellule> randomize(list<Cellule> inListeCellules, int nbrRandom) {

        list<Cellule> listeCellulesSelectionnes;

        int nbrCellules = inListeCellules.size(); 

        // https://www.bogotobogo.com/cplusplus/RandomNumbers.php
        srand((int)time(0));
        for(int i = 0; i < nbrRandom; i++) {

                int r = (rand() % nbrCellules) + 1;
                cout << r << endl;
                
                // listeCellulesSelectionnes.push_back();


        }
        

        // for (auto &cellule : inListeCellules) {

        // }
        return listeCellulesSelectionnes;
    }

    static list<Cellule> creerListeCellules(Plateau &plateau) {
        list<Cellule> listeCellules;
        for (int y = 0; y < plateau.tableHeight; y++) {
            for (int x = 0; x < plateau.tableWidth; x++) {
                if (plateau.grille[x][y].isPlateforme 
                        && !plateau.grille[x][y].flecheFusionne.exist()) {
                    listeCellules.push_back(plateau.grille[x][y]);
                }
            }
        }
        return listeCellules;
    }

        static void majGrille(Plateau &plateau, list<Cellule> inListeCellules) {
        for (auto &cellule : inListeCellules) {
            plateau.grille[x][y].flecheAjoute.set(cellule.flecheAjoute);
        }
    }
};

class Jeux {

    public:

    static void simulerAVide(Plateau* plateau, Stats &stats, bool debug) {
        const Strat strat(0, "Vide");
        int scorePlateau = 0;
        if (debug) cerr << plateau->toString() << endl;
        ExecutionTimer timer;
        timer.start();
        scorePlateau = Simulateur::simuler(*plateau, debug);
        timer.end();
        if (debug) cerr << "scorePlateau=" << scorePlateau << endl << endl;
        // Stats
        Commande commande;
        Score score(strat, scorePlateau, timer.getString(), commande);
        stats.listeScores.push_back(score);
    }

    static void simulerBaseImpasse(Plateau* plateau, Stats &stats, bool debug) {
        const Strat strat(1, "Impasses");
        int scorePlateau = 0;
        // if (debug) cerr << plateau->toString() << endl;
        ExecutionTimer timer;
        timer.start();
        Base *base0;
        base0 = new Base(*plateau);
        base0->poserImpasses();
        if (debug) cerr << base0->plateau.toString() << endl;
        scorePlateau = Simulateur::simuler(base0->plateau, debug);
        timer.end();
        if (debug) cerr << "scorePlateau=" << scorePlateau << endl << endl;
        // Stats
        Commande commande;
        Score score(strat, scorePlateau, timer.getString(), base0->commande);
        stats.listeScores.push_back(score);
    }

    static void simulerBaseImpasseEtAngles2(Plateau* plateau, Stats &stats, bool debug) {
        const Strat strat(1, "Impasses+Angles2");
        int scorePlateau = 0;
        // if (debug) cerr << plateau->toString() << endl;
        ExecutionTimer timer;
        timer.start();
        Base *base0;
        base0 = new Base(*plateau);
        base0->poserImpassesEtAngles2();
        if (debug) cerr << base0->plateau.toString() << endl;
        scorePlateau = Simulateur::simuler(base0->plateau, debug);
        timer.end();
        if (debug) cerr << "scorePlateau=" << scorePlateau << endl << endl;
        // Stats
        Commande commande;
        Score score(strat, scorePlateau, timer.getString(), base0->commande);
        stats.listeScores.push_back(score);
    }

        static void simulerBaseImpasseEtAngles(Plateau* plateau, Stats &stats, bool debug) {
        const Strat strat(1, "Impasses+Angles");
        int scorePlateau = 0;
        // if (debug) cerr << plateau->toString() << endl;
        ExecutionTimer timer;
        timer.start();
        Base *base0;
        base0 = new Base(*plateau);
        base0->poserImpassesEtAngles();
        if (debug) cerr << base0->plateau.toString() << endl;
        scorePlateau = Simulateur::simuler(base0->plateau, debug);
        timer.end();
        if (debug) cerr << "scorePlateau=" << scorePlateau << endl << endl;
        // Stats
        Commande commande;
        Score score(strat, scorePlateau, timer.getString(), base0->commande);
        stats.listeScores.push_back(score);
    }

    static void simulerBaseImpasseEtBords1(Plateau* plateau, Stats &stats, bool debug) {
        const Strat strat(1, "Impasses+Bords1");
        int scorePlateau = 0;
        // if (debug) cerr << plateau->toString() << endl;
        ExecutionTimer timer;
        timer.start();
        Base *base0;
        base0 = new Base(*plateau);
        base0->poserImpassesEtBords1();
        if (debug) cerr << base0->plateau.toString() << endl;
        scorePlateau = Simulateur::simuler(base0->plateau, debug);
        timer.end();
        if (debug) cerr << "scorePlateau=" << scorePlateau << endl << endl;
        // Stats
        Commande commande;
        Score score(strat, scorePlateau, timer.getString(), base0->commande);
        stats.listeScores.push_back(score);
    }

    static void simulerBaseImpasseEtBords2(Plateau* plateau, Stats &stats, bool debug) {
        const Strat strat(1, "Impasses+Bords2");
        int scorePlateau = 0;
        // if (debug) cerr << plateau->toString() << endl;
        ExecutionTimer timer;
        timer.start();
        Base *base0;
        base0 = new Base(*plateau);
        base0->poserImpassesEtBords2();
        if (debug) cerr << base0->plateau.print() << endl;
        scorePlateau = Simulateur::simuler(base0->plateau, debug);
        timer.end();
        if (debug) cerr << "scorePlateau=" << scorePlateau << endl << endl;
        // Stats
        Commande commande;
        Score score(strat, scorePlateau, timer.getString(), base0->commande);
        stats.listeScores.push_back(score);
    }

};



int main() {

    // HANDLE  HCmd = GetStdHandle(STD_OUTPUT_HANDLE); //creer un handle de sortie
    // SetConsoleTextAttribute(HCmd, FOREGROUND_RED|FOREGROUND_INTENSITY);  // couleur rouge


    list<Stats> listeStats;
    list<Plateau*> listePlateaux = initPlateaux();

    Stats *stats;
    for (auto &plateau : listePlateaux) {
        if (plateau->id == 28) {
            stats = new Stats(plateau);
            Jeux::simulerAVide(plateau, *stats, false);

            Plateau::reInitPlateau(*plateau);
            Jeux::simulerBaseImpasse(plateau, *stats, false);

            Plateau::reInitPlateau(*plateau);
            Jeux::simulerBaseImpasseEtAngles(plateau, *stats, false);

            Plateau::reInitPlateau(*plateau);
            Jeux::simulerBaseImpasseEtAngles2(plateau, *stats, false);

            Plateau::reInitPlateau(*plateau);
            Jeux::simulerBaseImpasseEtBords1(plateau, *stats, false);

            Plateau::reInitPlateau(*plateau);
            Jeux::simulerBaseImpasseEtBords2(plateau, *stats, true);

            list<Cellule> listeCellules = Random::creerListeCellules(*plateau);
            Random::randomize(listeCellules, 10);

            listeStats.push_back(*stats);
        }
    }
    
    int scoreGlobal = 0;
    for (auto &stats : listeStats) {
        cerr << stats.toString() << endl;
        scoreGlobal+= stats.getScoresIndex(0).score;
    }
    cerr << "scoreGlobal=" << scoreGlobal << endl;
    // cout << "" << endl;//U 1 1 R 2 2 D 3 3 L" << endl;

    return 0;
}