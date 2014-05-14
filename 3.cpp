#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <climits>
#include <cctype>
#include <ctime>
using namespace std;

#define rep(i, n) for (int i=0; i<(n); ++i)
#define repf(i, a, b) for (int i=(a); i<=(b); ++i)
#define repd(i, a, b) for (int i=(a); i>=(b); --i)
#define clr(a, b) memset(a, b, sizeof(a))
#define sz(a) ((int)(a).size())
#define pb push_back

#define S 200
#define M 26	//number of non-terminal symbol
#define N 128	//number of terminal symbol

struct Alpha{
	int t, e;		//t:type 0:non-terminal 1:terminal e:element
	Alpha(){}
	Alpha(int t, int e):t(t),e(e){}
	Alpha(char x){
		if (isupper(x)) t=0, e=x-'A';
		else t=1, e=x;
	}
	bool operator ==(const Alpha &a)const{
		return a.t==t && a.e==e;
	}
	void out(){
		if (t==0) putchar('A'+e);
		else putchar(e);
	}
};

struct Formula{
	vector<Alpha> a;
	Formula(){}
	void pb(Alpha x){a.pb(x); }
	void pb(char x){a.pb(Alpha(x));}
	void clear(){ a.clear(); }
	int size(){ return sz(a); }
	void reverse(){ std::reverse(a.begin(), a.end()); }
	Alpha operator [](int i)const{ return a[i]; }
	void out(){
		rep(i, sz(a)) a[i].out();
		cout<<endl;
	}
};

bool first[M+10][N+10], last[M+10][N+10];
int table[N+10][N+10];	//-1:empty  0:less than 1:equal 2:greater than
vector<Formula> gra[M+10];
int E = -1;

void outVT();
void outTable();

void readGra(){
	freopen("gra.txt", "r", stdin);
	char s[S+10];
	while (gets(s)){
		int n=strlen(s), i = s[0]-'A';
		if (E==-1) E = i;
		if (s[n-1]=='\13') --n;
		s[n] = '|';
		Formula formula;
		repf(k, 3, n)
			if (s[k]=='|')
				gra[i].pb(formula), formula.clear();
			else
				formula.pb(s[k]);
	}
}

void gaoVT(bool first[][N+10]){
	stack<pair<int,int> > stk;
	rep(i, M) rep(j, sz(gra[i])){
		Formula &f = gra[i][j];
		if (f[0].t==1)
			first[i][f[0].e] = 1, stk.push(make_pair(i, f[0].e));
		else if (sz(f)>1 && f[1].t==1)
			first[i][f[1].e] = 1, stk.push(make_pair(i, f[1].e));
	}

	while (sz(stk)){
		pair<int,int> p = stk.top();
		stk.pop();
		
		rep(i, M) rep(j, sz(gra[i])){
			Formula &f = gra[i][j];
			if (f[0] == Alpha(0, p.first) && !first[i][p.second])
				first[i][p.second]=1, stk.push(make_pair(i, p.second));
		}
	}
}

void getVT(){
	gaoVT(first);
	rep(i, M) rep(j, sz(gra[i])) gra[i][j].reverse();
	gaoVT(last);
	rep(i, M) rep(j, sz(gra[i])) gra[i][j].reverse();
}

void addTable(int a, int b, int op){
	if (table[a][b]!=-1 && table[a][b]!=op){
		puts("This isn't operator first grammar!"); return;
	}
	table[a][b] = op;
}

void getTable(){
	clr(table, -1);
	rep(i, M) rep(j, sz(gra[i])){
		Formula &f = gra[i][j];
		rep(k, sz(f)) if (f[k].t==1){
			int a = f[k].e;
			if (k && f[k-1].t==0){
				int e = f[k-1].e;
				rep(l, N) if (last[e][l])
					addTable(l, a, 2);
			}
			if (k+1<sz(f)){
				int e = f[k+1].e;
				if (f[k+1].t==1)
					addTable(a, e, 1);
				else{
					rep(l, N) if (first[e][l])
						addTable(a, l, 0);
					if (k+2<sz(f) && f[k+2].t==1)
						addTable(a, f[k+2].e, 1);
				}
			}
		}
	}
}


int main(){
	readGra();
	getVT();
	getTable();
	outTable();
	return 0;
}

void outVT(){
	cout<<"----------FirstVT---------------"<<endl;
	rep(i, M){
		bool flag=0;
		vector<int> v;
		rep(j, N) if (first[i][j])
			v.pb(j);
		if (sz(v)){
			printf("%c-> ", i+'A');
			rep(j, sz(v)) putchar(v[j]);
			puts("");
		}
	}

	cout<<endl;
	cout<<"-----------LastVT--------------"<<endl;
	rep(i, M){
		bool flag=0;
		vector<int> v;
		rep(j, N) if (last[i][j])
			v.pb(j);
		if (sz(v)){
			printf("%c-> ", i+'A');
			rep(j, sz(v)) putchar(v[j]);
			puts("");
		}
	}
}

void outTable(){
	
	char s[]="+*^i()#";
	int n=strlen(s);

	printf("  ");
	rep(i, n) printf("%c ", s[i]);
	puts("");

	rep(i, n){
		printf("%c ", s[i]);
		rep(j, n){
			int g=table[s[i]][s[j]];
			printf("%c ", g==-1?' ':g==0?'<':g==1?'=':'>');
		}
		puts("");
	}

	/*
	bool x[N+10];
	clr(x, 0);
	rep(i, N) rep(j, N) if (table[i][j]!=-1) x[i]=x[j]=1;
	printf("  ");
	rep(j, N) if (x[j]) printf("%c ", j);
		puts("");

	rep(i, N) if (x[i]){
		printf("%c ", i);
		rep(j, N) if (x[j]){
			int g=table[i][j];
			printf("%c ", g==-1?' ':g==0?'<':g==1?'=':'>');
		}
		puts("");
	}
	*/
}
