#pragma once
#include <vector>

class Joueur;

class Carte {
public:
    Carte();
    virtual ~Carte()=default;
    Carte(const Carte&)=delete;
    Carte& operator=(const Carte&)=delete;
    virtual void tirer(Joueur&) const=0;
};
