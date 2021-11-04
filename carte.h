#pragma once
#include <vector>

class Carte {
public:
    Carte()=default;
    virtual ~Carte()=default;
    Carte(const Carte&)=delete;
    Carte& operator=(const Carte&)=delete;
};
