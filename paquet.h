#pragma once
#include <string>

#include "carte.h"

class Game;
class Joueur;

class Paquet { // Paquet de cartes initial
public:
    std::vector<Carte*> paquet;
    Paquet(std::vector<std::string> paragraph,Game& G);
    Paquet(const Paquet&)=delete;
    Paquet& operator=(const Paquet&)=delete;
    static void interprete_ligne(const std::string& ligne,std::string& comm,int& arg1,int& arg2,double& arg3);
};

class Collection { // ~ J'ai pas de mot pour "ensemble de paquets"
public:
    Paquet paquet_chance;
    Paquet paquet_tresor;
    Paquet paquet_bonus;
    Paquet paquet_achetez;
    Paquet paquet_enfant;
    Paquet paquet_propriete;
    Paquet paquet_symboles;
    Collection(const std::string path,Game& G);
    Collection(const Collection&)=delete;
    Collection& operator=(const Collection&)=delete;
};

class Pioche { // Pioche utilisée en jeu
    const Paquet& paquet_source;
    std::vector<const Carte*> pioche;
public:
    Pioche(const Paquet& Pq);
    void melanger(); // Réinitialise la pioche et mélange le paquet
    void pop_back(); // Retire une carte de la pioche si s'est possible, sinon, mélange
    void tirer(Joueur& J); // Tire une carte et joue son effet sur le joueur J
};

class Banque { // ~ J'ai pas de mot pour "ensemble de pioches"
    const Collection& collection_source;
    Pioche pioche_chance;
    Pioche pioche_tresor;
    Pioche pioche_bonus;
    Pioche pioche_achetez;
    Pioche pioche_enfant;
    Pioche pioche_propriete;
    Pioche pioche_symboles;
public:
    Banque(const Collection& Coll);
    void melange_pioches(); // Réinitialise la banque
};
