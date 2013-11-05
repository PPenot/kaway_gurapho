#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "graph.h"
#include "tour.h"
#include <string.h>
#include <string>
#include <cstdlib>
#include <algorithm>

using namespace std;

void parseQueryString(string qs, vector<string>& args, vector<string>& values)
{
	int i = qs.find_first_of("=");
	int j = qs.find_first_of("&");

	do
	{
		args.push_back(qs.substr(0,i));
		values.push_back(qs.substr(i+1,j-i-1));

		qs.erase(0,j+1);

		i = qs.find_first_of("=");
		j = qs.find_first_of("&");

		if(j==string::npos) j=qs.size();
	}
	while(i!=string::npos);

}

template <class T>
bool isIn(vector<T> t, T val)
{
    int i=0;
    bool boule = false;
    int size = t.size();
    while(i<size && !boule)
    {
        boule = (t[i]== val);
        i++;
    }
    return boule;
}

bool createGraph(Graph& g, vector<string>& args, vector<string>& values)
{
    try
    {
        int i=0;
        int size = args.size();
        int id;
        float value;
        vector<int> ids;

        while(i<size)
        {
            if(args[i].find("node")!=string::npos)
            {
                id = atoi(args[i].substr(4).c_str());
                if(id==0) return false;
                if(isIn(ids,id)) return false;
                ids.push_back(id);
                g.addNode(id);

                if(values[i][0]!='u' && atof(values[i].c_str())>0)
                    g.setNodeValue(id,atof(values[i].c_str()));
                else if(values[i][0]=='u' && values[i].size()==1)
                {
                    
                }
                else
                    return false;
            }
            else if(args[i].find("edge")!=string::npos)
            {
                int idstart = atoi((args[i].substr(args[i].find_last_of('e')+1,args[i].find_first_of('-') - args[i].find_last_of('e')-1)).c_str());
                if(id==0 || !isIn(ids,idstart)) return false;

                int idend = atoi((args[i].substr(args[i].find_first_of('-') +1)).c_str());
                if(idend==0 || !isIn(ids,idend)) return false;

                try
                {
                    g.addEdge(idstart,idend);

                    if(atof(values[i].c_str())>0)
                        g.setEdgeValue(idstart,idend,atof(values[i].c_str()));
                    else return false;
                }
                catch(...)
                {
                    
                }
            }

            i++;
        }
    }
    catch(...)
    {
        return false;
    }

    return true;
}

int main(int argc, char** argv)
{
    vector<string> args(0);
    vector<string> values(0);

    char* test = "node1=13&nodet2=34&node3=u&node4=u&edge1-2=1&edge2-1=1&edge2-3=1&edge3-2=1&edge3-4=1&edge4-3=1&edge4-1=1&edge1-4=1&edge1-3=2&edge3-1=2&edge4-2=2&edge2-4=2&start=3&goal=3";
    //strcpy(test,"http://localhost/cgi-bin/little?node1=u&node2=u&node3=u&node4=u&edge1-2=1&edge2-1=1&edge2-3=1&edge3-2=1&edge3-4=1&edge4-3=1&edge4-1=1&edge1-4=1&edge1-3=2&edge3-1=2&edge4-2=2&edge2-4=2&start=2&goal=2");
    char* tachatte =  getenv("QUERY_STRING");

    string query_string(test);
    parseQueryString(query_string,args,values);

    /*
    for(int i=0;i<args.size();i++)
    {
            cout << args[i] << ":" << values[i] << endl;
    }
    */

    ofstream file("zobite.txt",ios::out | ios::trunc);


    Graph g(false,true);

    bool boule = createGraph(g,args,values);


    Tour response;

    int start = atoi(values[values.size()-1].c_str());

    if(boule)
        response = g.little(start);

    boule = boule && response.isCircuit() && ((int)g.getOrder()==(int)response.size());



    


    cout << "Content-Type:text/xml";
    cout << "<little>";
    string status;
    (boule) ? status="OK" : status="Aucune tournee trouvee";
    cout << "<status>" << status << "</status>";
    cout << "<cost>" << response.getCost() << "</cost>";
    cout << "<tour>";

    int* tab = response.toTab();

    for(int i=0;i<response.length();i++)
    {
        cout << "<id>" << tab[i] << "</id>";
    }

    cout << "</tour>";
    cout << "</little>";

    

    file.close();

    return 0;
}
