#include <vector>
#include <map>

using namespace std;

class dataset{
	public:
		vector<int> dtset_num;
		
	public:
		dataset(vector<int>);
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
