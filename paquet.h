#pragma once
#include <string>

#include "carte.h"

class Game;
class Joueur;

class Paquet { // Paquet de cartes initial
    std::vector<Carte*> paquet;
public:
    Paquet()=default;
    Paquet(std::vector<std::string> paragraph,Game& G);
    static void interprete_ligne(const std::string& ligne,std::string& comm,int& arg1,int& arg2,double& arg3);
    unsigned int size() const;
    const Carte* operator()(unsigned int i) const;
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
};

class Pioche { // Pioche utilisée en jeu
    const Paquet& paquet_source;
    std::vector<const Carte*> pioche;
    std::vector<const Carte*> defausse;
public:
    Pioche(const Paquet& Pq);
    void melanger(); // Mélange la pioche à partir de la défausse
    void reset(); // Réinitialise la pioche à partir du paquet source
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
    Banque(const Collection& Coll);
    void reset(); // Réinitialise la banque
};
