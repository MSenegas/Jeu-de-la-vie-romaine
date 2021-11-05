#include <stdexcept>

#include "game.h"
#include "case.h"

void Periode::interprete_ligne(const std::string& ligne,std::string& comm,int& arg) {
    // Interprète une chaîne de caractères de la forme <chaîne> [numéro] (seul le 1er numéro sera pris en compte)
    unsigned int ind_apres_espace=ligne.find(' ');
    comm=ligne.substr(0,ind_apres_espace);
    while (ind_apres_espace<ligne.size() && ligne.at(ind_apres_espace)==' ') ind_apres_espace++;
    if (ind_apres_espace<ligne.size())
        arg=std::stoi(ligne.substr(ind_apres_espace));
}

Periode::Periode(const std::vector<std::string> paragraph,Game& G) {
    std::string case_type;
    for (unsigned int i=0;i<paragraph.size();i++) {
        int case_cash=0;
        interprete_ligne(paragraph.at(i),case_type,case_cash);
        if (case_type=="stop") cases.push_back(new CaseStop());
        else if (case_type=="chance") cases.push_back(new CaseChance(G));
        else if (case_type=="tresor") cases.push_back(new CaseTresor(G));
        else if (case_type=="salaire") cases.push_back(new CaseSalaire(G,case_cash));
        else if (case_type=="pari") cases.push_back(new CasePari(G));
        else if (case_type=="loto") cases.push_back(new CaseLoto(G));
        else if (case_type=="justice") cases.push_back(new CaseJustice(G));
        else if (case_type=="rejouez") cases.push_back(new CaseRejouez(G));
        else if (case_type=="reculez_avancez") cases.push_back(new CaseReculezAvancez(G));
        else if (case_type=="enfant") cases.push_back(new CaseEnfant(G));
        else if (case_type=="prison") cases.push_back(new CasePrison());
        else if (case_type=="esclave") cases.push_back(new CaseEsclave());
        else if (case_type=="marche") cases.push_back(new CaseMarche(G));
        else if (case_type=="vacances") cases.push_back(new CaseVacances());
        else if (case_type=="conseil") cases.push_back(new CaseConseil());
        else if (case_type=="cash") cases.push_back(new CaseCash(case_cash));
        else if (case_type=="cash_cagnotte") cases.push_back(new CaseCashCagnotte(G,case_cash));
        else if (case_type=="cash_joueurs") cases.push_back(new CaseCashJoueurs(G,case_cash));
        else if (case_type=="cash_cagnotte_joueurs") cases.push_back(new CaseCashCagnotteJoueurs(G,case_cash));
        else {
            std::string err_msg=" n'est pas un nom de case reconnu";
            throw std::invalid_argument(case_type+err_msg);}}
}

bool Periode::is_empty() const {return cases.empty();}

unsigned int Periode::size() const {return cases.size();}

void Periode::tomber(const unsigned int &n_case, Joueur &J) {
    cases.at(n_case)->tomber(J);}

void Periode::passer(const unsigned int &n_case, Joueur &J) {
    cases.at(n_case)->passer(J);}

void Periode::reset_cases() {
    for (unsigned int i=0;i<cases.size();i++)
        cases.at(i)->reset_case();}

void Periode::affichage(int i0, int j0) const {
    for (unsigned int i=0;i<cases.size();i++)
        cases.at(i)->affichage(i0,j0+TAILLE_CASES_AFFICHAGE*i);}

Plateau::Plateau(const std::string path,Game& G) {
    std::vector<std::vector<std::string>> paragraph_list;
    Game::lire_fichier(paragraph_list,path);
    plateau=std::vector<Periode>(indice_plateau(N_CARRIERES,paragraph_list.size())+1,Periode());
    std::string nom_periode;
    for (unsigned int i=0;i<paragraph_list.size();i++) {
        int n_carr;
        int n_per=1;
        std::string entete=paragraph_list.at(i).front();
        Periode::interprete_ligne(entete,nom_periode,n_per);
        if (nom_periode=="esclave") n_carr=0;
        else if (nom_periode=="politicien") n_carr=1;
        else if (nom_periode=="commercant") n_carr=2;
        else if (nom_periode=="chemin_sur") n_carr=1;
        else if (nom_periode=="chemin_risque") n_carr=2;
        else {
            std::string err_msg=" n'est pas un nom de période reconnu. Si vous avez rajouté des carrières, merci de mettre à jour sans oublier les paramètres";
            throw std::invalid_argument(nom_periode+err_msg);}
        paragraph_list.at(i).erase(paragraph_list.at(i).begin());
        plateau.at(indice_plateau(n_carr,n_per))=Periode(paragraph_list.at(i),G);}
    while (!plateau.empty() && plateau.back().is_empty())
        plateau.pop_back();
}

bool Plateau::depasse_periode(const int& n_carr, const int& n_per, const unsigned int& n_case) const {
    if (n_case>=plateau.at(indice_plateau(n_carr,n_per)).size())
        return true;
    else return false;
}

bool Plateau::derniere_periode(const int &n_per) const {
    return (indice_plateau(1,n_per+1)>=plateau.size());}

unsigned int Plateau::size_periode(const int &n_carr, const int &n_per) const {
    return plateau.at(indice_plateau(n_carr,n_per)).size();}

void Plateau::tomber(const int &n_carr, const int &n_per, const unsigned int &n_case, Joueur &J) {
    plateau.at(indice_plateau(n_carr,n_per)).tomber(n_case,J);}

void Plateau::passer(const int &n_carr, const int &n_per, const unsigned int &n_case, Joueur &J) {
    plateau.at(indice_plateau(n_carr,n_per)).passer(n_case,J);}

void Plateau::reset_cases() {
    for (unsigned int i=0;i<plateau.size();i++)
        plateau.at(i).reset_cases();}

void affichejoueurs(const std::vector<const Joueur*>& LJ,int i0,int j0) {
    switch (LJ.size()) {
    case 1:
        LJ.front()->dessine_icone(i0,j0);
        return;
    case 2:
        LJ.front()->dessine_icone(i0+0.22*TAILLE_CASES_AFFICHAGE,j0,0.56*TAILLE_CASES_AFFICHAGE);
        LJ.back()->dessine_icone(i0+0.22*TAILLE_CASES_AFFICHAGE,j0+0.5*TAILLE_CASES_AFFICHAGE,0.56*TAILLE_CASES_AFFICHAGE);
        return;
    case 3:
        LJ.front()->dessine_icone(i0,j0+0.22*TAILLE_CASES_AFFICHAGE,0.56*TAILLE_CASES_AFFICHAGE);
        LJ.at(1)->dessine_icone(i0+0.38*TAILLE_CASES_AFFICHAGE,j0,0.56*TAILLE_CASES_AFFICHAGE);
        LJ.back()->dessine_icone(i0+0.38*TAILLE_CASES_AFFICHAGE,j0+0.5*TAILLE_CASES_AFFICHAGE,0.56*TAILLE_CASES_AFFICHAGE);
        return;
    case 4:
        LJ.front()->dessine_icone(i0,j0,0.56*TAILLE_CASES_AFFICHAGE);
        LJ.at(1)->dessine_icone(i0,j0+0.5*TAILLE_CASES_AFFICHAGE,0.56*TAILLE_CASES_AFFICHAGE);
        LJ.at(2)->dessine_icone(i0+0.44*TAILLE_CASES_AFFICHAGE,j0,0.56*TAILLE_CASES_AFFICHAGE);
        LJ.back()->dessine_icone(i0+0.44*TAILLE_CASES_AFFICHAGE,j0+0.5*TAILLE_CASES_AFFICHAGE,0.56*TAILLE_CASES_AFFICHAGE);
        return;
    case 5:
        LJ.front()->dessine_icone(i0,j0+0.28*TAILLE_CASES_AFFICHAGE,0.44*TAILLE_CASES_AFFICHAGE);
        LJ.at(1)->dessine_icone(i0+0.2*TAILLE_CASES_AFFICHAGE,j0,0.44*TAILLE_CASES_AFFICHAGE);
        LJ.at(2)->dessine_icone(i0+0.53*TAILLE_CASES_AFFICHAGE,j0+0.11*TAILLE_CASES_AFFICHAGE,0.44*TAILLE_CASES_AFFICHAGE);
        LJ.at(3)->dessine_icone(i0+0.53*TAILLE_CASES_AFFICHAGE,j0+0.45*TAILLE_CASES_AFFICHAGE,0.44*TAILLE_CASES_AFFICHAGE);
        LJ.back()->dessine_icone(i0+0.2*TAILLE_CASES_AFFICHAGE,j0+0.56*TAILLE_CASES_AFFICHAGE,0.44*TAILLE_CASES_AFFICHAGE);
        return;
    case 6:
        LJ.front()->dessine_icone(i0,j0+0.15*TAILLE_CASES_AFFICHAGE,0.39*TAILLE_CASES_AFFICHAGE);
        LJ.at(1)->dessine_icone(i0,j0+0.46*TAILLE_CASES_AFFICHAGE,0.39*TAILLE_CASES_AFFICHAGE);
        LJ.at(2)->dessine_icone(i0+0.27*TAILLE_CASES_AFFICHAGE,j0,0.39*TAILLE_CASES_AFFICHAGE);
        LJ.at(3)->dessine_icone(i0+0.53*TAILLE_CASES_AFFICHAGE,j0+0.15*TAILLE_CASES_AFFICHAGE,0.39*TAILLE_CASES_AFFICHAGE);
        LJ.at(4)->dessine_icone(i0+0.53*TAILLE_CASES_AFFICHAGE,j0+0.46*TAILLE_CASES_AFFICHAGE,0.39*TAILLE_CASES_AFFICHAGE);
        LJ.back()->dessine_icone(i0+0.27*TAILLE_CASES_AFFICHAGE,j0+0.61*TAILLE_CASES_AFFICHAGE,0.39*TAILLE_CASES_AFFICHAGE);
        return;
    default:
        throw std::invalid_argument("No players");}
}

void Plateau::affichage(std::vector<const Joueur*> LJ) const {
    // Plateau
    plateau.at(0).affichage(0,0);
    for (int n_carr=1;n_carr<=N_CARRIERES;n_carr++) {
        int n_per=1;
        while (!derniere_periode(n_per-1)) {
            plateau.at(indice_plateau(n_carr,n_per)).affichage(TAILLE_CASES_AFFICHAGE*indice_plateau(n_carr,n_per),0);
            n_per++;}}
    // Joueurs
    std::vector<const Joueur*> joueurs_meme_case;
    while (!LJ.empty()) {
        const Joueur* curr_player=LJ.back();
        LJ.pop_back();
        std::vector<const Joueur*>::iterator ind_j=LJ.end()-1;
        while (ind_j!=LJ.begin()-1) {
            if ((curr_player->esclave && (**ind_j).esclave &&
                 (**ind_j).case_esclave==curr_player->case_esclave) ||
                (!curr_player->esclave && !(**ind_j).esclave &&
                 (**ind_j).case_libre==curr_player->case_libre &&
                 (**ind_j).periode==curr_player->periode &&
                 (**ind_j).carriere==curr_player->carriere)) {
                joueurs_meme_case.push_back(*ind_j);
                std::vector<const Joueur*>::iterator ind_rmv=ind_j;
                ind_j--;
                LJ.erase(ind_rmv);}
            else ind_j--;}
        joueurs_meme_case.push_back(curr_player);
        if (curr_player->esclave)
            affichejoueurs(joueurs_meme_case,0,TAILLE_CASES_AFFICHAGE*curr_player->case_esclave);
        else
            affichejoueurs(joueurs_meme_case,TAILLE_CASES_AFFICHAGE*indice_plateau(curr_player->carriere,curr_player->periode),TAILLE_CASES_AFFICHAGE*curr_player->case_libre);
        joueurs_meme_case.clear();}}
