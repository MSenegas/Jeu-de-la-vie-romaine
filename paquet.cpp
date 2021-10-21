#include <stdexcept>

#include "paquet.h"

void Paquet::interprete_ligne(const std::string& ligne,std::string& comm,int& arg1,int& arg2,double& arg3) {
    // Interprète une chaîne de caractères de la forme <chaîne> [int1] [int2] [double3]
    unsigned int ancien_ind_espace;
    unsigned int ind_espace=ligne.find(' ');
    comm=ligne.substr(0,ind_espace);
    std::vector<int*> args12;args12.push_back(&arg1);args12.push_back(&arg2);
    for (unsigned int yeah=0;yeah<3;yeah++) {
        if (ind_espace!=(unsigned int)std::string::npos) {
            if (yeah==2) {
                std::string lstarg=ligne.substr(ind_espace+1);
                if (lstarg.back()=='%') {
                    lstarg.pop_back();
                    arg3=std::stod(lstarg)/100;}
                else
                    arg3=std::stod(lstarg);}
            else {
                ancien_ind_espace=ind_espace;
                ind_espace=ligne.find(' ',ind_espace+1);
                *args12.at(yeah)=std::stoi(ligne.substr(ancien_ind_espace+1,ind_espace-ancien_ind_espace-1));}}}
}

Pioche::Pioche(const Paquet& Pq): paquet_source(Pq) {melanger();}

void Pioche::melanger() {
    for (unsigned int i=0;i<paquet_source.paquet.size();i++)
        pioche.push_back(paquet_source.paquet.at(i));}

Banque::Banque(const Collection& Coll): collection_source(Coll),
    pioche_chance(Coll.paquet_chance),pioche_tresor(Coll.paquet_tresor),
    pioche_bonus(Coll.paquet_bonus),pioche_achetez(Coll.paquet_achetez),
    pioche_enfant(Coll.paquet_enfant),pioche_propriete(Coll.paquet_propriete),
    pioche_symboles(Coll.paquet_symboles) {}
