#pragma once
#include <vector>

class Game;
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
    void defausser() const;
};

class CarteAchetez: public Carte {
    int prix_achat;
    int prix_vente;
    double risk;
public:
    CarteAchetez(Pioche& P,int pa,int pv,double v): Carte(P),prix_achat(pa),prix_vente(pv),risk(v) {}
    void tirer(Joueur&) const;
    int base_value() const {return prix_achat;}
    int sale_value() const {return prix_vente;}
    double variation() const {return risk;}
};

class CartePropriete: public Carte {
    int prix_base;
    int revenu;
public:
    CartePropriete(Pioche& P,int b,int r): Carte(P),prix_base(b),revenu(r) {}
    void tirer(Joueur&) const;
    int base_value() const {return prix_base;}
    int operator()() const {return revenu;}
};

class CarteMettezProprieteEncheres: public Carte {
    Game& current_game;
public:
    CarteMettezProprieteEncheres(Pioche& P,Game& G): Carte(P),current_game(G) {}
    void tirer(Joueur& J) const;
};

class CarteSortezPrison: public Carte {
public:
    CarteSortezPrison(Pioche& P): Carte(P) {}
    void tirer(Joueur& J) const;
};

class CarteTirezEnfant: public Carte {
public:
    CarteTirezEnfant(Pioche& P): Carte(P) {}
    void tirer(Joueur& J) const;
};

class CarteEnfant: public Carte {
    int enfants;
public:
    CarteEnfant(Pioche& P,int nb): Carte(P),enfants(nb) {}
    void tirer(Joueur& J) const;
    int nb() const {return enfants;}
};

class CarteRejouez: public Carte {
public:
    CarteRejouez(Pioche& P): Carte(P) {}
    void tirer(Joueur& J) const;
};

class CarteReculezAvancez: public Carte {
protected:
    int amount;
public:
    CarteReculezAvancez(Pioche& P,int cases): Carte(P),amount(cases) {}
    virtual void tirer(Joueur& J) const;
};

class CarteReculezAvancezJoueurs: public CarteReculezAvancez {
    Game& current_game;
public:
    CarteReculezAvancezJoueurs(Pioche& P,Game& G,int cases): CarteReculezAvancez(P,cases),current_game(G) {}
    void tirer(Joueur&) const;
};

class CarteAutreChanceTresor: public Carte {
public:
    CarteAutreChanceTresor(Pioche& P): Carte(P) {}
    void tirer(Joueur& J) const; // Faire particulièrement attention à la conservation des cartes
};

class CarteAutreCashBonus: public Carte {
public:
    CarteAutreCashBonus(Pioche& P): Carte(P) {}
    void tirer(Joueur& J) const; // Faire particulièrement attention à la conservation des cartes
};

class CarteJeuHasardAvantage: public Carte {
public:
    CarteJeuHasardAvantage(Pioche& P): Carte(P) {}
    void tirer(Joueur& J) const;
};

class CartePari: public Carte {
    Game& current_game;
public:
    CartePari(Pioche& P,Game& G): Carte(P),current_game(G) {}
    void tirer(Joueur& J) const;
};

class CartePrison: public Carte {
public:
    CartePrison(Pioche& P): Carte(P) {}
    void tirer(Joueur& J) const;
};

class CarteEsclave: public Carte {
public:
    CarteEsclave(Pioche& P): Carte(P) {}
    void tirer(Joueur& J) const;
};

class CarteCash: public Carte {
protected:
    int gift;
public:
    CarteCash(Pioche& P,int gain): Carte(P),gift(gain) {}
    virtual void tirer(Joueur& J) const;
};

class CarteCashJoueurs: public CarteCash {
protected:
    Game& current_game;
public:
    CarteCashJoueurs(Pioche& P,Game& G,int gain): CarteCash(P,gain),current_game(G) {}
    virtual void tirer(Joueur&) const;
};

class CarteCashCagnotteJoueurs: public CarteCashJoueurs {
public:
    CarteCashCagnotteJoueurs(Pioche& P,Game& G,int gain); // Throw si gain>0
    void tirer(Joueur&) const;
};

class CarteSymboles: public Carte {
    // Réfléchir aux symboles possibles
};
