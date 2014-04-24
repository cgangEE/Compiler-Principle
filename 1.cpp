#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <climits>
using namespace std;

#define rep(i, n) for (int i=0; i<(n); ++i)
#define repf(i, a, b) for (int i=(a); i<=(b); ++i)
#define repd(i, a, b) for (int i=(a); i>=(b); --i)
#define clr(a, b) memset(a, b, sizeof(a))
#define pb push_back
#define sz(a) ((int)(a).size())
#define N 1000
#define KEY 26
#define SYB 18

struct Word{
	int syn;
	string token;
	Word(){}
	Word(int syn, string token):syn(syn), token(token){}
};

char s[N+10];
vector<string> codes;
vector<Word> words;

char key[KEY][10] = {"main", "if", "then", "while", "do",
			"static", "int", "double", "struct", "break",
			"else", "long", "switch", "case", "typedef",
			"char", "return", "const", "float", "short",
			"continue", "for", "void", "sizeof", "default",
			"do"};

int keyNum[KEY] = {
				1, 2, 3, 4, 5,
				6, 7, 8, 9, 10,
				11, 12, 13, 14, 15,
				16, 17, 18, 19, 20, 
				21, 22, 23, 24, 39,
				40
			};

char syb[SYB][10] = {"+", "-", "*", "/", ":", 
				  ":=", "<", "<>", "<=", ">",
				  ">=", "=", ";", "(", ")", 
				  "#", "++", "--"};

int sybNum[SYB] = {27, 28, 29, 30, 31,
				32, 33, 34,35, 36,
				37, 38, 41, 42, 43,
				0};

bool proKey(string s){
	rep(i, KEY) if (strcmp(s.c_str(), key[i])==0){
		words.pb(Word(keyNum[i], key[i])); 
		return true;
	}
	return false;
}

bool proSyb(string s){
	rep(i, SYB) if (strcmp(s.c_str(), syb[i])==0){
		words.pb(Word(sybNum[i], syb[i]));
		return true;
	}
	return false;
}



void process(){
	rep(i, sz(codes)){
		string code;
		rep(j, sz(codes[i])){
			char c = codes[i][j];
			if (isalpha(c) || isdigit(c))
				code += c;
			else{
				if (sz(code)){
					if (isalpha(code[0])){
				   		if (!proKey(code)) words.pb(Word(25, code));	//ID
					}
					else words.pb(Word(26, code));						//NUM
					code.clear();
				}
				if (c!=' ' && c!='\t'){
					string s = "";
					if (j != sz(codes[i])-1){
						char h = codes[i][j+1];
						if (proSyb( s+c+h )){
							j++;
							continue;
						}
					}
					proSyb(s+c);
				}
			}
		}
	}
	rep(i, sz(words)) cout<<words[i].syn<<' '<<words[i].token<<endl;
}

void rmCmt(){
	freopen("input.c", "r", stdin);
	freopen("output.c", "w", stdout);	
	bool commented = false;

	while (gets(s) != NULL){
		string line;
		int n = strlen(s);
		rep(i, n){
			if (commented){
				if (i != n-1 && s[i]=='*' && s[i+1]=='/')
					commented = false, ++i;
			}
			else{
				if (i != n-1){
					if (s[i]=='/' && s[i+1]=='/')
						break;
					else if (s[i]=='/' && s[i+1]=='*'){
						commented = true;
						++i;
						continue;
					}
				}
				line += s[i];
			}
		}
		if (sz(line)) codes.pb(line+' ');
	}
//	rep(i, sz(codes)) cout<<codes[i]<<endl;
}

int main(){
	puts("FT");
	rmCmt();
	process();
	
	return 0;
}
