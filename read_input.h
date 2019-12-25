#include <vector>
#include <fstream>
#include <string>
#include "var.h"

using namespace std;

#ifndef READ_INPUT_H
#define READ_INPUT_H

// function to check if variable is positive integer
bool is_digit_string(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

// recursive loop for datasets multidimentional vector and adds them with dataset number to datatset vector
void init_dataset(vector<int> ndtset, vector<int> current_dataset, vector<dataset> &dtset, int rec_level) {
	if(current_dataset.size() < ndtset.size())
	{
		rec_level++;
		for (int i=1;i<=ndtset.at(rec_level);i++) {
			current_dataset.push_back(i);
			init_dataset(ndtset,current_dataset, dtset, rec_level);
			current_dataset.pop_back();
		}
	}
	else
	{
		dataset tmp(current_dataset);
		dtset.push_back(tmp);
	}
}


// function to get dimension of datasets
void get_ndtset(string input, vector<int> &ndtset)
{
	ifstream input_file;
	input_file.open(input);
	if (!input_file)
	{
		cerr << "Unable to open file \""<<input<<"\""<<endl;
		exit(1);
	}
	string line;
	vector<string> words;
	
	bool first_ndtset=false;
	bool second_ndtset=false;
	while(getline(input_file, line))
	{
		string word = ""; 
		for (auto x : line) 
		{ 
			if (x == ' ' || x == '\t')
			{
				if(word!="")
				{
					words.push_back(word);
					word = "";
				}
				
			}
			else
			{
				if(x=='#')
				{
					goto comment;
				}
				word = word + x;
			}
		}
		comment:
		if(word!="")
		{
			words.push_back(word);
			word = "";
		}
		for(unsigned int i=0;i<words.size();i++)
		{
			if(first_ndtset==false && second_ndtset==false && words.at(i)=="ndtset")
			{
				first_ndtset=true;
			}
			else if(first_ndtset==true && second_ndtset==false && is_digit_string(words.at(i)))
			{
				ndtset.push_back(stoi(words.at(i)));
			}
			else if( first_ndtset==true && second_ndtset==false && !is_digit_string(words.at(i)) )
			{
				first_ndtset=false;
				second_ndtset=true;
			}
			else if(first_ndtset==false && second_ndtset==true && words.at(i)=="ndtset")
			{
				cout<<"There is more than one ndtset defined"<<endl;
				exit(1);
			}
		}
		words.clear();
	}
	input_file.close();
	if(ndtset.size()==0)
		ndtset.push_back(1);
}

// function to get variables from input file to accurate datasets
void read_input(string input,vector<dataset> &dtset,const vector<int> ndtset)
{
	ifstream input_file;
	input_file.open(input);
	if (!input_file)
	{
		cerr << "Unable to open file \""<<input<<"\""<<endl;
		exit(1);
	}
	string line;
	vector<string> words;
	while(getline(input_file, line))
	{
		string word = ""; 
		for (auto x : line) 
		{ 
			if (x == ' ' || x == '\t')
			{
				if(word!="")
				{
					words.push_back(word);
					word = "";
				}
				
			}
			else
			{
				if(x=='#')
				{
					goto comment;
				}
				word = word + x;
			}
		}
		comment:
		if(word!="")
		{
			words.push_back(word);
			word = "";
		}
		for(unsigned int i=0;i<words.size();i++)
		{
			if( isdigit(words.at(i)[0]) || words.at(i)[0]=='.' || words.at(i)[0]=='-' || ( (words.at(i)[0]=='E' || words.at(i)[0]=='e') && (words.at(i)[1]=='-' || isdigit(words.at(i)[1])) ) )
			{
				//mamy liczbę (obługa działań mat.)
			}
			else
			{
				//mamy słowo (obsługa datasetów)
			}
		}
		words.clear();
	}
	input_file.close();
}

#endif
