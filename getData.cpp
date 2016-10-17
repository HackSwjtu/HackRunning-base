
/*************************************************************************
    > File Name: ./handsomehow/noRun.cpp
    > Author: HandsomeHow
    > Mail: handsomehowyxh@gmail.com 
    > Created Time: 2016年10月15日 星期六 18时14分09秒
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <algorithm>
#include <string>
#include <cmath>
using namespace std;
const double eps = 1e-8;
const int inf = 0x3f3f3f3f;
const double PI = 3.1415926535897932384626;
#define EDGE_MAX_DIS (50.0)

#ifdef HandsomeHow
#define dbg(x) cerr << #x << " = " << x << endl
#else
#define dbg(x)
#endif
// (づ°ω°)づe★------------------------------------------------
map<string,string>msg;
fstream input, output;
string username,password;
class Time{
private:
		time_t _baseTime;
public:
	struct tm *currenttime;
	void gettime(){
		time_t passTime;
		time(&passTime);
		passTime+=8*3600;
		//GMT+8
	    _baseTime = passTime;
		currenttime = gmtime(&passTime);
		currenttime->tm_year += 1900;
		currenttime->tm_mon++;
	}

	void add(int t){
		_baseTime+=t;
		time_t passTime = _baseTime;
		currenttime = gmtime(&passTime);
		currenttime->tm_year += 1900;
		currenttime->tm_mon++;
	}
};

double radian(double d){
	return d * PI / 180.0;
}

class Point{
public:
	double lat,lng;
	Point(double lng, double lat):lat(lat),lng(lng){}
	Point():lat(0),lng(0){}

	bool operator ==(Point &p){
		return (fabs(p.lat - lat)<eps && fabs(p.lng - lng)<eps );
	}
	void outPut(){
		output<<lat<<" "<<lng<<endl;
	}


	void errPut(){
		cerr<<lat<<" "<<lng<<endl;
	}
	

	double disToPoint(Point &p){
		double radLat1 = radian(lat);
		double radLat2 = radian(p.lat);
		double a = radLat1 - radLat2;
		double b = radian(lng) - radian(p.lng);
		double s = 2 * asin(sqrt(pow(sin(a/2),2) + 
					   cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
		s = s * 6378.137;
		return s * 1000;
	}
	/*get the distance between tow point whit return value in mile*/

};

static const std::string base64_chars =   
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"  
             "abcdefghijklmnopqrstuvwxyz"  
             "0123456789+/";  
  
  
static inline bool is_base64(unsigned char c) {  
  return (isalnum(c) || (c == '+') || (c == '/'));  
}  
  
std::string base64_encode(char const* bytes_to_encode, unsigned int in_len) {  
  std::string ret;  
  int i = 0;  
  int j = 0;  
  unsigned char char_array_3[3];  
  unsigned char char_array_4[4];  
  
  while (in_len--) {  
    char_array_3[i++] = *(bytes_to_encode++);  
    if (i == 3) {  
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;  
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);  
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);  
      char_array_4[3] = char_array_3[2] & 0x3f;  
  
      for(i = 0; (i <4) ; i++)  
        ret += base64_chars[char_array_4[i]];  
      i = 0;  
    }  
  }  
  
  if (i)  
  {  
    for(j = i; j < 3; j++)  
      char_array_3[j] = '\0';  
  
    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;  
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);  
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);  
    char_array_4[3] = char_array_3[2] & 0x3f;  
  
    for (j = 0; (j < i + 1); j++)  
      ret += base64_chars[char_array_4[j]];  
  
    while((i++ < 3))  
      ret += '=';  
  
  }  
  
  return ret;  
  
}  


vector<vector<pair<double,int> > >E;
vector<Point>V;
vector<Point>okV;

void addPoint(){
	double x,y;
	input.open("okPoint.txt");
	for(int i = 0; i < 16; ++i){
		input>>x>>y;
		okV.push_back(Point(x,y));
	}
	input.close();

	input.open("pathPoint.txt");
	while(input>>y>>x)
		V.push_back(Point(x,y));
	input.close();
	
//	cerr<<"get "<<V.size()<<" points"<<endl;

	/*
	int cnt[18];
	for(int i = 0; i < 16; ++i)
		cnt[i]=0;
	*/

	for(int i = 0; i < 16; ++i) 
		V.push_back(okV[i]);
}

void initEdge(){
	int totsz = V.size();
	for(int i = 0; i < totsz; ++i)
		E.push_back(vector<pair<double,int> >());
	for(int i = 0; i < totsz; ++i)
		for(int j = 0; j < i; ++j){
			double tmpDis = V[i].disToPoint(V[j]);
			if(tmpDis <EDGE_MAX_DIS){
				E[i].push_back(make_pair(tmpDis,j));
				E[j].push_back(make_pair(tmpDis,i));
			}
		}
	/*add edge between two points that the distance between is less than 50miles*/

	for(int i = 0; i < totsz; ++i)
		sort(E[i].begin(),E[i].end());
	//sort the edge by distance
	

}

vector<int> getTar(){
	input.open("T.txt");
	vector<int>ret;
	int sz = V.size();
	double lat,lng;
	for(int i = 0; i < 5; ++i){
		input>>lat>>lng;
		Point tmp = Point(lat,lng);
		for(int j = sz-1; ; --j){
			if(V[j] == tmp){
				ret.push_back(j);
				break;
			}
		}
	}
	input.close();
	return ret;
}
//read 5 points from txt

vector<int>spfa(int S){
	vector<double>dis(V.size(), inf);
	vector<bool>in(V.size(),false);
	vector<int>pre(V.size(),-1);
	queue<int>q;
	q.push(S); in[S] = true; dis[S] = 0;
	while(!q.empty()){
		int u = q.front(); q.pop();
		in[u] = false;
		int sz = E[u].size(),beg = rand()%(min(sz/10+1,5)) ;
		for(int i = 0; i < sz; ++i){
			if(dis[u] + E[u][i].first < dis[E[u][i].second] ){
				dis[E[u][i].second] = dis[u] + E[u][i].first;
				pre[E[u][i].second] = u;
				if(!in[E[u][i].second]){
					in[E[u][i].second] = true;
					q.push(E[u][i].second);
				}
			}
		}
	}
	return pre;
}

//spfa and return the pre point of each point

vector<int>  getPath(vector<int>pointV, int setDis){
	vector<int>finalPath; 
	double nowDis = 0;
	int S  = rand()%V.size();
	finalPath . push_back(S);
	while(nowDis < setDis * 1000.0){
		S = finalPath.back();
		int T;
		if(pointV.size())
			T = rand()%pointV.size();
		else T = rand()%V.size();
		if(pointV.size()) T = pointV[T];
		vector<int>pathST = spfa(S);
		stack<int>st;
		while(T != S ){
			st.push(T);
			T = pathST[T];
			for(int i = 0; i < (int)pointV.size(); ++i){
				if(V[T].disToPoint(V[pointV[i]]) < 50.0  )
					pointV.erase(pointV.begin()+i);
			}
		}
		while(!st.empty()){
			nowDis+=V[finalPath.back()].disToPoint(V[st.top()]);
			finalPath.push_back(st.top());
			st.pop();
		}
	}
	return finalPath;
}
//return a valid path whth id

string intToStr(long long x){
	string ret;
	if(x==0) ret = "0";
	else{
		while(x>0){
			ret = ret + (char)((x%10)+'0');
			x/=10;
		}
	}
	reverse(ret.begin(),ret.end());
	return ret;
}

string doubleToStr(double x){
	string ret = intToStr( (long long)x );
	x-=(long long)x;
	ret = ret + '.';
	double tmp = 0.1;
	for(int i = 0; i < 6; ++i){
		int ii = x/tmp;
		x-=ii*tmp;
		ret = ret + (char)(ii+'0');
		tmp/=10;
	}
	return ret;
}

void outPutAllLocJson(vector<int>&path, long long flag){
	string Tmp = "allLocJson";
	//output.open("AllLocJson.txt");
	Point lst = V[path[1]];
	long long tmp = flag;
	double totalDis = rand()%100*1.0/10;
	int totalTime = rand()%15;
	Time mytime;
	mytime.gettime();
	int beginID = rand()%20+5;
	string s = "\'[";
	int sz = path.size();
	for(int i = 0; i < sz; ++i){
		totalTime+=5;
		mytime.add(5);
	 	s = s + "{\\\"gainTime\\\":\\\"" + intToStr((long long)mytime.currenttime->tm_year) + "-";

	 	if(mytime.currenttime->tm_mon<10) s=s+'0';
	 	s=s+ intToStr((long long)mytime.currenttime->tm_mon) + "-" ;

	 	if(mytime.currenttime->tm_mday<10) s=s+'0';
	 	s=s+ intToStr((long long)mytime.currenttime->tm_mday)+" ";

	 	if(mytime.currenttime->tm_hour<10) s=s+'0';
	 	s=s+ intToStr((long long)mytime.currenttime->tm_hour) + ":";

	 	if(mytime.currenttime->tm_min<10) s=s+'0';
	 	s=s+ intToStr((long long)mytime.currenttime->tm_min) + ":";

	 	if(mytime.currenttime->tm_sec<10) s=s+'0';
	 	s=s+ intToStr((long long)mytime.currenttime->tm_sec); 

	 	s=s+"\\\",\\\"type\\\":";
	 	if(i==0) s=s+"5";
	 	else if (i==sz-1) s=s+"6";
	 	else s=s+"0";

	 	s=s+",\\\"avgSpeed\\\":\\\"";
	 	s=s+doubleToStr( (rand()%3000+1000) /1000.0 );

	 	Point pot = V[path[i]];
	 	totalDis+=lst.disToPoint(pot);

	 	s=s+"\\\",\\\"flag\\\":"+intToStr((long long)tmp)+",\\\"lat\\\":\\\"";
	 	s=s+doubleToStr(pot.lat)+"\\\",\\\"lng\\\":\\\""+doubleToStr(pot.lng)+"\\\",\\\"totalTime\\\":"+intToStr(totalTime);
	 	s=s+",\\\"totalDis\\\":\\\""+doubleToStr(totalDis);
	 	s=s+"\\\",\\\"speed\\\":\\\""+doubleToStr(1000.0/(lst.disToPoint(pot)/5.0)/60);
	 	s=s+"\\\",\\\"radius\\\":"+intToStr(rand()%3+8)+"."+intToStr(rand()%10);
	 	s=s+",\\\"locType\\\":61,\\\"id\\\":";
	 	s=s+intToStr(i+beginID);
	 	s=s+"}";
	 	if(i!=sz-1) s=s+",";
	 	lst = pot;
	}
	
	s = s + "]\'";
	msg["totalTime"] = intToStr(totalTime);
	msg["speed"] = doubleToStr(totalTime*1.0/totalDis/60*1000);
	msg["totalDis"] = doubleToStr(totalDis/1000);
	msg["stopTime"] = intToStr(flag+totalTime*1000+rand()%1000);
	msg[Tmp]=s;
	//output<<s<<endl;
	//output.close();
}

string Hash(int len, int tp){
	string tmp = username.substr(3,6);
	string tab =  "9783512460ABCDEF";
	string ret;
	int idx = 0;
	while(ret.length()<len){
		idx++;
		idx%=username.length();
		int k = tmp[idx]-'0';
		k*=31;
		k%=password.length();
		k*=100007;
		k%=tp;
		ret=ret+tab[k];
	}
	return ret;
}

void getMsg(){
	msg["DeviceId"] = Hash(15,10);
	msg["CustomDeviceId"] = Hash(32,16);
}


void outPutFivePointJson(vector<int>P, long long flag){
	string Tmp = "fivePointJson";
	long long tmp = flag;
	string tmps = intToStr((long long)tmp);
	Time mytime;
	mytime.gettime();
	//output.open("./handsomehow/FivePointJson.txt");
	int fix = rand()%5;
	string s = "\'[";
	for(int i = 0; i < 5; ++i){
		s=s+"{\\\"flag\\\":"+tmps+",\\\"lon\\\":\\\""+doubleToStr(V[P[i]].lng)+"\\\",\\\"lat\\\":\\\"";
		s=s+doubleToStr(V[P[i]].lat);
		s=s+"\\\",\\\"isFixed\\\":";
		if(i==fix) s=s+"1";
		else s=s+"0";
		s=s+",\\\"isPass\\\":true,\\\"isFinal\\\":false,\\\"id\\\":";
		s=s+intToStr(i+66)+"}";
		if(i!=4) s=s+",";
	}

	s = s + "]\'";
	
	msg[Tmp]=s;
	//output<<s<<endl;
	//output.close();
}

void readMsg(){
	cout<<"username:";
	cin>>username;
	cout<<"password:";
	cin>>password;
	string tmp = username+":"+password;
	msg["Authorization"] = base64_encode(tmp.data(),tmp.length());
}


void outTemplate(){
	input.open("template.txt");	
	output.open("Go.py");
	string str;
	while(input>>str){
		//cerr<<str<<endl;
		if(str == "T")
			output<<"   ";
		else if(str == "#"){
			input>>str;
			output<<msg[str];
		}else if(str == "X")
			output<<endl;
		else if(str == "SQ")
			output<<" ";
		else 
			output<<str;
		
	}
	output.close();
	input.close();
}


int main(){
	srand(time(0));
	readMsg();
	getMsg();
	addPoint();
	initEdge();
	int setDis = 3;
	//int setDis = rand()%3+1;	//in range[1,3];
	vector<int>T = getTar(); 
	vector<int> path = getPath(T, setDis);
	long long flag = time(0)*1000ll+rand()%1000;
	msg["starttime"] = intToStr(flag);
	outPutFivePointJson(T,flag);
	outPutAllLocJson(path,flag);
	//output.open("Go.py");
	//for(auto it:msg) output<<it.first<<" "<<it.second<<endl;
	outTemplate();
	cerr<<"Finish"<<endl;
	return 0;
}
