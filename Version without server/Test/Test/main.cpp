//
//  main.cpp
//  Nets
//
//  Created by Ivan on 14.04.2021.
//

#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <fstream>
#include <regex>
#include <cstdlib>

using namespace std;

//recursive func, that works with one isolated graph and summs all of its' node values
int summ_rec_2(vector<bool> &bool_array, vector< vector<int> > bound_array, std::vector<int> value_array, int q_elem, int indx)
{
    int sum = 0;
    for(int j = 0; j < bound_array[indx].size() ; j ++ )
    {
        if (bool_array[bound_array[indx][j]] == true){
            bool_array[bound_array[indx][j]] = false;
            sum += summ_rec_2(bool_array, bound_array, value_array, q_elem, bound_array[indx][j]);
            
        }
    }
    return sum + value_array[indx];
}

// function, that works with the fist coordinate of vector of vectors, that keeps bounds of nodes and runs the recursive func
int summ_rec(vector<bool> bool_array, vector< vector<int> > bound_array, std::vector<int> value_array, int q_elem)
{
    int sum = 0;
    for(int i = 0; i < q_elem ; i ++ )
    {
        if (bool_array[i] == true){
            bool_array[i] = false;
            sum += pow(summ_rec_2(bool_array, bound_array,value_array,q_elem, i), 2) ; // one isolated graph -- the summ_rec() func changes the boolean array, hense it will close the nodes we wisited => summ_rec() in one iteration summs one isolated graph
            //bool_array[i] = false;
        }
    }
    return sum;
}


int main(int argc, const char * argv[]) {
    try{
    cout << "Hello! This is analysator of city's network system\n";
    
        //creating regular prototypes for first and second part of file
    const std::regex regular_for_way("([\\w])(,)([\\w])");// smthng like a,b
    const std::regex regular_for_value("([\\w])(:)([\\d]+)");// smthng like a:23
    std::smatch m;//varaible to construct a determinition of our line in a file and to devide it by a groups
   
    
    int quantity_of_nodes = 0;
        
    //main vector that includes in a first digit -- number of node (that was created by a map), array of numbers of neighbors -- in a second
    vector< vector<int> > g;

    //map, that creates a display from node's name to it's unique value
    map <string, int> mp, tmp_map;
    
    std::string line;
        
     //file opening
        std::ifstream in("/Users/ivan/Desktop/АСВК/Version without server/data.txt");
        if (in.is_open())
        {
            //reading of a new line from a file
            //We will create a map object, which will provide the compliance between the node name and its' unique number
            while (getline(in, line) && line != "}")
            {
                if(std::regex_search(line, m, regular_for_way))
                {
                    //IF necessary creating new map object
                    auto tmp_map = mp.find( string(m.str(1)));
                    if (tmp_map == mp.end())
                    {
                        //if the letter we read is new, we create new map compliance and insert new elem to vector
                        cout << m.str(1);
                        mp[string( m.str(1))] = quantity_of_nodes++;
                        g.emplace_back();
                    }
                    tmp_map = mp.find((string) m[3]);
                    if (tmp_map == mp.end())
                    {
                        cout << m.str(3);
                        mp[(string) m[3]] = quantity_of_nodes++;
                        g.emplace_back();
                    }
                    //Creating bind between two network buildings
                    g[mp[(string) m[1]]].push_back(mp[(string) m[3]]);
                    g[mp[(string) m[3]]].push_back(mp[(string) m[1]]);
                    
                    
                }
                else
                {
                    if (line != "{" && line != "}" && line != " " && line != "\n")
                        throw ("SYNTAX ERROR\n");
                }
            }
            getline(in, line);
            if (line != "{")
            {
                throw ("SYNTAX ERROR\n");
            }
            
            //Creating of arrayn of nodes' quantity of people
            std::vector<int> array_of_net_value(quantity_of_nodes);
            while (getline(in, line) && line != "}")
            {
                if(std::regex_search(line, m, regular_for_value))
                {
                    int net_num = mp[(string) m[1]];
                    //line.erase(0,2);
                    int net_val = stoi(m.str(3));
                    array_of_net_value[net_num] = net_val;
                    //s.Remove(3, 3)
                }
                else
                {
                    if (line != "{" && line != "}" && line != " " && line != "\n")
                        throw ("SYNTAX ERROR\n");
                }
                
            }
            
            
            //boolean array to save visited nodes
            std::vector<bool> array_visit(quantity_of_nodes);
            for (int i = 0; i < quantity_of_nodes; i ++)
            {
                array_visit[i] = true;
            }
            
            
            
            //Creating of arrayn of nodes' importance
            std::vector<int> node_importance(quantity_of_nodes);
            for (int i = 0; i < quantity_of_nodes; i ++)
            {
                //summing of all node's neighbours and it's value
                array_visit[i] = false;
                //throw the recursive algorythm we will summ all the cases when one of buildings will stop working, b
                node_importance[i] = summ_rec(array_visit, g, array_of_net_value, quantity_of_nodes);
                array_visit[i] = true;//restore it
              
            }
            
            
            
            
            cout << "\n";
            
            
            //searching for the worst node
            std::vector<int>::iterator worst;
            worst = std::min_element(node_importance.begin(), node_importance.end());
            int result = distance(node_importance.begin(), worst);
            
            //Searching elem in a map
            map <string, int> :: iterator it = mp.begin();
            std::ofstream oFile ( "Result.txt" ) ; //output file
            for ( int i = 0; it != mp.end(); it++, i++)
            {
                if (it->second == result)
                {
                    oFile << it->first;
                    cout << "The worst network building to remove is:" << it->first << endl;
                }
            }
            
        }
        in.close();     // закрываем файл
        
        //std::cout << mp['A'] << std::endl;
    }
    catch (const char* str)
    {
        cout << str << endl;
    }
    catch (...)
    {
        cout << "Undefined error\n";
    }
    return 0;
}
