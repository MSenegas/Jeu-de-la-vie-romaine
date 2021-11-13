#pragma once
#include <vector>

class Game;
class Joueur;
class Pioche;
class CarteSymboles;

class Carte {
protected:
    Pioche& pioche_source;
public:
    Carte(Pioche& P): pioche_source(P) {}
    virtual ~Carte()=default;
    Carte(const Carte&)=delete;
    Carte& operator=(const Carte&)=delete;
    virtual void tirer(Joueur&) const=0;
    virtual int base_value() const;
    virtual int operator()() const;
    virtual const std::vector<int>& symboles() const;
    virtual double variation() const;
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
    int operator()() const {return prix_vente;}
    double variation() const {return risk;}
};

class CartePropriete: public Carte {
    std::vector<int> val_symb;
    int prix_base;
    int revenu;
public:
    CartePropriete(Pioche& P,int b,int r,const std::vector<int>& v): Carte(P),val_symb(v),prix_base(b),revenu(r) {}
    void tirer(Joueur& J) const;
    int base_value() const {return prix_base;}
    int operator()() const {return revenu;}
    const std::vector<int>& symboles() const {return val_symb;}
};

class CarteMettezProprieteEncheres: public Carte {
    Game& current_game;
public:
    CarteMettezProprieteEncheres(Pioche& P,Game& G): Carte(P),current_game(G) {}
    void tirer(Joueur&) const; // Faire particulièrement attention à la conservation des cartes
};

class CarteSortezPrison: public Carte {
public:
    CarteSortezPrison(Pioche& P): Carte(P) {}
    void tirer(Joueur& J) const;
};

class CarteTirezEnfant: public Carte {
    Game& current_game;
public:
    CarteTirezEnfant(Pioche& P,Game& G): Carte(P),current_game(G) {}
    void tirer(Joueur& J) const;
};

class CarteEnfant: public Carte {
    int enfants;
public:
    CarteEnfant(Pioche& P,int nb): Carte(P),enfants(nb) {}
    void tirer(Joueur& J) const;
    int operator()() const {return enfants;}
};

class CarteRejouez: public Carte {
    Game& current_game;
public:
    CarteRejouez(Pioche& P,Game& G): Carte(P),current_game(G) {}
    void tirer(Joueur& J) const;
};

class CarteReculezAvancez: public Carte {
protected:
    Game& current_game;
    int amount;
public:
    CarteReculezAvancez(Pioche& P,Game& G,int cases): Carte(P),current_game(G),amount(cases) {}
    virtual void tirer(Joueur& J) const;
};

class CarteReculezAvancezJoueurs: public CarteReculezAvancez {
public:
    CarteReculezAvancezJoueurs(Pioche& P,Game& G,int cases): CarteReculezAvancez(P,G,cases) {}
    void tirer(Joueur&) const;
};

class CarteAutreChanceTresor: public Carte {
    Game& current_game;
public:
    CarteAutreChanceTresor(Pioche& P,Game& G): Carte(P),current_game(G) {}
    void tirer(Joueur& J) const;
};

class CarteAutreCashBonus: public Carte {
    Game& current_game;
public:
    CarteAutreCashBonus(Pioche& P,Game& G): Carte(P),current_game(G) {}
    void tirer(Joueur& J) const;
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
    virtual void tirer(Joueur& J) const;
};

class CarteCashCagnotteJoueurs: public CarteCashJoueurs {
public:
    CarteCashCagnotteJoueurs(Pioche& P,Game& G,int gain); // Throw si gain>0
    void tirer(Joueur&) const;
};

class CarteSymboles: public Carte {
    std::vector<int> symb;
public:
    CarteSymboles(Pioche& P,const std::vector<int>& C): Carte(P),symb(C) {}
    void tirer(Joueur&) const;
    const std::vector<int>& symboles() const {return symb;}
};
