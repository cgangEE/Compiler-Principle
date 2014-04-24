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
#define pb push_back
#define sz(a) ((int)(a).size())
#define clr(a, b) memset(a, b, sizeof(a))

#define S 200
#define G 200
#define C 128
#define M 26
#define KS Alpha(1, 0)

struct Alpha{
	int t, e;
	Alpha(){}
	Alpha(int t, int e):t(t),e(e){}
	bool operator == (const Alpha&p)const{
		return t==p.t && e==p.e; 
	}
	bool operator <(const Alpha&p)const{
		return t!=p.t?t<p.t:e<p.e;
	}
};

struct Formula{
	vector<Alpha> a;
	Formula(){}
	Formula(Alpha x){a.pb(x); }
	void push_back(char x){ 
		if (isupper(x)) a.pb(Alpha(0, x-'A'));
		else a.pb(Alpha(1, x));
	}
	void push_back(Alpha x){ a.pb(x); }
	void clear(){ a.clear(); }
	int size(){ return sz(a); }
	Alpha operator [](int i)const{ return a[i]; }
	Formula operator +(const Formula& f)const{
		Formula ret;
		rep(i, sz(a)) if (!(a[i]==KS)) ret.pb(a[i]);
		rep(i, sz(f.a)) if (!(f.a[i]==KS)) ret.pb(f.a[i]);
		if (!sz(ret)) ret.pb(KS);
		return ret;
	}
	bool operator <(const Formula &f)const{
		return a[0]<f.a[0];
	}
	Formula sub(int k){
		Formula ret;
		repf(i, k, sz(a)-1) if (!(a[i]==KS)) ret.pb(a[i]);
		if (!sz(ret)) ret.pb(KS);
		return ret;
	}
	void out(){
		rep(i, sz(a)){
			if (i) putchar(' ');
			if (a[i].t==0){
				if (a[i].e<M) putchar(a[i].e+'A');
				else
			   	cout<<a[i].e;
			}
			else if (a[i].e==0) cout<<a[i].e;
			else putchar(a[i].e);
		}
	}
};

typedef vector<Formula> vF;

vF gra[G+10];
int m[G+10][C+10];
bool v[G+10];
bool first[G+10][C+10], follow[G+10][C+10];
int idx = M, E = -1;
vector<int> edge[G+10];

void outFirst(){
	rep(i, idx) if (sz(gra[i])){
		if (i>=M) cout<<i;
		else putchar(i+'A');
		cout<<" -> ";
		rep(k, C) if (first[i][k]){
			if (k==0) cout<<0;
			else putchar(k);
			putchar(' ');
		}
			
		cout<<endl;
	}
}

void outFollow(){
	rep(i, idx) if (sz(gra[i])){
		if (i>=M) cout<<i;
		else putchar(i+'A');
		cout<<" -> ";
		rep(k, C) if (follow[i][k]){
			if (k==0) cout<<0;
			else putchar(k);
			putchar(' ');
		}
			
		cout<<endl;
	}
}

void out(){
	rep(i, idx) if (sz(gra[i])){
		if (i>=M) cout<<i;
		else putchar(i+'A');
		cout<<" -> ";
		vF & a=gra[i];
		rep(j, sz(a)){
			a[j].out();
			cout<<" | ";
		}
		cout<<endl;
	}
}

void outM(){
	clr(v, 0);
	rep(i, idx) rep(k, C) if (k) if (first[i][k] || follow[i][k]) v[k] =1;

	printf("  ");
	rep(k, C) if (v[k]) printf("%3c ", k);
	cout<<endl;
	rep(i, idx) if (sz(gra[i])){
		putchar(i+'A');
		putchar(' ');
		rep(k, C) if (v[k]) printf("%3d ", m[i][k]);
		cout<<endl;
		
	}
}
void gaoFirst(Formula &a, int e, int l);

void getFirst(vF &a, int e){
	v[e] = 1;
	rep(i, sz(a))
		gaoFirst(a[i], e, i);
}

void gaoFirst(Formula &a, int e, int l){
	rep(i, sz(a)){
		int j = a.a[i].e;
		if (a.a[i].t==0){
			if (j==e) return;
			if (!v[j]) getFirst(gra[j], j);
			rep(k, C) if (k && first[j][k]) first[e][k] = 1, m[e][k] = l;
			if (!first[j][0]) return;
		}
		else{
			first[e][j] = 1, m[e][j] = l;
			return;
		}
	}
	first[e][0] = 1;
	m[e][0] = l;
}

void gaoFollow(Formula &a, int e, int i){
	repf(j, i+1, sz(a)-1)
		if (a[j].t==0){
			rep(k, C) if (k && first[a[j].e][k]) follow[a[i].e][k] =1;
			if (!first[a[j].e][0]) return;
		}
		else{
			follow[a[i].e][a[j].e] = 1; return;
		}
	edge[e].pb(a[i].e);
}

void getFollow(Formula &a, int e){
	rep(i, sz(a)) if (a[i].t==0){
		gaoFollow(a, e, i);
	}
}

void bfsFollow(){
	queue<int> q;
	clr(v, 1);
	rep(i, idx) q.push(i);

	while (sz(q)){
		int i=q.front(); 
		q.pop(), v[i]=0;
		rep(k, sz(edge[i])){
			int j = edge[i][k];
			bool flag = 0;
			rep(k, C) if (follow[i][k] && !follow[j][k]){
				follow[j][k] = 1;
				if (flag == 0){
					flag = 1;
					if (!v[j])
						v[j] = 1, q.push(j);
				}
			}
		}
	}
}

void predict(){
	clr(m, -1);

	rep(i, idx) if (!v[i])
		getFirst(gra[i], i);
	rep(i, idx)
		rep(j, sz(gra[i]))
			getFollow(gra[i][j], i);

	follow[E]['#'] = 1;
	bfsFollow();
	
	rep(i, idx) if (first[i][0])
		rep(k, C) if (follow[i][k]) m[i][k] = m[i][0];
}


//---------------------------------------------------------

void getInto(vF &a, int e, vF b){
	vF c;
	rep(i, sz(a)) 
		if (a[i].a[0] == Alpha(0, e))
			rep(j, sz(b))
				c.pb(b[j]+a[i].sub(1));
		else
			c.pb(a[i]);
	a = c;
}

void rmLefRec(vF &a, int e){
	Formula formula(Alpha(0, idx));
	vF &b = gra[idx++], c;
	rep(i, sz(a))
		if (a[i].a[0] == Alpha(0, e))
			b.pb(a[i].sub(1) + formula);
		else
			c.pb(a[i] + formula);
	a = c;
	b.pb(Formula(KS));
}

void checkLefRec(vF &a, int e){
	rep(i, sz(a))
		if (a[i].a[0] == Alpha(0, e)){
			rmLefRec(a, e);
			return;
		}
}

void rmLefFac(vF &a){
	sort(a.begin(), a.end());
	vF c;
	rep(i, sz(a)){
		int j=i+1;
		while (j<sz(a) && a[j].a[0]==a[i].a[0]) ++j;
		if (j-i>1){
			c.pb(Formula(a[i].a[0]) + Formula(Alpha(0, idx)));
			vF &b = gra[idx++];
			repf(k, i, j-1) b.pb(a[k].sub(1));
			i = j-1;
		}
		else
			c.pb(a[i]);
	}
	a = c;
}

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
			else if (s[k]=='&')
				formula.pb(KS);
			else
				formula.pb(s[k]);
	}

	rep(i, idx){									//消去左递归
		rep(j, i) getInto(gra[i], j, gra[j]);
		checkLefRec(gra[i], i);
	}

	rep(i, idx)										//消去左因子 
		if (sz(gra[i]))
			rmLefFac(gra[i]);
}

void error(){
	puts("sooooorry! error occur!");
}

void process(char *lang){
	
	stack<Alpha> stk;
	int n = strlen(lang), i = 0;
	stk.push(Alpha(1, '#'));
	stk.push(Alpha(0, E));

	while (sz(stk)){
		Alpha x = stk.top();
		if (x.t==1){
			if (x.e==lang[i])
				stk.pop(), ++i;
			else if (x==KS)
				stk.pop();
			else{
				error();
				return;
			}
		}
		else if (m[x.e][lang[i]]!=-1){
			int to = m[x.e][lang[i]];
			stk.pop();
			repd(j, sz(gra[x.e][to])-1, 0)
				stk.push(gra[x.e][to][j]);
		}
		else{
			error();
			return;
		}
	}

	if (i!=n) error();
	else
		puts("No problem!");
}

void process(){
#ifdef __linux
	freopen("/dev/tty", "r", stdin);
#endif 

#ifdef __WINDOWS_
	freopen("CON", "r", stdin);
#endif

#ifdef _WIN32
	freopen("CON", "r", stdin);
#endif
	
	/*
	out();
	cout<<"---------------------------------------\n";
	outM();
	*/
	char lang[S+10];
	while (gets(lang)){
		int n = strlen(lang);
		lang[n] = '#', lang[n+1] = '\0';
	   	process(lang);
	}
}

int main(){
	readGra();
	predict();
	process();
	return 0;
}
