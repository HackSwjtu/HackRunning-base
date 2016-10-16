
/*************************************************************************
    > File Name: ./handsomehow/noRun.cpp
    > Author: HandsomeHow
    > Mail: handsomehowyxh@gmail.com 
    > Created Time: 2016年10月15日 星期六 18时14分09秒
 ************************************************************************/

#include <bits/stdc++.h>
using namespace std;
const double eps = 1e-8;
const int inf = 0x3f3f3f3f;
const double PI = 3.1415926535897932384626;
#define EDGE_MAX_DIS (30.0)

#ifdef HandsomeHow
#define dbg(x) cerr << #x << " = " << x << endl
#else
#define dbg(x)
#endif
// (づ°ω°)づe★------------------------------------------------

fstream input, output;

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

vector<vector<pair<double,int> > >E;
vector<Point>V;
vector<Point>okV;

void addPoint(){
	double x,y;
	input.open("./handsomehow/okPoint.txt");
	for(int i = 0; i < 16; ++i){
		input>>x>>y;
		okV.push_back(Point(x,y));
	}
	input.close();

	input.open("./handsomehow/pathPoint.txt");
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
	input.open("./handsomehow/T.txt");
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
		for(int i = beg; i < sz; ++i){
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
		if(pointV.size()){
			int T = rand()%pointV.size();
			T = pointV[T];
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
		}else{
			int u = finalPath.back();
			int to = rand()%E[u].size();
			to = E[u][to].second;
			nowDis+=V[u].disToPoint(V[to]);
			finalPath.push_back(to);
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

void outPutAllLocJson(vector<int>&path, time_t flag){
	output.open("./handsomehow/AllLocJson.txt");
	Point lst = V[path[1]];
	time_t tmp = flag;
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
	output<<s<<endl;
	output.close();
}

void outPutFivePointJson(vector<int>P, time_t flag){
	time_t tmp = flag;
	string tmps = intToStr((long long)tmp);
	Time mytime;
	mytime.gettime();
	output.open("./handsomehow/FivePointJson.txt");
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
	output<<s<<endl;
	output.close();
}



int main(){
	srand(time(0));
	addPoint();
	initEdge();
	int setDis = 3;
	//int setDis = rand()%3+1;	//in range[1,3];
	vector<int>T = getTar(); 
	vector<int> path = getPath(T, setDis);
	time_t flag = time(0)*1000+rand()%1000;
	outPutFivePointJson(T,flag);
	outPutAllLocJson(path,flag);
	return 0;
}
