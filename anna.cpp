#include<iostream>
#include<bits/stdc++.h>
#include<sys/stat.h> 
#include<fstream>
#include<stdlib.h>

using namespace std;

int main(int argc, char const *argv[])
{
    int T = 4;
    vector < vector <int> > v;
    for (int i = 0; i < 4; i++)
    {
        vector <int> g;
        for (int j = 0; j < 4; j++)
        {
            g.push_back(j);
        }
        v.push_back(g);
    }
    cout << v.size();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << " " << v[i][j] << "|" ;
        }
        cout << endl;
    }

    v.clear();
    cout << v.size() << endl;
    for (int i = 0; i < 4; i++)
    {
        vector <int> g;
        for (int j = 0; j < 4; j++)
        {
            g.push_back(j * j);
        }
        v.push_back(g);
        cout << v.size() << endl;
        cout << v[0][2] << "AA" << endl;
    }


    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << " " << v[i][j] << "|" ;
        }
        cout << endl;
    }

    return 0;
}
