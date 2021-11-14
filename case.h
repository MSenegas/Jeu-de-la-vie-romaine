#pragma once
#include <vector>

class Game;
class Joueur;

class Case {
public:
    Case()=default;
    virtual ~Case()=default;
    Case(const Case&)=delete;
    Case& operator=(const Case&)=delete;
    virtual void tomber(Joueur&) const=0; // Effet déclenché lorsque le joueur J tombe sur la case
    virtual void passer(Joueur&) const {} // Paye le salaire du joueur J (seul. salaire)
    virtual void affichage(int i0,int j0) const; // Affiche la case
};

class CaseStop: public Case {
public:
    CaseStop()=default;
    void tomber(Joueur&) const {}
    void affichage(int i0,int j0) const;
};

class CaseChance: public Case {
    Game& current_game;
public:
    CaseChance(Game& G): current_game(G) {}
    void tomber(Joueur& J) const;
    void affichage(int i0,int j0) const;
};

class CaseTresor: public Case {
    Game& current_game;
public:
    CaseTresor(Game& G): current_game(G) {}
    void tomber(Joueur& J) const;
    void affichage(int i0,int j0) const;
};

class CaseSalaire: public Case {
    Game& current_game;
    int salaire; // Montant du salaire
public:
    CaseSalaire(Game& G,const int& sal): current_game(G),salaire(sal) {}
    void tomber(Joueur& J) const;
    void passer(Joueur& J) const;
    void affichage(int i0,int j0) const;
};

class CasePari: public Case {
    Game& current_game;
public:
    CasePari(Game& G): current_game(G) {}
    void tomber(Joueur& J) const;
    void affichage(int i0,int j0) const;
};

class CaseLoto: public Case {
    Game& current_game;
public:
    CaseLoto(Game& G): current_game(G) {}
    void tomber(Joueur& J) const;
    void affichage(int i0,int j0) const;
};

class CaseJustice: public Case {
    Game& current_game;
public:
    CaseJustice(Game& G): current_game(G) {}
    void tomber(Joueur& J) const;
    void affichage(int i0,int j0) const;
};

class CaseRejouez: public Case {
    Game& current_game;
public:
    CaseRejouez(Game& G): current_game(G) {}
    void tomber(Joueur& J) const;
    void affichage(int i0,int j0) const;
};

class CaseReculezAvancez: public Case {
    Game& current_game;
public:
    CaseReculezAvancez(Game& G): current_game(G) {}
    void tomber(Joueur& J) const;
    void affichage(int i0,int j0) const;
};

class CaseEnfant: public Case {
    Game& current_game;
public:
    CaseEnfant(Game& G): current_game(G) {}
    void tomber(Joueur& J) const;
    void affichage(int i0,int j0) const;
};

class CasePrison: public Case {
public:
    CasePrison()=default;
    void tomber(Joueur& J) const;
    void affichage(int i0,int j0) const;
};

class CaseEsclave: public Case {
public:
    CaseEsclave()=default;
    void tomber(Joueur& J) const;
    void affichage(int i0,int j0) const;
};

class CaseVacances: public Case {
public:
    CaseVacances()=default;
    void tomber(Joueur&) const {}
    void affichage(int i0,int j0) const;
};

class CaseMarche: public Case {
    Game& current_game;
public:
    CaseMarche(Game& G): current_game(G) {}
    void tomber(Joueur& J) const;
    void affichage(int i0,int j0) const;
};

class CaseConseil: public Case {
public:
    CaseConseil()=default;
    void tomber(Joueur& J) const;
    void affichage(int i0,int j0) const;
};

class CaseCash: public Case {
protected:
    int gift;
public:
    CaseCash(const int& gain): gift(gain) {}
    virtual void tomber(Joueur& J) const; // donne gain au joueur
    virtual void affichage(int i0,int j0) const;
};

class CaseCashCagnotte: public CaseCash {
protected:
    Game& current_game;
public:
    CaseCashCagnotte(Game& G,const int& gain);
    void tomber(Joueur& J) const; // oblige le joueur à donner -gain à la cagnotte
    void affichage(int i0,int j0) const;
};

class CaseCashJoueurs: public CaseCash {
protected:
    Game& current_game;
public:
    CaseCashJoueurs(Game& G,const int& gain): CaseCash(gain), current_game(G) {}
    virtual void tomber(Joueur& J) const; // ordonne à tous les joueurs LJ de donner gain au joueur J
    virtual void affichage(int i0,int j0) const;
};

class CaseCashCagnotteJoueurs: public CaseCashJoueurs {
public:
    CaseCashCagnotteJoueurs(Game& G,const int& gain);
    void tomber(Joueur& J) const; // ordonne à tous les joueurs LJ de donner -gain à la cagnotte
    void affichage(int i0,int j0) const;
};
