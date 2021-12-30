//
//  main.cpp
//  Nets
//
//  Created by Ivan on 14.04.2021.
//
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <poll.h>
#include <math.h>
#include <signal.h>
#include <netdb.h>
#include <cstdint>
#include <functional>
#include <thread>
#include <list>
#include <cstdio>
#include <poll.h>
#include <regex>

#define MAX_QUEUE 5
#define MEM_INC_SIZE 8
#define BUF_SIZE 1024
#define NAME_SIZE 16



#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <cstdlib>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;
void sigHndlr(int s)
{
    printf("Good Bye! \n");
    exit(0);
}


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
        
        signal(SIGINT, sigHndlr);
        signal(SIGTSTP, sigHndlr);
        
        
    /* ------- SETTINGS OF SOCKETS -------*/
    int main_socket, port, clients, max_clients, events, temp_socket, i;
    ssize_t n_read;
    char buf[BUF_SIZE];
    //struct sockaddr_in adr;
    struct pollfd * fds, * temp_fds;
        
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;

        port = 3434;
    //we work with INET connection and addr family
        memset(&hints, 0, sizeof(struct addrinfo));
       hints.ai_family = AF_INET;
       hints.ai_socktype = SOCK_STREAM;
       hints.ai_flags = AI_PASSIVE;
       hints.ai_protocol = 0;          // Any protocol
       hints.ai_canonname = NULL;
       hints.ai_addr = NULL;
       hints.ai_next = NULL;

        
        // -------- IF YOU WANT YOU CAN CHANGE PORT BOTH IN CLIENT AND SERVER ------
        s = getaddrinfo(NULL, "3434", &hints, &result);
        
       if (s != 0) {
           throw("Getaddrinfo Error");
       }
        
       for (rp = result; rp != NULL; rp = rp->ai_next) {
           main_socket =  socket(rp->ai_family, rp->ai_socktype,0);
           if (main_socket  == -1)
               continue;

           errno = 0;
           
           
           if (::bind(main_socket, rp->ai_addr, rp->ai_addrlen) == 0)
                             break;                  // Success
           if (rp == NULL) {
               throw("bind Error");
           }
           //close(mai);
           
       }
    
    //setting of TCP listerning connection
    if(listen(main_socket, MAX_QUEUE) == -1){
        throw("TCP Error");
    }

    max_clients = MEM_INC_SIZE;
    clients = 0;

    //creating of array, that will store in it sockets' numbers of different clients
    fds = (struct pollfd*)malloc(sizeof(struct pollfd) * (max_clients + 1));
    if(fds == NULL){
        throw("Mallocs Error");
    }
    
    //zero element will provide connection of new users to server, saving them to new elem
    fds[0].fd = main_socket;
    fds[0].events = POLLIN | POLLERR | POLLPRI | POLLOUT;
    fds[0].revents = 0;
    
    for(;;){
        //poll is waiting while something will happend in descriptors
        events = poll(fds, clients + 1, 100);
        if(events == -1){
            throw("Poll Error");
        }
        
        // if after cooldown nothing happend we will continue
        if(events == 0)
            continue;
        
        //if zero elen => this is new user
        if(fds[0].revents){
            temp_socket = accept(main_socket, NULL, NULL);
            if(temp_socket == -1){
                throw("Recive Error");
            }
            clients++;
            cout << "Client connected\n";
            //if there is no place
            if(clients >= max_clients){
                max_clients += MEM_INC_SIZE;
                temp_fds = fds;
                fds = (struct pollfd *)realloc(fds, sizeof(struct pollfd) * (max_clients + 1));
                if(fds == NULL){
                    free(temp_fds);
                    throw("Realloc Error");
                }
            }

            //new user now will work on <int> <client> number
            fds[clients].fd = temp_socket;
            fds[clients].events = POLLIN | POLLERR | POLLPRI | POLLHUP;
            fds[clients].revents = 0;
            write(temp_socket,"server v0.0.1\n",strlen("server v0.0.1\n"));
            //shutdown(temp_socket, SHUT_WR);
            //to make poll usefull again
            fds[0].revents=0;
        }
        //here we are checking if something happend with already connected clients
        for(i = 1; i <= clients; i++){
            if(fds[i].revents){
                memset(buf, '\0', BUF_SIZE);
                n_read = read(fds[i].fd, buf, BUF_SIZE);
                //std::cout << buf;
                //nothing read => kick
                if(n_read == 0){
                    printf("Client disconnected\n");
                    close(fds[i].fd);
                    fds[i].fd = -1;
                }
                //BADDDDD
                if(n_read == -1){
                    close(fds[i].fd);
                    fds[i].fd = -1;
                    throw("Error when reading from socket");
                }
                //something useful
                if(n_read > 0){
                    /* ------- VARAIBLES FOR ANALYSATOR -------*/
                    //creating regular prototypes for first and second part of file
                    const std::regex regular_for_way("([\\w])(,)([\\w])");// smthng like a,b
                    const std::regex regular_for_value("([\\w])(:)([\\d]+)");// smthng like a:23
                    std::smatch m;//varaible to construct a determinition of our line in a file and to devide it by a groups
                    int quantity_of_nodes = 0;
                    vector< vector<int> > g;

                    map <string, int> mp, tmp_map;
                    
                    std::string line;
                    //we will cut the message in order to make it "analysable"
                    line = strtok(buf, "\n\0");
                    
                    
                    //We will create a map object, which will provide the compliance between the node name and its' unique number
                    while ((line.length() != 0) && line != "}")
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
                        line = strtok(NULL, "\n\0");
                    }
                    line = strtok(NULL, "\n\0");
                    if (line != "{")
                    {
                        throw ("SYNTAX ERROR\n");
                    }
                    
                    //Creating of arrayn of nodes' quantity of people
                    std::vector<int> array_of_net_value(quantity_of_nodes);
                    line = strtok(NULL, "\n\0");
                    while ((line.length() != 0) && line != "}")
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
                        
                        
                        line = strtok(NULL, "\n\0");
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
                        array_visit[i] = false;//the node we investigate is already have visited
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
                    //std::ofstream File ( "Result.txt" ) ; //output file
                    for ( int j = 0; it != mp.end(); it++, j++)
                    {
                        if (it->second == result)
                        {
                            char *  tmp = new (char[BUF_SIZE]);
                            strcpy(tmp, it->first.c_str());
                            write(fds[i].fd,"The worst network building to remove is:", strlen("The worst network building to remove is:") );
                            send(fds[i].fd, tmp ,sizeof(char), 0 );
                            cout << "The worst network building to remove is:" << it->first << endl;
                        }
                    }
                }
            }
            fds[i].revents = 0;//to make poll usefull again
        }
    }
    }
    catch (const char* str)
    {
        cout << str;
    }
    catch (...)
    {
        cout << "Undefined error\n";
    }
    return 0;
}
