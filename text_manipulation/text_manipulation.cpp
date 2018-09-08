//
// Book : Version du chapitre 23 de l'ouvrage.
// "Programming -- Principles and Practice Using C++" de Bjarne Stroustrup (2ème édition : 2014)
// Commit initial : 07/09/2018 
// Commit en cours : 08/09/2018 - Exercice n°2 de la page 887
// Caractères spéciaux : [ ]   '\n'   {  }   ||   ~   _     @


#include "pch.h"		// En-tête indispensable sous Windows et VC++
#include "MyHeader.h"	// Header personnel (en partie dérivé de std_lib_facilities de BS)

#include<vector>
#include<map>
#include<fstream>

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
		if (*p== "****") {
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

bool find_from_field(const Message* m, string& s, const string field)
{  

	for (const auto& x:*m)
		if (int n=is_prefix(x,field)) {
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

void print_messages(const Mail_file& mf)
{
	// Fonction Helper : affichage de tous les messages du fichier	
	cout <<endl<<endl<< "Nombre de messages différents : " << distance(mf.begin(), mf.end())<<endl;
	cout << "--------------------------------------" << endl;
	
	int i = 0;
	for (auto p = mf.begin(); p != mf.end(); ++p) {
		++i;
		cout << "********************** Message numéro " << i<< "******************************************************************************************"<<endl;
		
		for (const auto& x : *p) cout << x << endl;	// Impression de toutes les lignes du message via l'itérateur Line_iter intégré à la classe Message

	}
	

}


int main()
{
	try
	{
		// A mettre dans la 1ère ligne du "main" pour afficher les caractères accentués sous la console Windows
		std::locale::global(std::locale("fr-FR"));

		Mail_file mfile{ "mail.txt" };

		// traitement du sender (exo 1 page 887)
		multimap<string, const Message*> sender;

		for (const auto& m:mfile) {
			string s;
			if (find_from_field(&m, s, "De : "))
				sender.insert(make_pair(s, &m));
		
		}
						
		auto pp = sender.equal_range("Hakima BRIHI <hbrihi@finalife.com>");
		for (auto p = pp.first; p != pp.second; ++p)
			cout << find_subject(p->second) << endl;

		// traitement de l'objet (exo 2 page 887) ************************************************************
		
		multimap<string, const Message*> objet;

		for (const auto& m : mfile) {
			string s;
			if (find_from_field(&m, s, "Objet : "))
				objet.insert(make_pair(s, &m));

		}
		
		print_messages(mfile);	// Impression de tous les messages pour test

		string objet_voulu = "Re: SIMULATION DE FINANCEMENT";
		auto oo = objet.equal_range(objet_voulu);
		cout << endl << endl << "Nombre de messages dont l'objet est "<< objet_voulu<<" : " << distance(oo.first, oo.second) << " xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<< endl<<endl;
		for (auto p = oo.first; p != oo.second; ++p)
			for (const auto& x : *(p->second)) cout << x << endl;	// p->first contient l'objet recherhé et p->second pointe sur le message lié (donc * permet de récupérer le contenu pointé)
		

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
