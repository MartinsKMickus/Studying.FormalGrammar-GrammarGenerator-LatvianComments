// 2021_02_13 FormalasGramatikas_NMD1.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <fstream> // Saglabâðanai failâ
#include <vector> // Âtrai vârdu apstrâdei.
#include <set> // Koku veidoðanai, lai notiktu âtrâka apstrâde.
#include <conio.h> // Lietotâjam
#include <windows.h> // Skaòâm

using namespace std;

// Programmas izvade ir pierakstîta zem main bloka. Izvadç tiek attçlots arî iegûto vârdu skaits (DAÞÂDU Gala vârdu skaits: 1039482)
// Paði vârdi tiek saglabâti failâ tajâ paðâ mapç.
// Programmu viegli pielâgot citâm gramatikâm.


/// <summary>
/// Saistîtais saraksts int skaitïiem.
/// </summary>
class IntList
{
	struct Block
	{
		int value = 0;
		Block* next = NULL;
	};
	Block* First = NULL;
public: int count = 0, val = 0;
	  IntList(int count, int val)
	  {
		  Block* p;
		  First = new Block;
		  p = First;
		  p->value = val;
		  while (count-- > 1)
		  {
			  p->next = new Block;
			  p = p->next;
			  //p->value = 0;
		  }
	  }
	  int Get(int at)
	  {
		  auto p = First;
		  for (at--; at >= 0; at--)
		  {
			  p = p->next;
		  }

		  return p->value;
	  }
	  void Set(int at, int val)
	  {
		  auto p = First;
		  for (at--; at >= 0; at--)
		  {
			  p = p->next;
		  }
		  p->value = val;

	  }
	  void Add(int at)
	  {
		  auto p = First;
		  for (at--; at >= 0; at--)
		  {
			  p = p->next;
		  }
		  p->value++;
	  }
	  void Remove(int at)
	  {
		  auto p = First;
		  for (at--; at >= 0; at--)
		  {
			  p = p->next;
		  }
		  p->value--;
	  }
	  ~IntList()
	  {
		  for (auto p = First; First != NULL; p = First)
		  {
			  First = First->next;
			  delete p;
		  }
		  //cout << "List Destroyed!";
		  //cout << endl;
	  }
};
/// <summary>
/// Atïauj viegli pa vidu char virknei iespraust string.
/// </summary>
class MyList
{
	struct Block
	{
		char value = NULL;
		Block* next = NULL;
	};
	Block* First = NULL;
public: int size = 0;
	  MyList(string start)
	  {
		  auto b = new Block;
		  First = b;
		  b->value = start[0];
		  size++;
		  for (int pos = 1; pos < start.size(); pos++)
		  {
			  b->next = new Block;
			  b = b->next;
			  b->value = start[pos];
			  size++;
		  }
	  }
	  string Print()
	  {
		  string product;
		  for (auto p = First; p != NULL; p = p->next)
		  {
			  product += p->value;
			  //cout << p->value;
		  }
		  return product;
	  }
	  char Get(int at)
	  {
		  auto p = First;
		  for (at--; at >= 0; at--)
		  {
			  p = p->next;
		  }
		  return p->value;
	  }
	  void Replace(int at, string rep)
	  {
		  auto p = First, temp = First->next;

		  for (at--; at >= 0; at--)
		  {
			  p = p->next;
		  }
		  temp = p->next;
		  p->value = rep[0];

		  for (int pos = 1; pos < rep.size(); pos++)
		  {
			  p->next = new Block;
			  p = p->next;
			  p->value = rep[pos];
			  size++;
		  }
		  p->next = temp;
	  }
	  ~MyList()
	  {
		  for (auto p = First; First != NULL; p = First)
		  {
			  First = First->next;
			  delete p;
		  }
		  //cout << "List Destroyed!";
		  //cout << endl;
	  }
};

class FreeContextGrammar
{
public: int ReadyWords = 0, NewWordsEnd = 0, TreeDepth = 0;
	  int LetterCount = 0, RuleCount = 0, AllowedSize = 0;
	  bool developer = false, fast = true;
	  vector<string> Words;
	  set<string> FastWords, FinalWords;
	  vector<int> Rsizes;
	  ofstream wordsF;
public: vector<vector<string>> rules, Rules = vector<vector<string>>();
	  /// <summary>
	  /// Konstruç gramatikas vârdu ìeneratoru.
	  /// </summary>
	  /// <param name="Start">Vârds ar kuru sâkt.</param>
	  /// <param name="rules">Noteikumi sarakstâ, kas satur sarakstus ar string (pirmie simboli apzîmç neterminâlo simbolu, pçc tam paði noteikumi attiecîgajiem simboliem).</param>
	  /// <param name="allows">Cik garus gala vârdus drîkst ìenerçt.</param>
	  /// <param name="dev">Izdrukâ gala tipa vârdu, kad tas tiek atrasts.</param>
	  FreeContextGrammar(string Start, vector<vector<string>> rules, int allows = 20, bool dev = false, bool F = true)
	  {
		  fast = F;
		  AllowedSize = allows;
		  Words.push_back(Start);

		  developer = dev;

		  int searchCycle = 0, smallest = allows;
		  for(int pos = 0; pos<rules.size();pos++)
		  {
			  Rules.push_back(rules[pos]);
			  //searchCycle = 0;
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
				  //searchCycle++;
			  }
			  Rsizes.push_back(smallest);
		  }

		  NewWordsEnd++;
	  }
	  /// <summary>
	  /// Ieðana pa slâni dziïâk. Tiek saglabâti nâkamie JAUNIE vârdi. Viena no galvenajâm funkcijâm.
	  /// </summary>
	  void GoDeeper()
	  {
		  bool spoiledLong = false;
		  vector<int> divide; // Reprezentçs visa veida noteikumu pârveidojumus (Piemçram: 4*4*4*4, kad vârds ir (E)+(E+E)*E)
		  char rule;
		  string w;
		  int ruleloop = 0, sruleloop = 0;// , ruleCount = 0, letterCount = 0;
		  vector<string> extractedRules;
		  int MaxDepthJump = 1;
		  for (int pos = ReadyWords; pos < NewWordsEnd; pos++) // Apstrâdâti vârdi un jaunu vârdu sâkums.
		  {


			  w = Words[pos];
			  if (w == "E+E+E+E")
			  {
				  cout << "Hi";
			  }


			  for (int innerrulepos = 0; innerrulepos < Rules.size(); innerrulepos++) // jâiziet caur katru gramatikas notiekumu neterminâlajiem simboliem.
			  {
				  rule = Rules[innerrulepos].front()[0]; // Atrodam attiecîgos noteikumus. Pirmais simbols neterminâlais simbols.
				  RuleCount = Rules[innerrulepos].size() - 1;

				  for (int pos = 0; pos < Rules[innerrulepos].size(); pos++)
				  {
					  if (pos > 0) // Noteikumos nerakstîsim paðu neterminâlo simbolu.
					  {
						  extractedRules.push_back(Rules[innerrulepos][pos]);
					  }
					  //sruleloop++;
				  }
				  //sruleloop = 0;

				  for (int innerpos = 0; innerpos < w.size(); innerpos++)
				  {
					  if (w[innerpos] == rule) // Izskaitâm cik reizes konkrçtais neterminâlais simbols parâdâs vârdâ.
					  {
						  LetterCount++;
					  }
				  }
				  if (LetterCount > 0)
				  {
					  //IntList* divide = new IntList(LetterCount, -1); // Paðtaisîta datu struktûra.
				  //for (int change = 0; ; change = 0)
				  //{
				  //	divide->Add(change);
				  //	if (divide->Get(change) < RuleCount)
				  //	{
				  //		//change = -1;
				  //	}
				  //	else
				  //	{
				  //		/*divide->Set(change, 0);
				  //		divide->Add(++change);*/
				  //		while (divide->Get(change) >= RuleCount - 1 && change < LetterCount - 1)
				  //		{
				  //			divide->Set(change, 0);
				  //			divide->Add(++change);
				  //		}
				  //		/*if (change == LetterCount - 1 && divide->Get(change) == RuleCount - 1)
				  //		{
				  //			divide->Add(change);
				  //		}*/
				  //		if (divide->Get(LetterCount - 1) > RuleCount - 1)
				  //		{
				  //			/*_beep(500, 500);*/
				  //			break;
				  //		}
				  //			
				  //	}
				  //	GetWords(w, rule, extractedRules, divide);
				  //	//Print();
				  //	//cout << endl;
				  //}
					  
					  if (fast)
					  {
						  
						  if (MaxDepthJump < LetterCount) MaxDepthJump = LetterCount;
						  divide = vector<int>(LetterCount); // Izmantojot vektoru ìenerçsim variantus => 000, 100, 200, 300, 010, 020,... (Lai visos variantos uzìenerçtu jaunus vârdus.)
						  divide[0] = -1;
						  for (int p = 1; p < LetterCount; p++)
						  {
							  divide[p] = 0;
						  }
						  for (int change = 0; ; change = 0)
						  {
							  divide[change]++;
							  if (divide[change] < RuleCount)
							  {
								  //change = -1;
							  }
							  else
							  {
								  while (divide[change] >= RuleCount && change < LetterCount - 1)
								  {
									  divide[change] = 0;
									  divide[++change]++;
								  }
								  if (divide[LetterCount - 1] > RuleCount - 1)
								  {
									  break;
								  }

							  }
							  if (!spoiledLong)
							  {
								  spoiledLong = GetWords(w, rule, extractedRules, divide);
							  }
							  else
							  {
								  GetWords(w, rule, extractedRules, divide);
							  }
						  }
						  
					  }
					  else
					  {
						  for (int pos = 0; pos < LetterCount; pos++)
						  {
							  for (int p = 0; p < RuleCount; p++)
							  {
								  GetAllWords(w, rule, extractedRules, pos, p);
							  }
						  }
					  }
				  }
				  extractedRules.clear();
				  LetterCount = 0;
				  RuleCount = 0;

			  }
			  ReadyWords++;
		  }

		  //NewWordsStart = Words->size;
		  NewWordsEnd = Words.size();
		  if (spoiledLong) MaxDepthJump = 1;
		  TreeDepth+= MaxDepthJump;
	  }
	  /// <summary>
	  /// Vârda pârbaude uz garumu pçc nâkamâ pârveidojuma.
	  /// </summary>
	  /// <param name="w">Vârds kuru pârbaudît.</param>
	  /// <returns>Atgrieþ mazâko iespçjamo vârda garumu òemot vçrâ tikai vârdâ esoðos neterminâlos simbolus.</returns>
	  int PassLong(string w)
	  {
		  int Rsize = 0;
		  Rsize = w.size();
		  for(int pos = 0; pos < Rules.size(); pos++)
		  {
			  for (int p = 0; p < w.size(); p++)
			  {
				  if (w[p] == Rules[pos].front()[0])
				  {
					  Rsize += Rsizes[pos] - 1;
				  }
			  }
		  }
		  return Rsize;
	  }
	  /// <summary>
	  /// Pârbaude vai vârds jau nav uzìenerçts.
	  /// </summary>
	  /// <param name="w">Vârds kuru pârbaudît.</param>
	  void GetFinWords(string w)
	  {
		  int Rsize = 0;
		  Rsize = w.size();
		  for (int pos = 0; pos < Rules.size(); pos++)
		  {
			  for (int p = 0; p < w.size(); p++)
			  {
				  if (w[p] == Rules[pos].front()[0])
				  {
					  return;
				  }
			  }
		  }
		  if (developer) cout << w << endl;
		  FinalWords.insert(w);
	  }

	  /// <summary>
	  /// Viena no galvenajâm funkcijâm. Jaunu vârdu iegûðana.
	  /// </summary>
	  /// <param name="w">Vârds no kura jâiegûst vârdi.</param>
	  /// <param name="rule">Kurð neterminâlais simbols jâòem vçrâ.</param>
	  /// <param name="extrules">Neterminâlâ simbola instrukcijas.</param>
	  /// <param name="divide">Kuras instrukcijas jâizmanto pçc kârtas vârdam atrastajâ simbolâ. (Ja vârdâ ir, piemçram, èetri attiecîgie neterminâlie simboli, tad arî instrukcijas bûs pa èetrâm)</param>
	  bool GetWords(string w, char rule, vector<string> extrules, vector<int> divide)
	  {
		  MyList* RepW = new MyList(w);
		  bool SpoiledLong = false;
		  int letPos = 0, rulecycle = 0;
		  for (int p = 0; letPos < LetterCount; p++) // p - Pârbaudâmâ burta pozîcija. letPos - nomainîto simbolu skaits.
		  {
			  if (RepW->Get(p) == rule) // Ja vârdâ tiek atrasts simbols.
			  {
				  RepW->Replace(p, extrules[divide[letPos]]);
				  p += extrules[divide[letPos]].size() - 1;
				  letPos++;
			  }
		  }

		  SpoiledLong = AddWord(RepW->Print());
		  delete RepW;
		  return SpoiledLong;

	  }
	  void GetAllWords(string w, char rule, vector<string> extrules, int divide, int repRule)
	  {
		  MyList* RepW = new MyList(w);
		  int letPos = 0, rulecycle = 0;
		  for (int p = 0; letPos < LetterCount; p++) // p - Pârbaudâmâ burta pozîcija. letPos - nomainîto simbolu skaits.
		  {
			  if (RepW->Get(p) == rule) // Ja vârdâ tiek atrasts simbols.
			  {
				  if (letPos == divide)
				  {
					  RepW->Replace(p, extrules[repRule]);
					  break;
				  }
				  letPos++;
			  }
		  }

		  AddWord(RepW->Print());
		  delete RepW;

	  }
	  bool AddWord(string Word)
	  {

		  if (PassLong(Word) > AllowedSize) return true;


		  if (FastWords.find(Word) != FastWords.end())
		  {
			  //cout << FastWords.find(Word)->c_str() << endl;
			  //Sleep(3000);
			  return false;
		  }
		  //wordsF << Word/* +"       " << Words.size()*/ << endl; // Atkïûdoðanai

		  Words.push_back(Word);
		  FastWords.insert(Word);
		  GetFinWords(Word);
		  return false;
		  //cout << Word + "       " << Words.size() << endl; // Atkïûdoðanai
		  //cout << endl;
	  }
	  /// <summary>
	  /// Failâ saglabâ un izdrukâ iegûtos vârdus.
	  /// </summary>
	  /// <param name="ready">Saglabât tikai terminâlos vârdus.</param>
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
					  //cout << pair.c_str() << endl;
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
					  //cout << pair.c_str() << endl;
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
	bool dev = false, fast = true;
	vector<string> rules;
	cout << "Enter Terminal Symbol: ";
	auto button = _getch();
	cout << endl;
	string rule, start = "";
	while (button != ' ')
	{
		rules.push_back(string(1, button));
		cout << "Enter Rule for " << string(1, button) << " and press enter: ";
		cin >> rule;
		while (rule != "~")
		{
			rules.push_back(rule);
			cout << "Enter next Rule (To stop, enter ~): ";
			cin >> rule;
		}
		rul.push_back(rules);
		rules.clear();
		cout << "Enter next Terminal Symbol (To stop press space): ";
		button = _getch();
		cout << endl;
	}
	cout << "Enter Start Word: ";
	cin >> start;
	cout << "Show final words on screen (when generating)? y/n " << endl;
	cout << endl;
	button = _getch();
	if (button == 'y') dev = true;
	cout << "Use slower generator to get all words with no terminal symbols? (Must be on for correct total words generation)" << endl;
	cout << "Does not affect final terminal word count. Recommended off!!! (n) y/n" << endl;
	button = _getch();
	if (button == 'y') fast = false;

	cout << endl;
	cout << endl;
	//Sleep(2000);
	cout << "WARNING!!! LIST OF FINAL WORDS ARE SAVED ONLY AFTER EXECUTION. Process could be long!";
	Sleep(2000);
	cout << endl;
	cout << endl;
	cout << "Press any button to begin! "; // Izveidots apstâðanâs punkts, lai var, piemçram uzòemt laiku.
	_getch();
	cout << endl;
	cout << endl;
	FreeContextGrammar grammargen = FreeContextGrammar(start, rul, 15, dev, fast);

	cout << "---------------------------------------------------------------" << endl;
	while (grammargen.ReadyWords < grammargen.NewWordsEnd) // Kad vairs nebûs pieejami jauni vârdi.
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

//Programmas izvade (Release gadîjumâ. Galveno funkciju darbîbas laiks ~18sec. Procesors: i7-8700K 3.70GHz. Atmiòa: 32GB):

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