#pragma once
#include <string>
#include "parametres.h"

#include "case.h"

class Game;
class Joueur;

class Periode {
    std::vector<const Case*> cases;
public:
    Periode()=default;
    Periode(const Periode&)=delete;
    Periode& operator=(const Periode&)=delete;
    void creer(const std::vector<std::string>& paragraph,Game& G);
    static void interprete_ligne(const std::string& ligne,std::string& comm,int& arg);
    bool is_empty() const;
    unsigned int size() const;
    void tomber(const unsigned int n_case,Joueur& J) const;
    void passer(const unsigned int n_case,Joueur& J) const;
    void affichage(int i0,int j0,std::string nom) const; // Affiche la période
};

class Plateau {
    std::vector<Periode> plateau;
    // Convention: indice de période = N_CARRIERES*(n_per-1)+n_carr
    //      n_per le numéro de période (1 pour esclave)
    //      n_carr l'identifiant de carrière:
    //               esclave: n_carr = 0
    //            politicien: n_carr = 1
    //            commerçant: n_carr = 2
    //            chemin sûr: n_carr = 1
    //         chemin risqué: n_carr = 2
public:
    Plateau(const std::string& path,Game& G);
    bool depasse_periode(const int n_carr,const int n_per,const unsigned int n_case) const;
    // Indique si cette position dépasse de la période (la partie se charge de changer de période ensuite)
    bool derniere_periode(const int n_per) const; // Indique si dette période est la dernière (REMARQUE: Une hypothèse sous-jacente est que la carrière 0 est définie pour toutes les périodes. La période en argument doit également être définie)
    unsigned int size_periode(const int n_carr,const int n_per) const;
    void tomber(const int n_carr,const int n_per,const unsigned int n_case,Joueur& J) const;
    void passer(const int n_carr,const int n_per,const unsigned int n_case,Joueur& J) const;
    void affichage(int i0,int j0,std::vector<const Joueur*>& LJ) const; // Affiche le plateau et les joueurs
    inline static unsigned int indice_plateau(const int& n_carr,const int& n_per) {return N_CARRIERES*(n_per-1)+n_carr;}
};
