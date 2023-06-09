/**
 * @file jeu.c
 * @author {Grégoire BELLON, Klemens Galus, Julian LEBOUC}
 * @brief Permet de créer un jeu, de miner des bitcoins, acheter des upgrades
 * @version 0.1
 * @date 2021-03-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../lib/virus.h"
#include "../lib/upgrade.h"
#include "../lib/jeu.h"
#include "../lib/computer_list.h"
#include "../lib/country_list.h"
#include "../lib/random_lib.h"
#include "../lib/config.h"

/**
 * @brief Créer un jeu_t avec le virus_t passé en paramètre et la somme de bitcoins de départ
 * 
 * @param vir 
 * @param btc 
 * @return jeu_t* 
 */
extern jeu_t *jeu_create(virus_t *vir, float btc)
{ //cr�� un jeu
    jeu_t *jeu = malloc(sizeof(jeu_t));

    jeu->virus = vir;

    jeu->btc = btc;

    jeu->mining_rate=DEFAULT_MINING_RATE;

    jeu->virus_research=0;

    return (jeu);
}

extern void save_jeu(jeu_t *jeu){
    FILE *fichier;
	fichier = fopen("../datas/jeuSAVE", "w");
	fprintf(fichier,"%f %f", jeu->btc, jeu->virus_research);
	fclose(fichier);
	printf("Sauvegarde du jeu");
    save_virus(jeu->virus);
}

extern void load_jeu(jeu_t *jeu){
    FILE *fichier;
	fichier = fopen("../datas/jeuSAVE", "r");
	fscanf(fichier,"%f %f", &jeu->btc, &jeu->virus_research);
	fclose(fichier);
	printf("Chargement du jeu");
    load_virus(jeu->virus);
}

/**
 * @brief Permet l'achat d'un upgrade passé en paramètre, déduit son prix à notre argent
 * 
 * @param jeu 
 * @param upgrade 
 */
extern void buy_upgrade(jeu_t *jeu, upgrade_t *upgrade)
{ //achat d'un upgrade

    if(jeu->virus->research_rate+upgrade->research_rate<0){
        printf("Impossible d'acheter cet objet");
    }
    else if (jeu->btc >= upgrade->price)
    {
        jeu->btc = jeu->btc - upgrade->price; //enleve le prix de l'upgrade � notre argent

        jeu->virus->spreading_rate += upgrade->spreading_rate;

        jeu->virus->research_rate += upgrade->research_rate;
        
        upgrade_edit_price(upgrade, upgrade->price*2);
    }
    else
    {
        printf("Pas assez d'argent \n");
    }
}

/**
 * @brief Primitive de modification du taux de minage
 * 
 * @param jeu 
 * @param rate 
 */
extern void edit_mining_rate(jeu_t * jeu, float rate){ // modifie le taux de minage
    jeu->mining_rate=rate;
}

/**
 * @brief Ajoute à notre argent la somme minée par les ordinateurs du pays passé en paramètre
 * 
 * @param jeu 
 * @param country 
 */
extern void mine_btc_country(jeu_t *jeu, country_t * country) // mine des bitcoins 
{   
    jeu->btc+= BITCOIN_MINING_COEFFICIENT * country->compromised_pcs_cpt* jeu->mining_rate;
            
}

/**
 * @brief Execute la fonction mine_btc_country pour chaque pays le la liste de pays passée en paramètre
 * 
 * @param jeu 
 * @param list 
 */
extern void mine_btc_world(jeu_t *jeu, country_list_t * list){
    for(int i=0; i<list->nb; i++){
        mine_btc_country(jeu, list->liste[i]);
    } 
}

int search_virus(jeu_t * jeu, country_list_t * list){
    if(test_bernoulli(compromised_healthy_proportion(list)* jeu->mining_rate * jeu->virus->research_rate)==1){
        jeu->virus_research+=0.01;
    }
    return(jeu->virus_research>=1);
}

/**
 * @brief La fonction game_state calcule la proportion de pc infectés / pc sains et retourne 0 si la partie continue, 1 si la partie est gagnée, -1 si elle est perdue
 * 
 * @param list 
 * @param jeu 
 * @return int 
 */
extern int game_state(jeu_t * jeu, country_list_t * list){ // vérifies si on a infecté 51% des ordinateurs du monde
    float proportion = compromised_healthy_proportion(list);
    if(proportion>=WIN_PROPORTION){
        return(1);
    } else if(search_virus(jeu, list)){
        return(-1);
    } else{
        return(0);
    }
}


