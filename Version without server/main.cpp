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
            sum += pow(summ_rec_2(bool_array, bound_array,value_array,q_elem, i), 2) ; // one isolated graph
            //bool_array[i] = false;
        }
    }
    return sum;
}


int main(int argc, const char * argv[]) {
    
    cout << "Hello! This is analysator of city's network system\n";
    
    const std::regex regular_for_way("([\\w]+)(,)([\\w]+)");
    //std::regex regular_for_value(R"(\W{0,}(\w{1,})\W{0,}:\W{0,}(\d{1,})\W{0,})");
    std::smatch m;
    const string for_input = "adsds,b";
    if (std::regex_search(for_input, m, regular_for_way)){
        cout << "yep\n";
        //cout << m.size();
        cout << endl << m[1] << endl;
        cout << m[2] << endl;
        cout << m[3] << endl;

    }
    
    
    int quantity_of_nodes = 0;
    vector< vector<int> > g;

    map <string, int> mp, tmp_map;
    
    std::string line;
     
        std::ifstream in("data.txt");
        if (in.is_open())
        {
            while (getline(in, line) && line != "}")
            {
                if(std::regex_search(line, m, regular_for_way))
                {
                    //IF necessary creating new map object
                    auto tmp_map = mp.find((string) m[1]);
                    if (tmp_map == mp.end())
                    {
                        mp[(string) m[1]] = quantity_of_nodes++;
                        g.emplace_back();
                    }
                    tmp_map = mp.find((string) m[2]);
                    if (tmp_map == mp.end())
                    {
                        mp[(string) m[2]] = quantity_of_nodes++;
                        g.emplace_back();
                    }
                    //Creating bind between two network buildings
                    g[mp[(string) m[1]]].push_back(mp[(string) m[2]]);
                    g[mp[(string) m[2]]].push_back(mp[(string) m[1]]);
                    
                    
                }
            }
            getline(in, line);
            if (line != "{")
            {
                throw "ERROR";
            }
            
            //Creating of arrayn of nodes' value
            std::vector<int> array_of_net_value(quantity_of_nodes);
            while (getline(in, line) && line != "}")
            {
                int net_num = mp[(string) m[1]];
                line.erase(0,2);
                int net_val = stoi(line);
                array_of_net_value[net_num] = net_val;
                //s.Remove(3, 3)
                
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
                node_importance[i] = summ_rec(array_visit, g, array_of_net_value, quantity_of_nodes);
                array_visit[i] = true;
                
                /*
                int sum_i = 0;
                for( int j = 0; j < g[i].size(); j++)
                {
                    
                    sum_i += array_of_net_value[g[i][j]];
                }
                sum_i += array_of_net_value[i];
                sum_i = pow(sum_i, g[i].size());
                node_importance[i] = sum_i;*/
            }
            
            
            
            
            cout << "\n";
            
            
            
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
    
    return 0;
}
