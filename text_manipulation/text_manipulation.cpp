//
// Book : Version du chapitre 23 de l'ouvrage.
// "Programming -- Principles and Practice Using C++" de Bjarne Stroustrup (2ème édition : 2014)
// Commit initial : 07/09/2018 
// Commit en cours : Exercice n°1 de la page 887
// Caractères spéciaux : [ ]   '\n'   {  }   ||   ~   _     @

// Librairie indispensable sous Windows et VC++
#include "pch.h"

#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<iostream>

#include "MyHeader.h"

using namespace std;

typedef vector<string>::const_iterator Line_iter;

class Message {  
	Line_iter first;
	Line_iter last;
public:
	Message(Line_iter p1, Line_iter p2):first{p1}, last{p2} {  }
	Line_iter begin() const { return first; }
	Line_iter end() const { return last; }

};

using Mess_iter = vector<Message>::const_iterator;

struct Mail_file {  
	string name;
	vector<string> lines;
	vector<Message> m;

	Mail_file(const string& n);

	Mess_iter begin() const { return m.begin(); }
	Mess_iter end() const { return m.end(); }

};

Mail_file::Mail_file(const string& n)
{  
	ifstream in{ n };
	if (!in) {
		cerr << "no " << n << endl;
		exit(1);
	
	}

	for (string s; getline(in, s);)
		lines.push_back(s);

	auto first = lines.begin();
	for (auto p=lines.begin();p!=lines.end();++p) {
		if (*p== "----") {
			m.push_back(Message(first, p));
			first = p + 1;
		}
			
	}

}

int is_prefix(const string& s, const string& p)
{  

	int n = p.size();
	if (string(s, 0, n) == p) return n;
	return 0;

}

bool find_from_addr(const Message* m, string& s)
{  

	for (const auto& x:*m)
		if (int n=is_prefix(x,"De : ")) {
			s = string(x, n);
			return true;
		
		}
	return false;
}

string find_subject(const Message* m)
{  

	for (const auto& x : *m)
		if (int n = is_prefix(x, "Objet : ")) return string(x, n);
	return "";

}

int main()
{
	try
	{
		// A mettre dans la 1ère ligne du "main" pour afficher les caractères accentués sous la console Windows
		std::locale::global(std::locale("fr-FR"));

		Mail_file mfile{ "mail.txt" };

		multimap<string, const Message*> sender;

		for (const auto& m:mfile) {
			string s;
			if (find_from_addr(&m, s))
				sender.insert(make_pair(s, &m));
		
		}

		auto pp = sender.equal_range("Hakima BRIHI <hbrihi@finalife.com>");
		for (auto p = pp.first; p != pp.second; ++p)
			cout << find_subject(p->second) << endl;

		keep_window_open();

		return 0;
	}
	/*
	catch (XXXXXXX::Invalid&)
	{
		cerr << "error: ???????????????????? ! \n";
		keep_window_open("q");	// For some Windows(tm) setup
		return 1;
	}
	*/

	catch (runtime_error& e)
	{	// this code is to produce error messages
		cout << e.what() << '\n';
		keep_window_open("q");	// For some Windows(tm) setups
	}
}
