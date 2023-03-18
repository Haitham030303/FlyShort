#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <fstream>

using namespace std;

string checkForCollisions(string str);
void dijkstras(int** graph, int start, int end, int n, pair<string, string> airports[], vector<vector<vector<string>>> airlines, unordered_map<string, string> codes);
string space(string str);

int main(void)
{
	ifstream file("data.txt");
	
	int nodes, edges;
	file >> nodes >> edges;

	// allocate a graph (2D array) dynamically
    int** flights = (int**)calloc(nodes, sizeof(int*)); 
	for(int i = 0; i < nodes; i++)
		flights[i] = (int*)calloc(nodes, sizeof(int));     

	// initializing a 3D vector with dimensions [nodes][nodes][1]
	vector<vector<vector<string>>> airlines(nodes, vector<vector<string>>(nodes, vector<string>(1)));
	
	cout << "Loading data...";

    // READ IN THE FLIGHTS WITH THEIR DISTANCES(THE GRAPH) AND AIRLINES OFFERING THE FLIGHTS
	for(int i = 0; i < edges; i++)
	{
		int from, to, weight;
		string airline;
		file >> from >> to >> weight >> airline;
        flights[from][to] = weight;
		
		if (find(airlines[from][to].begin(), airlines[from][to].end(), airline) == airlines[from][to].end())
			airlines[from][to].insert(airlines[from][to].begin(), airline);
	}
	cout << endl;
	
	// 	READ IN AIRPORTS AND STORE THEM ALONG THEIR CORRESPONDING CITIES
	unordered_map<string, int> map; //  map of cities : indexes
	pair<string, string> airports[nodes]; // array of (airport, city) pairs
	
	for (int i = 0; i < nodes; i++)
	{
		string city, airport;
		file >> airport >> city;
		airports[i] = {airport, city};
		map.insert({city, i});
	}

	// READ IN AIRLINE CODES
	unordered_map<string, string>codes;
	for (int i = 0; i < 14; i++)
	{
		string code, airline;
		file >> code >> airline;
		codes.insert({code, airline});
	}
	

	//	READ IN ORIGIN AND DESTINATION CITIES
	string startCity, endCity;
	int start, end;

	system("clear");
    
	
	
	
	// READ IN STARTING NODE
	while(true)
	{
		cout << "Which city do you want to fly from?" << endl;
		cout << "(Format: \"Madison, SD\")" << endl;
		cout << endl << "=> ";
		cin >> startCity;
 
		startCity.assign(checkForCollisions(startCity));

		if (map.find(startCity) == map.end())
		{
			cout << "Couldn't find an airport in " << startCity << "!" << endl;
			cout << "Could be a spelling error." << endl; 
		}
		else
		{
			start = map[startCity];
			break;
		}
	}


	// READ IN DESTINATION NODE
	while(true)
	{
		cout << endl << "Which city do you want to fly to?" << endl;
		cout << "(Format: \"Madison,SD\")" << endl;
		cout << endl << "=> ";
		cin >> endCity;

		endCity.assign(checkForCollisions(endCity));

		if (map.find(endCity) == map.end())
		{
			cout << endl << "Couldn't find an airport in " << endCity << "!" << endl;
			cout << "Could be a spelling error." << endl << endl; 
		}
		else
		{
			end = map[endCity];
			break;
		}
	}	
	//	FIND THE SHORTEST PATH BETWEEN THE TWO AIRPORTS
	dijkstras(flights, start, end, nodes, airports, airlines, codes);

    cout << endl;
	return 0;
}


// function to check if there is more than one airport in that city
string checkForCollisions(string str)
{
	int option;

	if (str.compare("Chicago,IL") == 0)
	{
		while(true)
		{
			cout << "Which airport is nearer to you?" << endl;
			cout << "\t 1. Chicago Midway International Airport" << endl;
			cout << "\t 2. Chicago O'Hare International Airport" << endl;
			cin >> option;
			if (option == 1)
			{
				str = "Chicago1,IL";
				break;
			}
			else if (option == 2)
			{
				str = "Chicago2,IL";
				break;
			}
			else
				cout << "Invalid Option! Please Enter 1 or 2!" << endl;
		}
	}
	
	if (str.compare("Houston,TX") == 0)
	{
		while(true)
		{
			cout << "Which airport is nearer to you?" << endl;
			cout << "\t 1. William P. Hobby Airport" << endl;
			cout << "\t 2. George Bush Intercontinental Airport" << endl;
			cin >> option;
			if (option == 1)
			{
				str = "Houston1,TX";
				break;
			}
			else if (option == 2)
			{
				str = "Houston2,TX";
				break;
			}
			else
				cout << "Invalid Option! Please Enter 1 or 2!" << endl;
		}
	}
	
	if (str.compare("New_York,NY") == 0)
	{
		while(true)
		{
			cout << "Which airport is nearer to you?" << endl;
			cout << "\t 1. John F. Kennedy International Airport(New York International Airport)" << endl;
			cout << "\t 2. LaGuardia Airport (Marine Air Terminal)" << endl;

			cin >> option;
			if (option == 1)
			{
				str = "New_York1,NY";
				break;
			}
			else if (option == 2)
			{
				str = "New_York2,NY";
				break;
			}
			else
				cout << "Invalid Option! Please Enter 1 or 2!" << endl;
		}
	}

	if (str.compare("San_Diego,CA") == 0)
	{
		while(true)
		{
			cout << "Which airport is nearer to you?" << endl;
			cout << "\t 1. McClellan-Palomar Airport" << endl;
			cout << "\t 2. San Diego International Airport(Lindbergh Field)" << endl;

			cin >> option;
			if (option == 1)
			{
				str = "San_Diego1,CA";
				break;
			}
			else if (option == 2)
			{
				str = "San_Diego2,CA";
				break;
			}
			else
				cout << "Invalid Option! Please Enter 1 or 2!" << endl;
		}
	}
	return str;
}


void dijkstras(int** graph, int start, int end, int n, pair<string, string> airports[], vector<vector<vector<string>>> airlines, unordered_map<string, string> codes)
{
	bool *visited = (bool *)calloc(n, sizeof(int));
	int *distances = (int *)calloc(n, sizeof(int)); 
	int *parents = (int *)malloc(n * sizeof(int));

	
	// We start with the starting node being already visited
    visited[start] = true;

	for(int i = 0; i < n; i++)
	{
        if (i != start) 
            distances[i] = INT_MAX;
	}

    int current = start;
    parents[start] = -1;

	while (true)
	{
        for(int i = 0; i < n; i++)
		{
            // for each neighbor of that node that's not visited yet
			if(visited[i] == false && graph[current][i] != 0)
			{
                // if the distance through this path is less than the already established distance
				if(distances[current] + graph[current][i] < distances[i])
                {
					// we have found a shorter route
                    distances[i] = distances[current] + graph[current][i];
                    parents[i] = current;
                }
            }
		}

		int mindistance = INT_MAX;
        bool allNodesAreVisited = true;

        // Go to the closest node that's not visited yet
		for(int i = 0; i < n; i++)
		{
			if(visited[i] == false && distances[i] < mindistance)
			{
				current = i;
				mindistance = distances[i];
                allNodesAreVisited = false;
			}
		}

        if (allNodesAreVisited)
            break;

		visited[current] = true;
	}






	// PRINT FLIGHT INFORMATION
	system("clear");
	
	
	cout << "Flight Information: " << endl;
	cout << "~~~~~~~~~~~~~~~~~~"<< endl << endl;

    stack<int> path;

	if (visited[end] == false)
	{
		cout << "OOPS! There is no path found between " << space(airports[start].second) << " and " << space(airports[end].second) << "!" << endl << endl;
		return;
	}

	int x = end;

	while (x != start)
	{
		path.push(x);
		x = parents[x];
	}

	path.push(start);
	
	if (path.size() == 2)
	{
		cout << "This is the shortest path found: "<< endl;
		cout << endl << "There is a direct flight between " << space(airports[start].second) << " and " << space(airports[end].second) << "." << endl << endl;
	}

	int st, en;
	while(!path.empty())
	{
		if (n != 3) cout << "This is the shortest path found: "<< endl;
		int n = path.size() - 1;
		for (int i = 0; i < n; i++)
		{
			st = path.top();
			path.pop();
			en = path.top();
			cout << endl << endl;

			if(n == 1)
			{
				cout << "--------------------------------------------------------------------------------------------" << endl;
				cout << "Flight: " << endl;
				cout << "--------------------------------------------------------------------------------------------";
			}
			else
			{
				cout << "--------------------------------------------------------------------------------------------" << endl;
				cout << "Flight " << i + 1 <<  ": " << endl;
				cout << "--------------------------------------------------------------------------------------------";
			}
			cout << endl << endl;
			cout << "Departing: "<< endl;
			cout << "---------" << endl;
			cout << "\t   City: " << space(airports[st].second) << endl;
			cout << "\tAirport: " << space(airports[st].first) << endl;
			cout << endl;
			cout << "Destination: " << endl; 
			cout << "-----------" << endl;
			cout << "\t   City: " << space(airports[en].second) << endl;
			cout << "\tAirport: " << space(airports[en].first) << endl;
			cout << endl;
			cout << "Distance: " << distances[en] - distances[parents[en]] << " miles" << endl;
			cout << "--------" << endl << endl;
			vector<string> v = airlines[st][en];
			cout << "Airlines Offering this flight: " << endl;
			cout << "-----------------------------" << endl;
			for (int i = 0; i < v.size() - 1; i++)
			{
			cout << "\t    " << i + 1 << ": " << space(codes[v[i]]) << endl;
			}
		}
		cout << endl << endl << endl;
		cout << "----------------------------------------------------------------------------" << endl;
		cout << "Total Distance: " << distances[end] << " miles"<< endl;
		cout << "----------------------------------------------------------------------------" << endl;
		return;
	}
}

// function to add proper spacing by replacing underscores with spaces and add spaces after commas
string space(string str)
{
	string s;
	
	if (str.find(',') != string::npos)
		str.insert(str.find(',') + 1, " ");
	
	int n = str.length();

	for (int i = 0; i < n; i++)
	{
		char sp = ' ';
		if (str[i] == '_')
			s.push_back(sp);
		
		else
			s.push_back(str[i]);
	}
	return s;

}