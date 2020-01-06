#include <vector>
#include <fstream>
#include <string>
#include <cmath>
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

// function converting word to number
double word_to_num(string &word, int line_number)
{
	vector<double> numbers;
	vector<char> operators;
	string::size_type check_point;
	int open_brackets=0;
	int close_brackets=0;
	// separating word on numbers and operators and checking brackets
	while(word.size()!=0)
	{
		if(word[0]=='(')
		{
			open_brackets++;
			operators.push_back(word[0]);
			word.erase(word.begin());
		}
		else if(word[0]==')')
		{
			close_brackets++;
			if(close_brackets>open_brackets)
			{
				cerr<<"There are wrong brackets in number in line "<<line_number<<endl;
				exit(1);
			}
			operators.push_back(word[0]);
			word.erase(word.begin());
		}
		else if(word[0]=='*' || word[0]=='/' || word[0]=='^')
		{
			operators.push_back(word[0]);
			word.erase(word.begin());
		}
		else{
			if(numbers.size()<operators.size()-open_brackets-close_brackets)
			{
				cerr<<"Wrong mathematical expression in line "<<line_number<<endl;
				exit(1);
			}
			if( (word[0]=='+' || word[0]=='-') && numbers.size()!=operators.size()-open_brackets-close_brackets )
			{
				operators.push_back('+');
			}
			try
			{
				numbers.push_back(stod(word,&check_point));
			}
			catch (const std::invalid_argument& ia)
			{
				cerr<<"Wrong mathematical expression in line "<<line_number<<endl;
				exit(1);
			}
			word=word.substr(check_point);
		}
	}
	if(close_brackets!=open_brackets)
	{
		cerr<<"There are wrong brackets in number in line "<<line_number<<endl;
		exit(1);
	}
	// loop that makes next math operations until all is done
	while(operators.size()!=0)
	{
	// first goes searching for most nested first bracket
		int num_open_bracket=-1, num_closing_bracket=-1;
		int num_operations=0;
		int temp_operations=0;
		for(unsigned int i=0; i<operators.size(); i++){
			if(operators[i]=='('){
				num_open_bracket=i;
				num_operations+=temp_operations;
			}
			else if(operators[i]==')'){
				num_closing_bracket=i;
				break;
			}
			else
				temp_operations++;
		}
	// if there is no bracket set borders outside operators vector
		if(num_closing_bracket==-1)
		{
			num_closing_bracket=operators.size();
		}
		// for most nested bracket search for power sign (^)
		for(int i=num_open_bracket+1; i<num_closing_bracket; i++){
			if(operators[i]=='^')
			{
				numbers[num_operations+num_open_bracket+1-i] = pow( numbers[num_operations+num_open_bracket+1-i], numbers[num_operations+num_open_bracket+2-i] );
				numbers.erase(numbers.begin()+num_operations+num_open_bracket+2-i);
				operators.erase(operators.begin()+i);
				num_closing_bracket--;
				i--;
			}
		}
		// for most nested bracket search for */ sign
		for(int i=num_open_bracket+1; i<num_closing_bracket; i++){
			if(operators[i]=='*')
			{
				numbers[num_operations+num_open_bracket+1-i] *= numbers[num_operations+num_open_bracket+2-i];
				numbers.erase(numbers.begin()+num_operations+num_open_bracket+2-i);
				operators.erase(operators.begin()+i);
				num_closing_bracket--;
				i--;
			}
			if(operators[i]=='/')
			{
				numbers[num_operations+num_open_bracket+1-i] /= numbers[num_operations+num_open_bracket+2-i];
				numbers.erase(numbers.begin()+num_operations+num_open_bracket+2-i);
				operators.erase(operators.begin()+i);
				num_closing_bracket--;
				i--;
			}
		}
		// for most nested bracket search for + sign
		for(int i=num_open_bracket+1; i<num_closing_bracket; i++){
			if(operators[i]=='+')
			{
				numbers[num_operations+num_open_bracket+1-i] += numbers[num_operations+num_open_bracket+2-i];
				numbers.erase(numbers.begin()+num_operations+num_open_bracket+2-i);
				operators.erase(operators.begin()+i);
				num_closing_bracket--;
				i--;
			}
		}
	// checking if there are brackets to delete
		if(num_closing_bracket!=0)
		{
			operators.erase(operators.begin()+num_open_bracket);
			operators.erase(operators.begin()+num_open_bracket);
		}
	}
	
	return numbers.at(0);
}

// function for managing dataset variables
void word_to_var(string &word, string &var_name, vector <int> ndtset, vector<dataset> &dtset, vector<int> &dts_numbers, int line_number)
{
	string::size_type check_point;
	// first we separate variable name from dataset coordinates
	for(unsigned int i=0; i<=word.size();i++)
	{
		if(!isalpha(word[i]) || i==word.size())
		{
			var_name=word.substr(0,i);
			word=word.substr(i, word.size());
			break;
		}
	}
	// and we check if this variable exist
	if(dtset.at(0).data.find(var_name)==dtset.at(0).data.end())
	{
		cerr<<"Error in line "<<line_number<<": There is no such variable as \""<<var_name<<"\"!"<<endl;
		exit(1);
	}
	// now we extract dataset coordinates for this variable
	if(word.size()==0)
	{
		for(unsigned int i=0; i<ndtset.size();i++)
		{
			dts_numbers.push_back(0);
		}
	}
	while(word.size()!=0)
	{
		if(word[0]=='?')
		{
			dts_numbers.push_back(0);
			word=word.substr(1, word.size());
		}
		else if(word[0]==':')
		{
			dts_numbers.push_back(-1);
			word=word.substr(1, word.size());
		}
		else if(word[0]=='+')
		{
			dts_numbers.push_back(-2);
			word=word.substr(1, word.size());
		}
		else if(word[0]=='*')
		{
			dts_numbers.push_back(-3);
			word=word.substr(1, word.size());
		}
		else if(word[0]==',')
		{
			word=word.substr(1, word.size());
		}
		else
		{
			try
			{
				dts_numbers.push_back(stoi(word,&check_point));
			}
			catch (const std::invalid_argument& ia)
			{
				cerr<<"Wrong dataset expression in line "<<line_number<<endl;
				exit(1);
			}
			if(dts_numbers.back()<=0)
			{
				cerr<<"Error in line "<<line_number<<": In variable \""<<var_name<<"\": there are only positive dataset numbers!"<<endl;
				exit(1);
			}
			word=word.substr(check_point);
		}
	}
	if(dts_numbers.size()!=ndtset.size())
	{
		cerr<<"Error in line "<<line_number<<": In variable \""<<var_name<<"\": wrong datatset dimension!"<<endl;
		exit(1);
	}
	for(unsigned int i=0; i<dts_numbers.size();i++)
	{
		if(dts_numbers[i]>ndtset[i])
		{
			cerr<<"Error in line "<<line_number<<": In variable \""<<var_name<<"\": there is no such datatset!"<<endl;
			exit(1);
		}
	}
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
	string current_word;
	string semicolon_word;
	vector<double> past_numbers;
	vector<double> numbers;
	vector<int> dtset_coordinates;
	vector<int> semicolon_coordinates;
	bool was_semicolon=false;
	int line_number=0;
	while(getline(input_file, line))
	{
		line_number++;
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
				//we have number (math operations handling)
				if(current_word.size()!=0)
					numbers.push_back( word_to_num(word, line_number) );
				else
				{
					cerr<<"Error in line "<<line_number<<": Before putting numbers there must be a variable name first!"<<endl;
					exit(1);
				}
			}
			else
			{
				//we have word (dataset handling)
				//first we check if there are collected numbers for previous variable to assign
				
				// then we check taken word
				word_to_var(word, current_word, ndtset, dtset, dtset_coordinates, line_number);
				// and search for special signs
				// if there was semicolon we must check if variable name matches as well as dataset coordinates
				
				
			}
		}
		words.clear();
	}
	input_file.close();
}

#endif
