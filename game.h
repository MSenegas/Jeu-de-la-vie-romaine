#pragma once
#include <functional>

#include "plateau.h"
#include "paquet.h"
#include "joueur.h"

class Game {
    unsigned int t; // Temps écoulé
    Plateau plateau;
    Banque banque;
    std::vector<Joueur> liste_joueurs;
    unsigned int cagnotte;
public:
    static std::mt19937_64 gene_alea; // Générateur aléatoire
    static std::_Bind<std::uniform_int_distribution<>(std::mt19937_64)> lancer_de; // Générateurs de nombres aléatoires selon une loi
    static std::_Bind<std::uniform_int_distribution<>(std::mt19937_64)> lancer_piece;
    static std::_Bind<std::uniform_int_distribution<>(std::mt19937_64)> lancer_de_mise_pari_loto;
    static std::_Bind<std::uniform_int_distribution<>(std::mt19937_64)> lancer_de_gain_loto;
    Game(const std::string& path_plateau,const std::string& path_cartes,unsigned int nb_joueurs);
    static std::vector<int> tirage_carrieres(const unsigned int nb_joueurs); // Effectue le tirage au sort pour les carrières des joueurs !!! À CHANGER SI L'ON RAJOUTE DES CARRIERES !!!
    static void lire_fichier(std::vector<std::vector<std::string>>& paragraph_list,const std::string path);
    void play_full_game(); // Lance une partie en entier
    bool is_over() const; // Regarde si la partie est terminée
    void game_loop(); // Gestion du tour
    void mouv_joueur(Joueur& J,const int& de); // Appelle la fonction mouv du joueur (utilisée par les cases, cartes)
    void reset(); // Réinitialise la partie
    friend void CaseCashJoueurs::tomber(Joueur&) const;
    friend void CaseCashCagnotte::tomber(Joueur&) const;
    friend void CaseCashCagnotteJoueurs::tomber(Joueur&) const;
    friend void CaseChance::tomber(Joueur&) const;
    friend void CaseTresor::tomber(Joueur&) const;
    friend void CaseSalaire::tomber(Joueur&) const;
    friend void CaseEnfant::tomber(Joueur&) const;
    friend void CaseMarche::tomber(Joueur&) const;
    friend void CarteMettezProprieteEncheres::tirer(Joueur&) const;
    friend void CarteTirezEnfant::tirer(Joueur&) const;
    friend void CarteReculezAvancezJoueurs::tirer(Joueur&) const;
    friend void CarteAutreChanceTresor::tirer(Joueur&) const;
    friend void CarteAutreCashBonus::tirer(Joueur&) const;
    friend void CarteCashJoueurs::tirer(Joueur&) const;
    friend void CarteCashCagnotteJoueurs::tirer(Joueur&) const;
};
