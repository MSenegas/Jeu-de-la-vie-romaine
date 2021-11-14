#pragma once
#include <string>

#include "carte.h"

class Game;
class Joueur;

class Pioche { // Pioche utilisée en jeu
    std::vector<const Carte*> pioche;
    std::vector<const Carte*> defausse;
public:
    Pioche()=default;
    ~Pioche();
    Pioche(const Pioche&)=delete;
    Pioche& operator=(const Pioche&)=delete;
    void creer(const std::vector<std::string>& paragraph,Game& G);
    static void interprete_ligne(const std::string& ligne,std::string& comm,int& arg1,int& arg2,double& arg3,std::vector<int>& sym);
    void melanger(); // Mélange la pioche à partir de la défausse
    const Carte* pop(); // Retire une carte de la pioche (mélange si nécessaire)
    void tirer(Joueur& J); // Tire une carte et joue son effet sur le joueur J
    void defausser(const Carte* C); // Met une carte dans la défausse
};

class Banque { // ~ J'ai pas de mot pour "ensemble de pioches"
public:
    Pioche pioche_chance;
    Pioche pioche_tresor;
    Pioche pioche_bonus;
    Pioche pioche_achetez;
    Pioche pioche_enfant;
    Pioche pioche_propriete;
    Pioche pioche_symboles;
    Banque(const std::string& path,Game& G);
    void reset(); // Réinitialise la banque
};
