#pragma once
#include <vector>

class Joueur;
class Pioche;

class Carte {
protected:
    Pioche& pioche_source;
public:
    Carte(Pioche& P): pioche_source(P) {}
    virtual ~Carte()=default;
    Carte(const Carte&)=delete;
    Carte& operator=(const Carte&)=delete;
    virtual void tirer(Joueur&) const=0;
    virtual void salaire() const {}
    void defausser() const;
};
