#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "virus.h"
#include "upgrade.h"
#include "jeu.h"
#include "computer_list.h"
#include "country_list.h"

int main()
{

    virus_t *virus = virus_create("kaboub", 0.5, 0.5);
    virus_display(virus);

    upgrade_t *upgrade = upgrade_create("phishing", 30.0, 0.2, 0.2);
    upgrade_t *cles_usb = upgrade_create("clés usb", 15, 0.1, 0.3);
    upgrade_t *trojan = upgrade_create("trojan", 100.0, 0.5, 0.3);
    upgrade_t *fake_ad = upgrade_create("fake ad", 150, 1.0, 1.0);
    upgrade_display(upgrade);

    jeu_t *jeu = jeu_create(virus, 200.5);
    virus_display(jeu->virus);

    printf("Thunes : %f \n", jeu->btc);
    buy_upgrade(jeu, upgrade);

    virus_display(jeu->virus);
    printf("Thunes : %f \n", jeu->btc);

    /*computer_list_t * list_pc = liste_creer(10);
    computer_t * c = computer_create(10, 5, 0);
    computer_t * c1 = computer_create(15, 10, 1);
    liste_elem_ecrire(list_pc, c);
    liste_elem_ecrire(list_pc, c1);

    mine_btc(jeu, list_pc);
    printf("Thunes : %f \n", jeu->btc);*/

    country_list_t * list_country = creer_country_list();
    afficher_country_list(list_country);

    return 0;
}