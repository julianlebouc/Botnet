/**
 * @file upgrade.c
 * @author {Grégoire BELLON, Klemens Galus, Julian LEBOUC}
 * @brief Fonction primitives pour la structure upgrade_t
 * @version 0.1
 * @date 2021-03-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../lib/upgrade.h"

/**
 * @brief Créer un objet upgrade_t
 * 
 * @param name 
 * @param price 
 * @param spr_rate 
 * @param rsch_rate 
 * @return upgrade_t* 
 */
extern upgrade_t *upgrade_create(char name[], float price, float spr_rate, float rsch_rate)
{ //cr�� un upgrade
    upgrade_t *upgrade = malloc(sizeof(upgrade_t));

    upgrade->name = malloc(strlen(name) * sizeof(char)); //il me semble que la taille d'un char peut changer selon le système....on est jamais trop prudents

    strcpy(upgrade->name, name);

    upgrade->price = price;

    upgrade->spreading_rate = spr_rate;

    upgrade->research_rate = rsch_rate;
    return (upgrade);
}

extern void upgrade_edit(upgrade_t *u, char name[], float price, float spr_rate, float rsch_rate)
{
    if (name != NULL)
        upgrade_edit_name(u, name);
    if (price != -1)
        u->price = price;
    if (spr_rate != -1)
        u->spreading_rate = spr_rate;
    if (rsch_rate != -1)
        u->research_rate = rsch_rate;
}

extern void upgrade_edit_name(upgrade_t *u, char *name)
{
    u->name = realloc(u->name, sizeof(name));
    strcpy(u->name, name);
}

/**
 * @brief Modifies le prix d'un objet upgrade_t
 * 
 * @param upgrade 
 * @param price 
 */
extern void upgrade_edit_price(upgrade_t *upgrade, float price)
{ //modifie le prix du upgrade pass� en parametre
    upgrade->price = price;
}

/**
 * @brief Modifies le taux de recherche d'un objet upgrade_t
 * 
 * @param upgrade 
 * @param rsch_rate 
 */
extern void upgrade_edit_rsch_rate(upgrade_t *upgrade, float rsch_rate)
{ //modifie le taux de recherche du upgrade pass� en parametre
    upgrade->research_rate = rsch_rate;
}

/**
 * @brief Modifies le taux de propagagtion d'un objet upgrade_t
 * 
 * @param upgrade 
 * @param spr_rate 
 */
extern void upgrade_edit_spr_rate(upgrade_t *upgrade, float spr_rate)
{ //modifie le taux de propagation du upgrade pass� en parametre
    upgrade->spreading_rate = spr_rate;
}

/**
 * @brief Détruit un objet upgrade_t
 * 
 * @param upgrade 
 */
extern void upgrade_destroy(upgrade_t **upgrade)
{ //detruie le upgrade
    if (*upgrade != NULL)
    {
        free((*upgrade)->name);
        free(*upgrade);
        *upgrade = NULL;
    }
}

/**
 * @brief Affiche un objet upgrade_t
 * 
 * @param upgrade 
 */
extern void upgrade_display(upgrade_t *upgrade)
{ //affiche les donn�es du upgrade
    if (upgrade != NULL)
    {
        printf("Nom du upgrade : %s \n", upgrade->name);
        printf("Prix : %f \n", upgrade->price);
        printf("Taux de propagation : %f \n", upgrade->spreading_rate);
        printf("Taux de recherche : %f \n", upgrade->research_rate);
    }
    else
    {
        printf("Le upgrade n'existe pas");
    }
}

/*void main(){
    upgrade_t *op = upgrade_create("kebab",32.5,0.2,0.5);
    upgrade_display(op);
    upgrade_edit_rsch_rate(op, 0.5);
    upgrade_display(op);
}*/
