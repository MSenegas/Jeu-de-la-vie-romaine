#pragma once
#include <vector>

class Joueur;
class Game;

class Case {
public:
    Case()=default;
    Case(const Case&)=delete;
    Case& operator=(const Case&)=delete;
    virtual void tomber(Joueur&); // Effet déclenché lorsque le joueur J tombe sur la case
    virtual void passer(Joueur&); // Paye le salaire du joueur J (seul. salaire)
    virtual void reset_case(); // Réinitialise la case
};

class CaseStop: public Case {};

class CaseChance: public Case {
    Game& current_game;
public:
    CaseChance(Game& G);
    void tomber(Joueur& J);
};

class CaseTresor: public Case {
    Game& current_game;
public:
    CaseTresor(Game& G);
    void tomber(Joueur& J);
};

class CaseSalaire: public Case {
    std::vector<Joueur*> collected; // Les joueurs qui ont collecté leur salaire (utilisé seulement pour le salaire)
    Game& current_game;
    int salaire; // Montant du salaire
public:
    CaseSalaire(Game& G,const int& sal);
    void tomber(Joueur& J);
    void passer(Joueur& J);
    void ajouter_joueur_deja_paye(Joueur& J); // Ajoute le joueur J à ceux qui ont déjà reçu leur salaire
    void reset_case();
};

class CasePari: public Case {
    Game& current_game;
public:
    CasePari(Game& G);
    void tomber(Joueur& J);
};

class CaseLoto: public Case {
    Game& current_game;
public:
    CaseLoto(Game& G);
    void tomber(Joueur& J);
};

class CaseJustice: public Case {
    Game& current_game;
public:
    CaseJustice(Game& G);
    void tomber(Joueur& J);
};

class CaseRejouez: public Case {
    Game& current_game;
public:
    CaseRejouez(Game& G);
    void tomber(Joueur& J);
};

class CaseReculezAvancez: public Case {
    Game& current_game;
public:
    CaseReculezAvancez(Game& G);
    void tomber(Joueur& J);
};

class CaseEnfant: public Case {
    Game& current_game;
public:
    CaseEnfant(Game& G);
    void tomber(Joueur& J);
};

class CasePrison: public Case {
public:
    void tomber(Joueur& J);
};

class CaseEsclave: public Case {
public:
    void tomber(Joueur& J);
};

class CaseVacances: public Case {};

class CaseMarche: public Case {
    Game& current_game;
public:
    CaseMarche(Game& G);
    void tomber(Joueur& J);
};

class CaseConseil: public Case {
public:
    void tomber(Joueur& J);
};

class CaseCash: public Case {
protected:
    int gift;
public:
    CaseCash(const int& gain);
    virtual void tomber(Joueur& J); // donne gain au joueur
};

class CaseCashCagnotte: public CaseCash {
protected:
    Game& current_game;
public:
    CaseCashCagnotte(Game& G,const int& gain);
    void tomber(Joueur& J); // oblige le joueur à donner -gain à la cagnotte
};

class CaseCashJoueurs: public CaseCash {
protected:
    Game& current_game;
public:
    CaseCashJoueurs(Game& G,const int& gain);
    virtual void tomber(Joueur& J); // ordonne à tous les joueurs LJ de donner gain au joueur J
};

class CaseCashCagnotteJoueurs: public CaseCashJoueurs {
public:
    CaseCashCagnotteJoueurs(Game& G,const int& gain);
    void tomber(Joueur& J); // ordonne à tous les joueurs LJ de donner -gain à la cagnotte
};
