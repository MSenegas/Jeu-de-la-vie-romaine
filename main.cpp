#include <Imagine/Graphics.h>
#include "game.h"
#include "case.h"
#include "carte.h"

int main(){
    Game G("testplateau.txt","testcartes.txt",6);
    Imagine::openWindow(1500,1000);
    G.affichage();
    Imagine::endGraphics();
    return 0;
}
