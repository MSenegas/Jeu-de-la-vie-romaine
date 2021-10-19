#include <stdexcept>

#include "game.h"
#include "case.h"

void Periode::interprete_ligne(const std::string& ligne,std::string& comm,int& arg) {
    // Interprète une chaîne de caractères de la forme <chaîne> [numéro] (seul le 1er numéro sera pris en compte)
    unsigned int ind_espace=ligne.find(' ');
    comm=ligne.substr(0,ind_espace);
    if (ind_espace!=(unsigned int)std::string::npos)
        arg=std::stoi(ligne.substr(ind_espace+1));
}

Periode::Periode(const std::vector<std::string> paragraph,Game& G) {
    std::string case_type;
    int case_cash=0;
    for (unsigned int i=0;i<paragraph.size();i++) {
        interprete_ligne(paragraph.at(i),case_type,case_cash);
        if (case_type=="stop") cases.push_back(CaseStop());
        else if (case_type=="chance") cases.push_back(CaseChance(G));
        else if (case_type=="tresor") cases.push_back(CaseTresor(G));
        else if (case_type=="salaire") cases.push_back(CaseSalaire(G,case_cash));
        else if (case_type=="pari") cases.push_back(CasePari(G));
        else if (case_type=="loto") cases.push_back(CaseLoto(G));
        else if (case_type=="justice") cases.push_back(CaseJustice(G));
        else if (case_type=="rejouez") cases.push_back(CaseRejouez(G));
        else if (case_type=="reculez_avancez") cases.push_back(CaseReculezAvancez(G));
        else if (case_type=="enfant") cases.push_back(CaseEnfant(G));
        else if (case_type=="prison") cases.push_back(CasePrison());
        else if (case_type=="esclave") cases.push_back(CaseEsclave());
        else if (case_type=="vacances") cases.push_back(CaseVacances());
        else if (case_type=="conseil") cases.push_back(CaseConseil());
        else if (case_type=="cash") cases.push_back(CaseCash(case_cash));
        else if (case_type=="cash_cagnotte") cases.push_back(CaseCashCagnotte(G,case_cash));
        else if (case_type=="cash_joueurs") cases.push_back(CaseCashJoueurs(G,case_cash));
        else if (case_type=="cash_cagnotte_joueurs") cases.push_back(CaseCashCagnotteJoueurs(G,case_cash));
        else {
            std::string err_msg=" n'est pas un nom de case reconnu";
            throw std::invalid_argument(case_type+err_msg);}
    }
}

bool Periode::is_empty() const {return cases.empty();}

unsigned int Periode::size() const {return cases.size();}

void Periode::tomber(const unsigned int &n_case, Joueur &J) {
    cases.at(n_case).tomber(J);}

void Periode::passer(const unsigned int &n_case, Joueur &J) {
    cases.at(n_case).passer(J);}

void Periode::reset_cases() {
    for (unsigned int i=0;i<cases.size();i++)
        cases.at(i).reset_case();}

Plateau::Plateau(const std::string path,Game& G) {
    std::vector<std::vector<std::string>> paragraph_list;
    Game::lire_fichier(paragraph_list,path);
    plateau=std::vector<Periode>(indice_plateau(N_CARRIERES,paragraph_list.size())+1,Periode());
    std::string nom_periode;
    int n_carr;
    int n_per=1;
    for (unsigned int i=0;i<paragraph_list.size();i++) {
        std::string entete=paragraph_list.at(i).front();
        Periode::interprete_ligne(entete,nom_periode,n_per);
        if (nom_periode=="esclave") n_carr=0;
        else if (nom_periode=="politicien") n_carr=1;
        else if (nom_periode=="commerçant") n_carr=2;
        else if (nom_periode=="chemin_sûr") n_carr=1;
        else if (nom_periode=="chemin_risqué") n_carr=2;
        else {
            std::string err_msg=" n'est pas un nom de période reconnu. Si vous avez rajouté des carrières, merci de mettre à jour sans oublier d'augmenter const int N_C";
            throw std::invalid_argument(nom_periode+err_msg);}
        paragraph_list.at(i).erase(paragraph_list.at(i).begin());
        plateau.at(indice_plateau(n_carr,n_per))=Periode(paragraph_list.at(i),G);
    }
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
