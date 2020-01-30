#include <vector>
#include <iostream>
#include "read_input.h"

using namespace std;

int main(int argc, char *argv[]){
	vector<int> ndtset;
	if(argc==1)
	{
		cout<<"No input file was given!"<<endl;
		exit(1);
	}
	get_ndtset(argv[1], ndtset);
	vector<int> current_dataset;
	vector<dataset> dtset;
	init_dataset(ndtset, current_dataset, dtset, -1);
	current_dataset.~vector<int>(); //removing unused vector
	read_input(argv[1], dtset, ndtset);
	for(unsigned int i=0; i<dtset.size(); i++)
	{
		dtset[i].check_sym();
	}
	
	return 0;
}
