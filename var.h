#include "spglib.h"
#include <vector>
#include <map>
#include<iostream>

using namespace std;

class dataset{
	public:
		vector<int> dtset_num;
		
	public:
		dataset(vector<int>);
		void print_data();
		map< string, vector<double> > data;
		void check_sym();
};

dataset::dataset(vector<int> current_dataset)
{
	dtset_num = current_dataset;
	vector<int> a;
	data.emplace("acell", vector<double>());
	data.emplace("rprim", vector<double>());
	data.emplace("angdeg", vector<double>());
	data.emplace("xred", vector<double>());
	data.emplace("xcart", vector<double>());
	data.emplace("natom", vector<double>());
	data.emplace("ntypat", vector<double>());
	data.emplace("typat", vector<double>());
}

void dataset::print_data()
{
	map<string, vector<double>>::iterator it;
	for(unsigned int i=0; i<dtset_num.size(); i++)
		cout<<dtset_num[i]<<'\t';
	cout<<endl;
	for ( it = data.begin(); it != data.end(); it++ )
	{
		cout << it->first<<'\t';
		for(unsigned int i=0; i<it->second.size(); i++)
		         cout << it->second[i] << '\t';
		cout << endl;
	}
	cout<<endl;
}

void dataset::check_sym()
{
	double lattice[3][3];
	lattice[0][0]=data["acell"][0]*data["rprim"][0];
	lattice[0][1]=data["acell"][0]*data["rprim"][1];
	lattice[0][2]=data["acell"][0]*data["rprim"][2];
	lattice[1][0]=data["acell"][1]*data["rprim"][3];
	lattice[1][1]=data["acell"][1]*data["rprim"][4];
	lattice[1][2]=data["acell"][1]*data["rprim"][5];
	lattice[2][0]=data["acell"][2]*data["rprim"][6];
	lattice[2][1]=data["acell"][2]*data["rprim"][7];
	lattice[2][2]=data["acell"][2]*data["rprim"][8];
	
	int xred_size = (int)data["xred"].size();
	double position[xred_size/3][3];
	for(int i=0; i*3<xred_size;i++)
	{
		position[i][0]=data["xred"][3*i];
		position[i][1]=data["xred"][3*i+1];
		position[i][2]=data["xred"][3*i+2];
	}
	int types_num = (int)data["natom"][0];
	int types[types_num];
	for(int i=0; i<types_num;i++)
	{
		int ntypat_num = (int)data["typat"][i];
		types[i]=data["ntypat"][ntypat_num];
	}	
	
	int num_atom = types_num;
	double symprec = 1e-5;
	char symbol[21];
	int num_spg = spg_get_international(symbol, lattice, position, types, num_atom, symprec);
	printf("*** Example of spg_get_international ***:\n");
	if ( num_spg > 0 ) {
		printf("%s (%d)\n", symbol, num_spg);
	} else {
		printf("Space group could not be found.\n");
	}
}
