#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <cstdlib>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <fstream>
#include <time.h>

using namespace std;

// g++ -std=c++11 paper-folding-curve.cpp -o paper-folding-curve

ofstream myfile;
bool file = false;
long compteurProgressBar = 0;

class CLParser
{
public:

    CLParser(int argc_, char * argv_[],bool switches_on_=false);
    ~CLParser(){}

    string get_arg(int i);
    string get_arg(string s);
    bool isExist(string s);

private:

    int argc;
    vector<string> argv;

    bool switches_on;
    map<string,string> switch_map;
};

CLParser::CLParser(int argc_, char * argv_[],bool switches_on_)
{
    argc=argc_;
    argv.resize(argc);
    copy(argv_,argv_+argc,argv.begin());
    switches_on=switches_on_;

    //map the switches to the actual
    //arguments if necessary
    if (switches_on)
    {
        vector<string>::iterator it1,it2;
        it1=argv.begin();
        it2=it1+1;

        while (true)
        {
            if (it1==argv.end()) break;
            if (it2==argv.end()) break;

            if ((*it1)[0]=='-')
                switch_map[*it1]=*(it2);

            it1++;
            it2++;
        }
    }
}

string CLParser::get_arg(int i)
{
    if (i>=0&&i<argc)
        return argv[i];

    return "";
}

string CLParser::get_arg(string s)
{
    if (!switches_on) return "";

    if (switch_map.find(s)!=switch_map.end())
        return switch_map[s];

    return "";
}

bool CLParser::isExist(string s)
{
    if (switches_on) return true;

    //if (switch_map.find(s)!=switch_map.end())
    //    return true;

    return false;
}

long bin(int order) {
    long c=0;
    long out=0;
    while(c < order) {
      out+=pow(2, c);
      c+=1;
    }
    return out;
}

void curve(int order, long start, long current, long end, long modulo) {
    if (current<=end+1) {
        if (current % modulo) {
          compteurProgressBar++;
          string value = "1";
          if (long(ceil(current / modulo)) % 2) {
            value = "0";
          }
          if (file) {
            myfile << value;
            //string progress = "\r5";
            //cout << "\r";
            //cout << compteurProgressBar << " \r";
            //std::cout.flush();

          } else {
            cout << value;
          }
        } else {
           curve(order-1, start, current, end, modulo*2);
        }
    }
}

void path(int order, long s, long e) {
    long start=s+1;
    long current=s+1;
    long end=e;
    long modulo=2;
    if (file) {
      cout << (end-start)+1;
      cout << " curves in progress..." << endl;
      //cout << "0" << endl;
    }
    while(s <= e) {
      curve(order, start, current, end, modulo);
      current+=1;
      s+=1;
    }
    if(!file) cout << endl;
}

int main(int argc, char* argv[]) {

    CLParser cmd_line(argc,argv,true);

        //int order = atoi(argv[1]);
    //nb=bin(order);
    //cout << nb << endl;

    string temp;
    bool tempExist;

    int order;
    temp=cmd_line.get_arg("--order");
    if (temp!="") {
      order = stoi(temp);
    }

    long nb;
    nb=bin(order);
    //cout << nb << endl;
/*
    tempExist=cmd_line.isExist("--number");
    cout << tempExist << endl;
    if (tempExist) {
      cout << nb << endl;
    }
*/
    long start;
    temp=cmd_line.get_arg("--start");
    if (temp!="") {
      start = stol(temp);
      if (start<0) start=0;
    } else {
      start = 0;
    }

    long end;
    temp=cmd_line.get_arg("--end");
    if (temp!="") {
      end = stol(temp);
      if (end>nb) end=nb;
    } else {
      end = nb;
    }

    temp=cmd_line.get_arg("--output");
    if (temp!="") {
      myfile.open (temp);
      file = true;
    }

    clock_t tStart = clock();
    /* Do your stuff here */

    path(order, start, end);

    if(file) {
      myfile.close();
      printf("Time taken: %.3fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    }
    return 0;
}
