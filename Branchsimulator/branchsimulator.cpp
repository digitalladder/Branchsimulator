#include<iostream>
#include<string>
#include<bitset>
#include<vector>
#include<fstream>
#include<cmath> 
#include<sstream>
#include<numeric>

using namespace std;
#define T 1	//branch taken
#define NT 0	//branch not taken

int bitod(int p,int q)	//callback function
{
		return (p<<1)+q;
}
	
int main()
{
	int m, k;
	int j = 0;
	int total=0,miss=0;	//total trace and miss prediction counters
	string re,tr;
	bitset<32> trace;
	string line;
	unsigned int add,result;	//trace address and branch result
	int BHR;	//branch history register
	vector<bool> regist;
	
	//initialize counters

	ifstream configure;
	configure.open("config_new.txt");
	if (configure.is_open())
	{
		getline(configure, line);
		stringstream s1,s2;
		s1<<line;
		s1>>m;
		getline(configure, line);
		s2<<line;
		s2>>k;
		cout<<"m="<<m<<"  "<<"k="<<k<<endl;
	}
	else cout << "unable to open file" << endl;
	configure.close();
	vector<vector<int> >count;
	count.resize(pow(2, m));
	for (int i = 0; i < pow(2, m); i++)
	{
		count[i].resize(pow(2, k));
		for (int j = 0; j < pow(2, k); j++)
			count[i][j] = 3;
	}
	
	//globle history register
	regist.resize(k,1);	
	BHR=accumulate(regist.begin(),regist.end(),0,bitod);
	cout<<"initial BHR="<<BHR<<endl; 
	cout<<"Branch address"<<"			"<<"Branch result"<<endl;
	
	//reading trace
	ifstream tracefile;
	tracefile.open("trace.txt");
	ofstream rfout;
	rfout.open("trace.txt.out");
	rfout<<"Branch prediction"<<endl;
	if (tracefile.is_open())
	{
		while (getline(tracefile, line))
		{
			istringstream ist(line);
			ist>>tr>>re;
			stringstream addr(tr);
			addr>>std::hex>>add;
			stringstream res(re);
			res>>std::hex>>result;
			trace=bitset<32>(add);
			cout<<trace<<"  "<<result<<endl;
			int branch = add <<32-m>>32-m;
			bool prediction = (bool) count[branch][BHR];	//make a prediction to current branch
			if (result == NT)
			{
				if (count[branch][BHR] == 3)
					count[branch][BHR] = 1;
				else count[branch][BHR] = 0;
			}
			if (result == T)
			{
				if (count[branch][BHR] == 0)
					count[branch][BHR] = 1;
				else count[branch][BHR] = 3;
			}
			regist.insert(regist.begin(),bool(result));		//renew BHR value
			regist.pop_back();
			BHR=accumulate(regist.begin(),regist.end(),0,bitod);
			cout<<"new BHR="<<bitset<sizeof(int)*2>(BHR)<<endl;
			if (rfout.is_open())
			{
				rfout << prediction << endl;
			}
			else cout << "unable to open file";
			total++;
			miss+=(!(prediction==result));
			cout<<"total="<<total<<"     "<<"miss="<<miss<<endl;
		}
	}
	else cout << "unable to open file";
	tracefile.close();
	rfout.close(); 
	return 0;
}
