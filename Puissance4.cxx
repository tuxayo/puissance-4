#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

#define classdef typedef

namespace
{
	classdef vector <char> CVLigne;
	typedef CVLigne::size_type IndLi_t;
	classdef vector <CVLigne> CVMatrice;
	typedef CVMatrice::size_type IndMat_t;

	void ClearScreen ()
	{
		cout << "\033[H\033[2J";
	} // Clear Screen ()

	const string KReset   ("0");
	const string KNoir    ("30");
	const string KRouge   ("31");
	const string KVert    ("32");
	const string KJaune   ("33");
	const string KBleu    ("34");
	const string KMAgenta ("35");
	const string KCyan    ("36");

	string J1;
	string J2;

	void Couleur (const string & coul)
	{
		cout << "\033[" << coul.c_str () <<"m";
	} // Couleur ()

	void ChoixCouleur (const CVLigne & Li, unsigned i)
    {
        if (Li [i] == 'X')
        {
            Couleur (KJaune); // Pions joueur 1
        }
        else if (Li [i] == 'O')
        {
            Couleur (KRouge); // Pions joueur 2
        }
    } // ChoixCouleur ()

	void AfficheLigne (const CVLigne & Li)
	{
		Couleur(KBleu);
		cout << "| ";
		for( unsigned i = 0; i < Li.size(); ++i)
		{
			ChoixCouleur(Li, i);
            cout << Li [i];
            Couleur (KBleu);
            cout << " | ";
		}
		cout << "\n ——— ——— ——— ——— ——— ——— ———\n";
        Couleur (KReset);
	} // AfficheLigne ()

	void AfficheMatrice (const CVMatrice & Mat)
	{
		ClearScreen ();
		Couleur (KReset);
		for (unsigned i = 0; i < Mat.size(); ++i)
		{
			AfficheLigne (Mat [i]);
		}
	} // AfficheMatrice ()

	void AffichePuissance4 (const CVMatrice & Mat)
	{
		cout << endl;
		AfficheMatrice (Mat);
		char NomColonne = 'A';
		cout << "  " << NomColonne;
		++NomColonne;
		for (unsigned i = 1; i < 7; ++i)
		{
			cout << "   " << NomColonne;
			NomColonne +=1;
		}
	} // AffichePuissance4 ()

	void AfficheScore (unsigned Point1, unsigned Point2)
	{
	    Couleur(KJaune);
	    cout << "\n  ________         ";
	    Couleur(KRouge);
	    cout << "________";
	    Couleur(KJaune);
	    cout << "\n |" << J1 << "|       ";
	    Couleur(KRouge);
	    cout  << '|'<< J2 << "|";
	    Couleur(KJaune);
	    cout << "\n |   " << Point1 << "    |  ";
	    Couleur(KReset);
	    cout << "VS   ";
	    Couleur(KRouge);
	    cout << "|   " << Point2 << "    |";
	    Couleur(KJaune);
	    cout << "\n |________|       ";
	    Couleur (KRouge);
	    cout << "|________|";
	    Couleur(KReset);
	} // AfficheScore ()

	void InitMat (CVMatrice & Mat)
	{
		Mat.resize (7);
		for (IndMat_t IndLi (0); IndLi < Mat.size(); ++IndLi)
		{
			Mat [IndLi].resize (7);
			for (IndLi_t IndCol (0); IndCol < Mat [IndLi].size(); ++IndCol)
			{
				Mat [IndLi][IndCol] = ' ';
			}
		}

	} // InitMat ()

	string Pseudo (unsigned n)
	{
		string Str;
		cout << "Saisir le Pseudo du Joueur " << n << " : ";
		getline(cin,Str);
		while (Str.size()>8)
		{
			cout << "Seulement 8 caractères maximums svp" << endl;
			cout << "Nouveau Pseudo : ";
			getline(cin,Str);
		}
		if (Str.size()<8)
		{
			string S1(((8-Str.size())%2+((8-Str.size())/2)),' ');
			string S2((8-Str.size())/2,' ');	//string avec le nombre d'espace pour completer
			Str=S2+Str+S1;

		}
		return Str;
	} //Pseudo ()

	void PositionneJeton (CVMatrice & Mat, const unsigned NumCol,
				unsigned & NumLi, const bool CoupDuJoueur1,
			    unsigned & NbCoups1, unsigned & NbCoups2)
	{
			NumLi = 6;
			while (true)
			{
				if ((Mat [NumLi][NumCol] == ' ') /*& (NumLi!=0)*/)
				{
					if (CoupDuJoueur1)
					{
						Mat [NumLi][NumCol] = 'X';
						++NbCoups1;
						break;
					}
					else
					{
						Mat [NumLi][NumCol] = 'O';
						++NbCoups2;
						break;
					}
				}
				else
				{
					if (NumLi == 0)
					{
						break;
					}
					--NumLi;
				}
			} // End while
	} // PositionneJeton ()

	bool TestVictoireColonne (const CVMatrice & Mat, const unsigned NumLi, const unsigned NumCol, const bool CoupDuJoueur1)
	{
		if (NumLi > 3) return 0; // Si la colonne comporte moins de 3 pions, pas la peine de tester
		char CaracRecherche;
		if (CoupDuJoueur1)
		{
			CaracRecherche = 'X';
		}
		else
		{
			CaracRecherche = 'O';
		}
		unsigned Ligne = NumLi + 1;
		while ((Ligne < Mat.size()) && Mat [Ligne][NumCol] == CaracRecherche)
		{
			++Ligne;
		}
		// cout << (4 == Ligne-NumLi);
		return (4 == Ligne - NumLi);
	} // TestVictoireColonne ()

    int maximum(int a,int b)
    {
        if(a>b)
        return a;
        else
        return b;
    } // Maximum ()

	bool TestVictoireLigne (const CVMatrice & Mat, const unsigned NumLi, const unsigned NumCol, const bool CoupDuJoueur1)
	{
		char CaracRecherche;
		if (CoupDuJoueur1)
		{
			CaracRecherche = 'X';
		}
		else
		{
			CaracRecherche = 'O';
		}
		unsigned NbInspections;
		if (NumCol < Mat.size() /2)
		{
		    NbInspections = NumCol + 1;
		}
		else
		{
		    NbInspections = Mat.size () - NumCol;
		}
		int PosDepart = maximum (0, int(NumCol - Mat.size () /2));
		for (unsigned i = 0;  i < NbInspections; ++i)
		{
			unsigned NbCorrespondances = 0;
			for (unsigned Pos = PosDepart + i ; (Pos < Mat.size ()) &&  (CaracRecherche == Mat [NumLi][Pos]) ; ++Pos)
				++ NbCorrespondances;

			if (4 == NbCorrespondances) return true;
		}
		return false;
	} // TestVictoireLigne ()

	bool TestVictoireDiagonaleCroissante (const CVMatrice & Mat, const bool CoupDuJoueur1)
	{
		char Carac;
		if (CoupDuJoueur1)
		{
			Carac = 'X';
		}
		else
		{
			Carac = 'O';
		}

		unsigned NumLiT (3);
		unsigned NumColT (0);
		unsigned NBCorespondances (0);

		while (true)
		{

			while (Mat [NumLiT][NumColT] == Carac)
			{
				if (NBCorespondances == 4) return true;
				--NumLiT;
				++NumColT;


				if (NumLiT == 0) break;
				++ NBCorespondances;
			} // END WHILE 2


			++NumLiT;
			if (NumLiT == 6) return false;
		} // END WHILE 1

	} // TestVictoireDiagonaleCroissante ()

	int ppal ()
	{
		unsigned NbCoups1 = 0;
		unsigned NbCoups2 = 0;
		CVLigne L;          //
		L.reserve (7);      //
		CVMatrice M (7, L); // Déclaration d'une matrice M7 vide
		InitMat (M);        // Remplissage avec des '.'
		AffichePuissance4 (M); // Affichage de la matrice accompagnée de '-' et des noms des colonnes
	    unsigned Point1 = 0;
		unsigned Point2 = 0;
		AfficheScore (Point1, Point2); // Ici le score vaut toujours O à O
		unsigned NumLigne;
		char Choix;
		bool Joueur = true;
		for (unsigned NbCase = 0; ; ++NbCase)
		{
			string Saisie;	//Variable qui stockera ce qui est saisie au clavier
			string NomJoueur;
			while (true)
			{
				if((!Joueur + 1) ==1)
				{
					NomJoueur = J1;
				}
				else
				{
					NomJoueur = J2;
				}
				//cout << "\n\nJoueur " << !Joueur + 1 << "  :  Dans quelle colonne voulez vous placer le pion? ";
				cout << "\n\n"<< NomJoueur << "  :  Dans quelle colonne voulez vous placer le pion? ";
				getline(cin,Saisie);
				Choix = Saisie [0];		//On garde que le premier caractère saisi
				Choix = toupper(Choix);
				//if (Choix == ('A' | 'B' | 'C' | 'D' | 'E' | 'F' | 'G')) break; --NE MARCHE PAS-- :(
				if (Choix == 'A') break;
				if (Choix == 'B') break;
				if (Choix == 'C') break;
				if (Choix == 'D') break;
				if (Choix == 'E') break;
				if (Choix == 'F') break;
				if (Choix == 'G') break;
				cout << "La colonne n'existe pas" << endl;
			} // END TRUE
			PositionneJeton (M, Choix - 'A', NumLigne, Joueur, NbCoups1, NbCoups2); // Placement du pion dans la matrice
			if ((TestVictoireColonne (M, NumLigne, Choix - 'A', Joueur)) ||
				(TestVictoireLigne (M, NumLigne, Choix - 'A', Joueur)) ||
				(TestVictoireDiagonaleCroissante(M, Joueur)))
				//Tests de victoire
			{
				if (Joueur)
				{
				    ++Point1;
				}
				else
				{
				    ++Point2;
				}
				InitMat (M); //Réinitialisation la grille
			}

			AffichePuissance4 (M); //Actualisation de la grille après coups
			AfficheScore (Point1, Point2); // Affichage du score
			if(Point1 == 3)
			{
				cout << "\n\n" << NomJoueur << "a gagné la partie en " << NbCoups1
					 << " coups!\n";
				break; // Fin de la partie
			}
			if(Point2 == 3)
			{
			    cout << "\n\n" << NomJoueur << "a gagné la partie en " << NbCoups2
					 << " coups!\n";
			    break; //Fin de la partie
			}
			Joueur = !Joueur;
			//cout << M.size();
			//cout << NumLigne;
			if (NbCase == 49)
			{
			cout << endl << endl << "Match Nul !" << endl;
			break; //Fin de la partie
			}

		}

		return 0;
	} // ppal ()


} // namespace

int main ()
{
	J1=	Pseudo(1);
	J2=	Pseudo(2);

	ppal ();
	return 0;
} // main ()
