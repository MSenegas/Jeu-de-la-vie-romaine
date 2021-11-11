#pragma once
#include <vector>

#include "plateau.h"

class CarteSortezPrison;
class CarteEnfant;
class CarteAchetez;
class CartePropriete;
class CarteSymboles;

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
    // Enfants
    std::vector<const CarteEnfant*> cartes_enfant;
    // Autres
    unsigned int conseils;
    std::vector<const CarteSortezPrison*> cartes_sortez_prison;
    std::vector<const CarteAchetez*> cartes_achetez;
    std::vector<const CartePropriete*> cartes_propriete;
    std::vector<const CarteSymboles*> cartes_symboles;
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
    void add_carte_enfant(const CarteEnfant* C); // Vérifie si c'est possible
    void add_carte_sortez_prison(const CarteSortezPrison* C);
    bool remove_carte_sortez_prison(); // Regarde s'il y a une carte sortez de prison et la retire le cas échéant
    bool decision_acheter_vendre() const; // Effectue la décision d'acheter ou de vendre
    void decision_acheter_cartes(std::vector<const CarteAchetez*>& LC) const; // Décide des cartes à acheter ou non
    void acheter_cartes_achetez(const std::vector<const CarteAchetez*>& LC);
    void vendre_cartes_achetez();
    void add_carte_propriete(const CartePropriete* C);
    // évtl. faire un système de vente entre joueurs
    void add_carte_symbole(const CarteSymboles* C);
    void remove_carte_symbole();
    void reset(const int& n_carr); // Réinitialise le joueur
    void dessine_icone(int i0,int j0,int taille_cadre=TAILLE_CASES_AFFICHAGE) const; // Affiche le joueur
    friend void Plateau::affichage(std::vector<const Joueur*>) const;
    // évtl. faire variante générations
};
