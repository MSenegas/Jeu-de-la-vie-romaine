#include <stdexcept>
#include <algorithm>

#include "game.h"
#include "paquet.h"

void Paquet::interprete_ligne(const std::string& ligne,std::string& comm,int& arg1,int& arg2,double& arg3) {
    // Interprète une chaîne de caractères de la forme <chaîne> [int1] [int2] [double3]
    unsigned int ancien_ind_apres_espace;
    unsigned int ind_apres_espace=ligne.find(' ');
    comm=ligne.substr(0,ind_apres_espace);
    while (ind_apres_espace<ligne.size() && ligne.at(ind_apres_espace)==' ') ind_apres_espace++;
    std::vector<int*> args12;args12.push_back(&arg1);args12.push_back(&arg2);
    for (unsigned int yeah=0;yeah<3;yeah++) {
        if (ind_apres_espace<ligne.size()) {
            if (yeah==2) {
                std::string lstarg=ligne.substr(ind_apres_espace);
                while (lstarg.back()==' ') lstarg.pop_back();
                if (lstarg.back()=='%') {
                    lstarg.pop_back();
                    arg3=std::stod(lstarg)/100;}
                else
                    arg3=std::stod(lstarg);}
            else {
                ancien_ind_apres_espace=ind_apres_espace;
                ind_apres_espace=ligne.find(' ',ind_apres_espace);
                while (ind_apres_espace<ligne.size() && ligne.at(ind_apres_espace)==' ') ind_apres_espace++;
                *args12.at(yeah)=std::stoi(ligne.substr(ancien_ind_apres_espace,ind_apres_espace-ancien_ind_apres_espace-1));}}}
}

Paquet::Paquet(std::vector<std::string> paragraph,Game& G) {
    std::string carte_type;
    for (unsigned int i=0;i<paragraph.size();i++) {
        int carte_cost=0;int carte_rv=0;double carte_prc=0;
        interprete_ligne(paragraph.at(i),carte_type,carte_cost,carte_rv,carte_prc);
        if (carte_type=="") paquet.push_back(new Carte());
        else {
            std::string err_msg=" n'est pas un nom de carte reconnu";
            throw std::invalid_argument(carte_type+err_msg);}}
}

unsigned int Paquet::size() const {return paquet.size();}

const Carte* Paquet::operator()(unsigned int i) const {return paquet.at(i);}

Collection::Collection(const std::string path,Game& G) {
    std::vector<std::vector<std::string>> paragraph_list;
    Game::lire_fichier(paragraph_list,path);
    for (unsigned int i=0;i<paragraph_list.size();i++) {
        std::string nom_paquet=paragraph_list.at(i).front();
        paragraph_list.at(i).erase(paragraph_list.at(i).begin());
        if (nom_paquet=="chance")
            paquet_chance=Paquet(paragraph_list.at(i),G);
        else if (nom_paquet=="tresor")
            paquet_tresor=Paquet(paragraph_list.at(i),G);
        else if (nom_paquet=="bonus")
            paquet_bonus=Paquet(paragraph_list.at(i),G);
        else if (nom_paquet=="achetez")
            paquet_achetez=Paquet(paragraph_list.at(i),G);
        else if (nom_paquet=="enfant")
            paquet_enfant=Paquet(paragraph_list.at(i),G);
        else if (nom_paquet=="propriete")
            paquet_propriete=Paquet(paragraph_list.at(i),G);
        else if (nom_paquet=="symboles")
            paquet_symboles=Paquet(paragraph_list.at(i),G);
        else {
            std::string err_msg=" n'est pas un nom de paquet reconnu.";
            throw std::invalid_argument(nom_paquet+err_msg);}}
}

Pioche::Pioche(const Paquet& Pq): paquet_source(Pq) {reset();}

void Pioche::reset() {
    pioche.clear();defausse.clear();
    for (unsigned int i=0;i<paquet_source.size();i++)
        pioche.push_back(paquet_source(i));
    std::shuffle(pioche.begin(),pioche.end(),Game::gene_alea);
}

void Pioche::melanger() {
    for (unsigned int i=0;i<defausse.size();i++)
        pioche.push_back(defausse.at(i));
    defausse.clear();
    std::shuffle(pioche.begin(),pioche.end(),Game::gene_alea);
}

const Carte* Pioche::pop() {
    if (pioche.empty())
        melanger();
    const Carte* rep=pioche.back();
    pioche.pop_back();
    return rep;
}

void Pioche::tirer(Joueur& J) {pop()->tirer(J);}

Banque::Banque(const Collection& Coll): pioche_chance(Coll.paquet_chance),
    pioche_tresor(Coll.paquet_tresor),pioche_bonus(Coll.paquet_bonus),
    pioche_achetez(Coll.paquet_achetez),pioche_enfant(Coll.paquet_enfant),
    pioche_propriete(Coll.paquet_propriete),pioche_symboles(Coll.paquet_symboles) {}

void Banque::reset() {
    pioche_chance.reset();
    pioche_tresor.reset();
    pioche_bonus.reset();
    pioche_achetez.reset();
    pioche_enfant.reset();
    pioche_propriete.reset();
    pioche_symboles.reset();
}
