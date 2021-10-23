#pragma once
#include <vector>

class Plateau;
class Carte;

class Joueur {
    // Argent
    int money;
    unsigned int dettes;
    // Position
    int carriere;
    int periode;
    int case_libre;
    bool a_fini;
    // Prison & Esclave
    bool prison;
    unsigned int essais_prison;
    bool esclave;
    int case_esclave;
    // Autres
    unsigned int conseils;
    unsigned int enfants;
    // Cartes
    std::vector<Carte*> cartes_chance_tresor;
    std::vector<Carte*> cartes_achetez; // Trier avant vente
    std::vector<Carte*> cartes_propriete;
    std::vector<Carte*> cartes_symboles;
public:
    Joueur(const int& n_carr);
    void cash_flow(const int& gain); // Augmente l'argent du joueur de gain (peut être négatif)
    void verif_dettes(); // Vérifie s'il faut emprunter ou s'il est possible de rembourser des dettes
    void add_conseil();
    void play(Plateau& P,unsigned int& cag); // Gère le tour du joueur
    void mouv(Plateau& P,const int& de); // Effectue le mouvement des joueurs et les effets des cases
    bool is_over() const;
    void carriere_nouv_periode(); // Renvoie la carrière correspondant à la période !!! À CHANGER SI L'ON RAJOUTE DES CARRIERES !!!
    int decision_chemin_risque_sur() const; // Effectue la décision du chemin risqué ou sûr
    void prendre_cagnotte(unsigned int& cag); // Donne la cagnotte au joueur (ne vérifie pas)
    void passer_salaire(); // Gère les coupons, le revenu des conseils et des propriétés
    void aller_prison();
    void tenter_sortir_prison(const int& de);
    void sortir_prison();
    void aller_esclavage();
    void sortir_esclavage();
    void add_enfant(const int& nb_enfants); // Vérifie si c'est possible
    void add_carte_chance_tresor(Carte* C); // Gestion des cartes sortez de prison
    void remove_carte_chance_tresor();
    void acheter_cartes_achetez(std::vector<Carte*> LC);
    void vendre_cartes_achetez();
    void add_carte_propriete(Carte* C);
    // évtl. faire un système de vente entre joueurs
    void add_carte_symbole(Carte* C);
    void remove_carte_symbole();
    void reset(const int& n_carr); // Réinitialise le joueur
    void dessine_icone(int i0,int j0) const;
    void affichage() const;// Affiche le joueur
    // évtl. faire variante générations
};
