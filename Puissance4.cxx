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
	void PositionneJeton (CVMatrice & Mat, const unsigned NumCol, unsigned & NumLi,
					bool & CoupDuJoueur1,unsigned & NbCoups1,
					unsigned & NbCoups2, unsigned & NbCase)
		{
			NumLi = Mat.size () - 1;
			while (NumLi > 0 && (Mat [NumLi][NumCol] != ' '))
				--NumLi;
			if (NumLi ==0 && (Mat [NumLi][NumCol] != ' '))
				{
					CoupDuJoueur1 = !CoupDuJoueur1;
					return;
				}
			Mat [NumLi][NumCol] = (CoupDuJoueur1 ? 'X' : 'O'); // Place le pion correspondant au joueur
			++NbCase;
			if (CoupDuJoueur1)
				{
					++NbCoups1;
				}
			else
				{
					++NbCoups2;
				}
		} //PositionneJeton ()

				/*NumLi = 6;		//On par de la case d'en bas
				while (true)
				{
					if ((Mat [NumLi][NumCol] == ' ') )	//test si la case est vide
					{
						if (CoupDuJoueur1)
						{
							Mat [NumLi][NumCol] = 'X';
							++NbCoups1;
							++NbCase;
							break;
						}
						else
						{
							Mat [NumLi][NumCol] = 'O';
							++NbCoups2;
							++NbCase;
							break;
						}
					}
					else if (NumLi ==0)
					{
						CoupDuJoueur1 = !CoupDuJoueur1;
						break;
					}
					else
					{
						--NumLi;
					}
				}*/

	bool TestVictoireColonne (const CVMatrice& Mat, const unsigned NumLi, const unsigned NumCol, const bool CoupDuJoueur1)
	{
		const char KCarAInspecter = (CoupDuJoueur1 ? 'X' : 'O');
		if (NumLi > 3) return false;

		unsigned Li (NumLi + 1);

		while ((Li < Mat.size ()) && Mat [Li][NumCol] == KCarAInspecter)
			++Li;

		return (4 == Li - NumLi);
	} //TestVictoireColonne ()

	bool TestVictoireLigne (const CVMatrice & Mat, const unsigned NumLi, const unsigned NumCol, const bool CoupDuJoueur1)
	{
		char CaracRecherche;
		CaracRecherche = (CoupDuJoueur1 ? 'X' : 'O');
		unsigned NbInspections;
		if (NumCol < Mat.size() /2)
		{
		    NbInspections = NumCol + 1;
		}
		else
		{
		    NbInspections = Mat.size () - NumCol;
		}
		int PosDepart = max (0, int(NumCol - Mat.size () /2));
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

		unsigned NumLi (6);
		unsigned Li;
		unsigned Col;
		unsigned NBCorespondances;
		for (unsigned NumCol = 0; NumCol < 4; ++NumCol)
		{
			while (NumLi > 0)
			{
				Li = NumLi;
				Col = NumCol;
				NBCorespondances = 0;
				while (Mat [Li][Col] == Carac)
				{
					--Li;
					++Col;


					++ NBCorespondances;
					if (NBCorespondances == 4) return true;
					if (Li == 0) break;

				} // END WHILE 2


				--NumLi;
				if (Li == 2) break;
			} // END WHILE 1
		} // END FOR
		return false;

	} // TestVictoireDiagonaleCroissante ()

	int ppal ()
	{
		unsigned NbCoups1 = 0;
		unsigned NbCoups2 = 0;
		CVLigne L;          //
		L.reserve (7);      //
		CVMatrice M (7, L); // Déclaration d'une matrice M7 vide
		InitMat (M);        // Remplissage avec des ' '
		AffichePuissance4 (M); // Affichage de la matrice accompagnée des noms des colonnes
	    unsigned Point1 = 0;
		unsigned Point2 = 0;
		AfficheScore (Point1, Point2); // Ici le score vaut toujours O à O
		unsigned NumLigne;
		char Choix;
		bool Joueur = true;
		unsigned NbCase = 0;
		while(true)
		{
			string Saisie;	//Variable qui stockera ce qui est saisie au clavier
			string NomJoueur;
			string Nom;
			while (true)
			{
				if((!Joueur + 1) ==1)
				{
					Couleur(KJaune);
					NomJoueur = J1;
					Nom = J1;
				}
				else
				{
					Couleur(KRouge);
					NomJoueur = J2;
					Nom = J2;
				}
				//cout << "\n\nJoueur " << !Joueur + 1 << "  :  Dans quelle colonne voulez vous placer le pion? ";
				cout << "\n\n"<< Nom << "  :  Dans quelle colonne voulez vous placer le pion? ";
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
				Couleur(KReset);
			} // END TRUE
			PositionneJeton (M, Choix - 'A', NumLigne, Joueur, NbCoups1, NbCoups2, NbCase); // Placement du pion dans la matrice
			if ((TestVictoireColonne (M, NumLigne, Choix - 'A', Joueur)) ||
				(TestVictoireLigne (M, NumLigne, Choix - 'A', Joueur)) ||
				(TestVictoireDiagonaleCroissante(M, Joueur)))
				//Tests de victoire
			{
				if (Joueur)
				{
				    ++Point1;
				    NbCase = 0;
				}
				else
				{
				    ++Point2;
				    NbCase = 0;
				}
				InitMat (M); //Réinitialisation la grille
			}

			AffichePuissance4 (M); //Actualisation de la grille après coups
			AfficheScore (Point1, Point2); // Affichage du score
			cout << "\nPlus que " << 49-NbCase;
			if(Point1 == 3)
			{
				cout << "\n\n" << Nom << "a gagné la partie en " << NbCoups1
					 << " coups!\n";
				break; // Fin de la partie
			}
			if(Point2 == 3)
			{
			    cout << "\n\n" << Nom << "a gagné la partie en " << NbCoups2
					 << " coups!\n";
			    break; //Fin de la partie
			}
			Joueur = !Joueur;
			//cout << M.size();
			//cout << NumLigne;
			if (NbCase == 49)
			{
			cout << endl << endl << "Match Nul !" << endl;
			continue; //Fin de la partie
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
