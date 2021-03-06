//
// Book : Version du chapitre 23 de l'ouvrage.
// "Programming -- Principles and Practice Using C++" de Bjarne Stroustrup (2ème édition : 2014)
// Commit initial : 07/09/2018 
// Commit en cours : 08/09/2018 
// Caractères spéciaux : [ ]   '\n'   {  }   ||   ~   _     @


#include "pch.h"		// En-tête indispensable sous Windows et VC++
#include "MyHeader.h"	// Header personnel (en partie dérivé de std_lib_facilities de BS)

#include<vector>
#include<map>
#include<fstream>
#include<regex>
#include<iomanip>		//setw
#include<sstream>		

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

struct Pharaon_subscribers {
	string num_abo{};
	string nom{};
	string prenom{};
	string code_postal{};
	string ville{};
	string mail{};

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
	// Helper function before REGEX use (exos 1 & 2)
	for (const auto& x:*m)
		if (int n=is_prefix(x,field)) {
			s = string(x, n);
			return true;
		
		}
	return false;
}

bool find_from_field1(const Message* m, string& s, const regex pat)
{
	// Helper function with REGEX use (exo 3)
	smatch matches;
		
	for (const auto& x : *m)
		if (regex_search(x, matches, pat)) {
			s = string(x, 5);
			return true;

		}
	return false;
}

string find_subject(const Message* m)
{  
	// Helper function before REGEX use (exos 1 & 2)
	for (const auto& x : *m)
		if (int n = is_prefix(x, "Objet : ")) return string(x, n);
	return "";

}

string find_subject1(const Message* m, const regex pat)
{
	// Helper function with REGEX use (exo 3)
	smatch matches;

	for (const auto& x : *m)
		if (regex_search(x, matches, pat)) return string(x, 8);
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

		// Traitement du sender (exo 1 page 887)
		multimap<string, const Message*> sender;

		for (const auto& m:mfile) {
			string s;
			if (find_from_field(&m, s, "De : "))
				sender.insert(make_pair(s, &m));
		
		}
						
		auto pp = sender.equal_range("Hakima BRIHI <hbrihi@finalife.com>");
		for (auto p = pp.first; p != pp.second; ++p)
			cout << find_subject(p->second) << endl;

		
		// Traitement de l'objet (exo 2 page 887) ************************************************************
		
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
			for (const auto& x : *(p->second)) cout << x << endl;	// p->first contient l'objet recherché et p->second pointe sur le message lié (donc * permet de récupérer le contenu pointé)
		

		// REGEX on subject & sender instead of helper functions (exo 3 page 887) ************************************************************
		// On reprend la même trame que l'exo 1 afin de vérifier que les résultats sont comparables
		Mail_file mfile1{ "mail.txt" };
				
		multimap<string, const Message*> sender1;

		regex pat1{R"(De : \w+)"};
		for (const auto& m : mfile1) {
			string s;
			if (find_from_field1(&m, s, pat1))
				sender1.insert(make_pair(s, &m));

		}

		regex pat2{ R"(Objet : \w+)" };
		auto pp1 = sender1.equal_range("Hakima BRIHI <hbrihi@finalife.com>");
		for (auto p = pp1.first; p != pp1.second; ++p)
			cout << find_subject1(p->second, pat2) << endl;


		// Exo 6 page 887
		ifstream ist = open_file_read("banque.txt");
		cout << "****************************************************************************************************" << endl;
		cout << "Lecture du fichier bancaire" << endl;

		string s{};
		int line_number = 0;
		regex pat3{ R"(\d{2}/\d{2}/\d{4}|\d{2}/\d{2})" };
		while (!ist.eof()) {					// Tant que la fin du fichier n'est pas atteinte, on continue à extraire de nouveaux enregistrements
			getline(ist, s);					// On lit une ligne terminée par un NEWLINE
			++line_number;
			smatch matches;

			if (regex_search(s, matches, pat3)) cout << "ligne no " << setw(4)<<line_number << " : " << matches[0] << endl;
		}
		ist.close();

		
		// Exo final sur pharaon.txt
		ifstream ist1 = open_file_read("pharaon.txt");
		cout << "****************************************************************************************************" << endl;
		cout << "Lecture du fichier des abonnés Pharaon & récupération des e-mails" << endl;

		string s1{};
		regex pat4{ R"(\w+@\w+.(fr|com|net))" };
		while (!ist1.eof()) {					// Tant que la fin du fichier n'est pas atteinte, on continue à extraire de nouveaux enregistrements
			getline(ist1, s1);					// On lit une ligne terminée par un NEWLINE
			smatch matches{};

			if (regex_search(s1, matches, pat4)) cout << "Abonné no " << setw(4) << s1.substr(3,5) << " : " << matches[0] << endl;
		}
		
		cout << "****************************************************************************************************" << endl;
		cout << "Lecture du fichier des abonnés Pharaon & stockage des bonnes lignes dans une structure" << endl;
		vector<Pharaon_subscribers> ps{};
		
		ist1.seekg(0); // rewind to start of the file
		
		// Structure-type d'une ligne du fichier source : PH	11095	PH11095	ELODIE FONTENEAU	06500 MENTOU	laure2106@hotmail.com
		//regex pat5{ R"(^(PH)(\t\d{5})(\tPH\d{5})(\t\w+\s\w+)(\t\d{5}\s\w+)(\t\w+@\w+.(fr|com|net)))" }; // OK partiellement
		regex pat5{ R"(^(PH)(\t\d{5})(\tPH\d{5})(\t[-[:alpha:]]+\s[-[:alpha:]]+)(\t\d{5}\s[-[:alpha:]]+)(\t[-.[:alnum:]]+@\w+.(fr|com|net)))" }; // tiret et point acceptés (nom, mail..)
		// regex pat5{ R"(^(PH)(\t\d{5})(\tPH\d{5})(\t[-[:alpha:]]+\s[-[:alpha:]]+)(\t\d{5}\s[ [:alpha:]]+)(\t\w+@\w+.(fr|com|net)))" }; // L'espace est accepté dans la ville... 
		// ... mais la lecture du flux via stringstream qui se base sur les espaces de séparation ne fonctionne plus !

		string s2{};
		while (!ist1.eof()) {					// Tant que la fin du fichier n'est pas atteinte, on continue à extraire de nouveaux enregistrements
			getline(ist1, s2);					// On lit une ligne terminée par un NEWLINE
			smatch matches{};

			vector<string>ch(8);
			if (regex_search(s2, matches, pat5)) { 
				cout << s2 << endl; // affichage de contrôle
				stringstream ss(s2);
				ss >> ch[0] >> ch[1] >> ch[2] >> ch[3] >> ch[4] >> ch[5] >> ch[6] >> ch[7];
				ps.push_back({ch[1], ch[3],ch[4],ch[5],ch[6],ch[7] });
							
			}
		}
		
		for (auto x : ps)
			cout << x.num_abo << " - " << x.prenom << " - " << x.nom << " - " << x.code_postal << " - " << x.ville << " - " << x.mail << endl;
		
		
		ist1.close();



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
