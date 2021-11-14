#pragma once
#include <vector>

#include "plateau.h"

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
    std::vector<const Case*> salaires_recus;
    // Prison & Esclave
    bool prison;
    unsigned int essais_prison;
    bool esclave;
    unsigned int fois_esclave;
    int case_esclave;
    // Autres
    unsigned int conseils;
    std::vector<const Carte*> cartes_sortez_prison;
    std::vector<const Carte*> cartes_achetez;
    std::vector<const Carte*> cartes_enfant;
    std::vector<const Carte*> cartes_propriete;
public:
    Joueur(const int n_carr);
    ~Joueur();
    void cash_flow(const int gain); // Augmente l'argent du joueur de gain (peut être négatif)
    void verif_dettes(); // Vérifie s'il faut emprunter ou s'il est possible de rembourser des dettes
    void add_conseil();
    void play(Plateau& P,unsigned int& cag); // Gère le tour du joueur
    void mouv(Plateau& P,const int de); // Effectue le mouvement des joueurs et les effets des cases
    bool is_over() const;
    void carriere_nouv_periode(); // Renvoie la carrière correspondant à la période !!! À CHANGER SI L'ON RAJOUTE DES CARRIERES !!!
    int decision_chemin_risque_sur() const; // Effectue la décision du chemin risqué ou sûr
    void prendre_cagnotte(unsigned int& cag); // Donne la cagnotte au joueur (ne vérifie pas)
    void passer_salaire(); // Gère les coupons, le revenu des conseils et des propriétés
    void ajouter_salaire_deja_recu(const Case* C); // Ajoute à la liste des salaires déjà reçus
    void aller_prison();
    void tenter_sortir_prison(const int de);
    void sortir_prison();
    void aller_esclavage();
    void sortir_esclavage();
    void add_carte_enfant(const Carte* C); // Vérifie si c'est possible
    void add_carte_sortez_prison(const Carte* C);
    bool remove_carte_sortez_prison(); // Regarde s'il y a une carte sortez de prison et la retire le cas échéant
    static bool NulliteAchat(const Carte* A,const Carte* B); // Fonction de comparaison, renvoie true si B est moins bonne que A; à utiliser lors de l'achat
    static bool QualiteVente(const Carte* A,const Carte* B); // Fonction de comparaison, renvoie true si B est meilleure que A; à utiliser lors de la vente
    bool decision_acheter_vendre() const; // Effectue la décision d'acheter ou de vendre
    void decision_acheter_cartes(std::vector<const Carte*>& LC) const; // Décide des cartes à acheter ou non
    void acheter_cartes_achetez(const std::vector<const Carte*>& LC);
    void vendre_cartes_achetez();
    int decision_enchere_carte_propriete(Game& G,const Carte* C,int reduction) const; // Décide combien proposer pour cette carte propriété
    void add_carte_propriete(const Carte* C);
    bool decision_autre_chance_tresor() const; // Décide si tirer une carte chance ou trésor lorsque la carte le demandant est tirée
    bool decision_autre_cash_bonus() const; // Décide si tirer une carte bonus ou de l'argent
    int decision_jeu_hasard_avantage() const; // Décide combien miser au jeu de harard avantageux
    void reset(const int& n_carr); // Réinitialise le joueur
    void dessine_icone(int i0,int j0,int taille_cadre=TAILLE_CASES_AFFICHAGE) const; // Affiche le joueur
    friend void CaseSalaire::passer(Joueur&) const;
    friend void Plateau::affichage(std::vector<const Joueur*>) const;
};
