// 2021_02_13 FormalasGramatikas_NMD1.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <fstream> // Saglab��anai fail�
#include <list>
#include <vector> // �trai v�rdu apstr�dei.
#include <set> // Koku veido�anai, lai notiktu �tr�ka apstr�de.
#include <conio.h> // Lietot�jam
#include <windows.h> // Ska��m

using namespace std;

// Programmas izvade ir pierakst�ta zem main bloka. Izvad� tiek att�lots ar� ieg�to v�rdu skaits (DA��DU Gala v�rdu skaits: 1039482)
// Pa�i v�rdi tiek saglab�ti fail� taj� pa�� map�.
// Programmu viegli piel�got cit�m gramatik�m.


/// <summary>
/// At�auj viegli pa vidu char virknei iespraust string.
/// </summary>


class FreeContextGrammar
{
public: int ReadyWords = 0, NewWordsEnd = 0, TreeDepth = 0;
	  int LetterCount = 0, RuleCount = 0, AllowedSize = 0;
	  bool developer = false;
	  list<string> FinalWords;
	  vector<string> Words;
	  set<string> FastWords;
	  vector<int> Rsizes;
	  ofstream wordsF;
public: vector<vector<string>> rules, Rules = vector<vector<string>>();
	  /// <summary>
	  /// Konstru� gramatikas v�rdu �eneratoru.
	  /// </summary>
	  /// <param name="Start">V�rds ar kuru s�kt.</param>
	  /// <param name="rules">Noteikumi sarakst�, kas satur sarakstus ar string (pirmie simboli apz�m� netermin�lo simbolu, p�c tam pa�i noteikumi attiec�gajiem simboliem).</param>
	  /// <param name="allows">Cik garus gala v�rdus dr�kst �ener�t.</param>
	  /// <param name="dev">Izdruk� gala tipa v�rdu, kad tas tiek atrasts.</param>
	  FreeContextGrammar(string Start, vector<vector<string>> rules, int allows = 20, bool dev = false)
	  {
		  AllowedSize = allows;
		  Words.push_back(Start);
		  developer = dev;
		  int searchCycle = 0, smallest = allows;
		  for(int pos = 0; pos<rules.size();pos++)
		  {
			  Rules.push_back(rules[pos]);		  
			  smallest = allows;
			  for(int pos1 = 0; pos1<rules[pos].size();pos1++)
			  {
				  if (pos1 > 0)
				  {
					  if (rules[pos][pos1].size() < smallest)
					  {
						  smallest = rules[pos][pos1].size();
					  }
				  }
			  }
			  Rsizes.push_back(smallest);
		  }
		  NewWordsEnd++;
	  }
	  /// <summary>
	  /// Ie�ana pa sl�ni dzi��k. Tiek saglab�ti n�kamie JAUNIE v�rdi. Viena no galvenaj�m funkcij�m.
	  /// </summary>
	  void GoDeeper()
	  {
		  char rule;
		  string w;
		  for (int pos = ReadyWords; pos < NewWordsEnd; pos++) // Apstr�d�ti v�rdi un jaunu v�rdu s�kums.
		  {
			  w = Words[pos];

				  for (int innerrulepos = 0; innerrulepos < Rules.size(); innerrulepos++) // j�iziet caur katru gramatikas notiekumu netermin�lajiem simboliem.
				  {
					  rule = Rules[innerrulepos].front()[0]; // Atrodam attiec�gos noteikumus. Pirmais simbols netermin�lais simbols.
					  RuleCount = Rules[innerrulepos].size() - 1;
					  
					  for (int innerpos = 0; innerpos < w.size(); innerpos++)
					  {
						  if (w[innerpos] == rule) // Izskait�m cik reizes konkr�tais netermin�lais simbols par�d�s v�rd�.
						  {
							  LetterCount++;
						  }
					  }
					  if (LetterCount > 0)
					  {
						  for (int pos = 0; pos < 1; pos++)
						  {
							  for (int p = 0; p < RuleCount; p++)
							  {
								  GetAllWords(w, rule, Rules[innerrulepos], p);
							  }
						  }
					  }
					  LetterCount = 0;
					  RuleCount = 0;

				  }
			  ReadyWords++;
		  }
		  NewWordsEnd = Words.size();
		  TreeDepth++;
	  }
	  /// <summary>
	  /// V�rda p�rbaude uz garumu p�c n�kam� p�rveidojuma.
	  /// </summary>
	  /// <param name="w">V�rds kuru p�rbaud�t.</param>
	  /// <returns>Atgrie� maz�ko iesp�jamo v�rda garumu �emot v�r� tikai v�rd� eso�os netermin�los simbolus.</returns>
	  int PassLong(string& w)
	  {
		  int Rsize = w.size();
		  char test;
		  //Rsize = w.size();
		  for(int pos = 0; pos < Rules.size(); pos++)
		  {
			  test = Rules[pos].front()[0];
			  for (int p = 0; p < w.size(); p++)
			  {
				  if (w[p] == test)
				  {
					  Rsize += Rsizes[pos] - 1;
				  }
			  }
		  }
		  return Rsize;
	  }
	  /// <summary>
	  /// P�rbaude vai v�rd� ir netermin�lie simboli.
	  /// </summary>
	  /// <param name="w">V�rds kuru p�rbaud�t.</param>
	  void GetFinWords(string& w)
	  {
		  int Rsize = w.size();
		  //Rsize = w.size();
		  char test;
		  for (int pos = 0; pos < Rules.size(); pos++)
		  {
			  test = Rules[pos].front()[0];
			  for (int p = 0; p < Rsize; p++)
			  {
				  if (w[p] == test)
				  {
					  FastWords.emplace(w);
					  return;
				  }
			  }
		  }
		  if (developer) cout << w << endl;
		  FinalWords.emplace_back(w);
	  }

	  /// <summary>
	  /// Viena no galvenaj�m funkcij�m. Jaunu v�rdu ieg��ana.
	  /// </summary>
	  /// <param name="w">V�rds no kura j�ieg�st v�rdi.</param>
	  /// <param name="rule">Kur� netermin�lais simbols j��em v�r�.</param>
	  /// <param name="extrules">Netermin�l� simbola instrukcijas.</param>
	  /// <param name="repRule">Kur� i-tais noteikums j�izmanto</param>
	  void GetAllWords(string w, char& rule, vector<string>& extrules, int& repRule)
	  {
		  int WSize = w.size();
		  for (int p = 0; p < WSize; p++) // p - P�rbaud�m� burta poz�cija. letPos - nomain�to simbolu skaits.
		  {
			  if (w[p] == rule) // Ja v�rd� tiek atrasts simbols.
			  {
					  w.replace(p, 1, extrules[repRule + 1]);
					  break;
			  }
		  }
		  AddWord(w);
	  }
	  void AddWord(string& w)
	  {		  
		  if (PassLong(w) > AllowedSize) return;

		  if (FastWords.find(w) != FastWords.end())
		  {
			  return;
		  }

		  Words.emplace_back(w);
		  GetFinWords(w);
	  }
	  /// <summary>
	  /// Fail� saglab� un izdruk� ieg�tos v�rdus.
	  /// </summary>
	  /// <param name="ready">Saglab�t tikai termin�los v�rdus.</param>
	  void Print(bool screen = false, bool ready = true)
	  {
		  wordsF.open("Words.txt");
		  if (ready)
		  {
			  if (screen)
			  {
				  for (auto const& pair : FinalWords) 
				  {
					  cout << pair.c_str() << endl;
					  wordsF << pair.c_str() << endl;
				  }
			  }
			  else
			  {
				  cout << "Please wait! Saving words to file..." << endl;
				  for (auto const& pair : FinalWords)
				  {
					  wordsF << pair.c_str() << endl;
				  }
			  }
		  }
		  else
		  {
			  if (screen)
			  {
				  for (auto const& pair : FastWords)
				  {
					  cout << pair.c_str() << endl;
					  wordsF << pair.c_str() << endl;
				  }
			  }
			  else
			  {
				  cout << "Please wait! Saving words to file..." << endl;
				  for (auto const& pair : FastWords)
				  {
					  wordsF << pair.c_str() << endl;
				  }
			  }
		  }
		  wordsF << "---------------------------------------------------------------" << endl;
		  wordsF << " Terminal words : " << TermWCount() << endl;
		  wordsF << " Total words : " << NewWordsEnd << endl;
		  cout << "---------------------------------------------------------------" << endl;
		  cout << endl;
		  cout << " Terminal words : " << TermWCount() << endl;
		  cout << " Total words : " << NewWordsEnd << endl;
		  cout << endl;
		  cout << "Generated: Words.txt" << endl;
		  wordsF.close();
	  }
	  int TermWCount()
	  {
		  return FinalWords.size();
	  }
};
int main()
{
	vector<vector<string>> rul = vector<vector<string>>(); // Instrukcijas par gramatiku.
	bool dev = false;
	vector<string> ru2;
	ru2.push_back("E");
	ru2.push_back("E+E");
	ru2.push_back("E*E");
	ru2.push_back("(E)");
	ru2.push_back("xI");
	vector<string> ru3;
	ru3.push_back("I");
	ru3.push_back("0I");
	ru3.push_back("1I");
	ru3.push_back("0");
	ru3.push_back("1");
	rul.push_back(ru2);
	rul.push_back(ru3);
	cout << "Show final words on screen (when generating)? y/n " << endl;
	cout << endl;
	auto button = _getch();
	if (button == 'y') dev = true;
	cout << "WARNING!!! LIST OF FINAL WORDS ARE SAVED ONLY AFTER EXECUTION. Process could be long!";
	Sleep(2000);
	cout << endl;
	cout << endl;
	cout << "Press any button to begin! "; // Izveidots apst��an�s punkts, lai var, piem�ram uz�emt laiku.
	_getch();
	cout << endl;
	cout << endl;
	FreeContextGrammar grammargen = FreeContextGrammar("E", rul, 15, dev);

	cout << "---------------------------------------------------------------" << endl;
	while (grammargen.ReadyWords < grammargen.NewWordsEnd) // Kad vairs neb�s pieejami jauni v�rdi.
	{
		//cout << endl;


		cout << "Depth: " << grammargen.TreeDepth << endl;
		cout << " Different words: " << grammargen.NewWordsEnd << endl;
		cout << " New words found: " << grammargen.NewWordsEnd - grammargen.ReadyWords << endl;
		cout << " Terminal words : " << grammargen.TermWCount() << endl;
		//Beep(130.81, 500);
		cout << "---------------------------------------------------------------" << endl;
		grammargen.GoDeeper();

	}
	bool ready = true;
	cout << "Print Final words on screen? y/n " << endl;
	button = _getch();
	if (button == 'y') dev = true;
	else dev = false;
	cout << "Save and/or print also words that contains Not Terminal symbols? y/n " << endl;
	button = _getch();
	if (button == 'y') ready = false;


	grammargen.Print(dev, ready);

	cout << endl;
	cout << "Program will exit in: ";
	for (int pos = 20; pos > 0; pos--)
	{
		cout << pos << " ";
		Sleep(1000);
	}

	return 0;
}

//Programmas izvade (Release gad�jum�. Galveno funkciju darb�bas laiks ~18sec. Procesors: i7-8700K 3.70GHz. Atmi�a: 32GB):

//Show final words on screen(when gen
//
//	Use slower generator to get all word
//	Does not affect final terminal word
//
//
//	WARNING!!!LIST OF FINAL WORDS ARE S
//
//	Press any button to begin!
//
//	------------------------------------
//	Depth: 0
//	Different words : 1
//	New words found : 1
//	Terminal words : 0
//	------------------------------------
//	Depth : 1
//	Different words : 5
//	New words found : 4
//	Terminal words : 0
//	------------------------------------
//	Depth : 3
//	Different words : 45
//	New words found : 40
//	Terminal words : 2
//	------------------------------------
//	HiDepth : 4
//	Different words : 685
//	New words found : 640
//	Terminal words : 16
//	------------------------------------
//	Depth : 5
//	Different words : 11595
//	New words found : 10910
//	Terminal words : 350
//	------------------------------------
//	Depth : 6
//	Different words : 112683
//	New words found : 101088
//	Terminal words : 17476
//	------------------------------------
//	Depth : 7
//	Different words : 457023
//	New words found : 344340
//	Terminal words : 148666
//	------------------------------------
//	Depth : 8
//	Different words : 994714
//	New words found : 537691
//	Terminal words : 366520
//	------------------------------------
//	Depth : 9
//	Different words : 1454710
//	New words found : 459996
//	Terminal words : 611150
//	------------------------------------
//	Depth : 10
//	Different words : 1738922
//	New words found : 284212
//	Terminal words : 778506
//	------------------------------------
//	Depth : 11
//	Different words : 1906938
//	New words found : 168016
//	Terminal words : 885242
//	------------------------------------
//	Depth : 12
//	Different words : 2003002
//	New words found : 96064
//	Terminal words : 951738
//	------------------------------------
//	Depth : 13
//	Different words : 2057530
//	New words found : 54528
//	Terminal words : 992442
//	------------------------------------
//	Depth : 14
//	Different words : 2087226
//	New words found : 29696
//	Terminal words : 1015994
//	------------------------------------
//	Depth : 15
//	Different words : 2107706
//	New words found : 20480
//	Terminal words : 1028282
//	------------------------------------
//	Depth : 16
//	Different words : 2124090
//	New words found : 16384
//	Terminal words : 1044666
//	------------------------------------
//	Print Final words on screen ? y / n
//	Save and /or print also words that co
//	Please wait!Saving words to file...
//	------------------------------------
//
//	Terminal words : 1044666
//	Total words : 2124090
//
//	Generated : Words.txt
//
//	Program will exit in : 20 19 18 17 16