#pragma once
#include "plateau.h"
#include "paquet.h"
#include "joueur.h"

class Game {
    unsigned int t; // Temps écoulé
    Plateau plateau;
    Collection collection;
    Banque banque;
    std::vector<Joueur> liste_joueurs;
public:
    unsigned int cagnotte;

    Game(const std::string path_plateau,const std::string path_cartes,const unsigned int nb_joueurs);
    static std::vector<int> tirage_carrieres(const unsigned int nb_joueurs); // Effectue le tirage au sort pour les carrières des joueurs !!! À CHANGER SI L'ON RAJOUTE DES CARRIERES !!!
    static void lire_fichier(std::vector<std::vector<std::string>>& paragraph_list,const std::string path);
    void game_loop(); // Gestion du tour
    void mouv_joueur(Joueur& J,const int& de); // Appelle la fonction mouv du joueur (utilisée par les cases, cartes)
    void reset(); // Réinitialise la partie
    friend void CaseCashJoueurs::tomber(Joueur&);
    friend void CaseCashCagnotteJoueurs::tomber(Joueur&);
    friend void CaseChance::tomber(Joueur&);
    friend void CaseTresor::tomber(Joueur&);
    friend void CaseSalaire::tomber(Joueur&);
    friend void CaseEnfant::tomber(Joueur&);
    friend void CaseMarche::tomber(Joueur&);
};
