#include <vector>
#include <map>

using namespace std;

class dataset{
	public:
		vector<int> dtset_num;
		
	public:
		dataset(vector<int>);
		void print_data();
		map< string, vector<double> > data;
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
