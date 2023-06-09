/**
 * @file botnet.c
 * @author {Grégoire BELLON, Klemens Galus, Julian LEBOUC}
 * @brief Le fichier botnet.c est le fichier principal, il utilise tous les autres pour éxecuter une partie.
 * @version 0.1
 * @date 2021-03-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <time.h>

#include "../lib/virus.h"
#include "../lib/upgrade.h"
#include "../lib/upgrade_list.h"
#include "../lib/jeu.h"
#include "../lib/computer_list.h"
#include "../lib/country_list.h"
#include "../lib/config.h"
#include "../lib/data_sdl.h"

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

/**
 * @brief Permet d'afficher la liste de tous les pays, utilise la fonction afficher_country
 * @param cl {Pointeur sur une structure de liste des pays}
*/
void afficher_pays(country_list_t *cl)
{
    for (int i = 0; i < 18; i++)
    {
        afficher_country(cl->liste[i]);
    }
}

/**
 * @brief Main 
 * 
 * @return int 
 */
void play(Mix_Chunk *a, int time)
{
    printf("%d \n", Mix_PlayChannel(-1, a, 0));
    delay(time);
}

int mainMenu()
{
    SDL_Color white = {255, 255, 255};
    Mix_Chunk *Select = NULL;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_Window *pWindow = NULL;
    SDL_Renderer *pRenderer = NULL;
    SDL_Rect pstart, pcharge, pquit;
    SDL_Event events;
    if (SDL_CreateWindowAndRenderer(LONG, HAUT, SDL_WINDOW_SHOWN, &pWindow, &pRenderer) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        printf("AUDIO FAILLED TO LAUNCH \n");
        return -1;
    }
    Select = Mix_LoadWAV("../asset/select.wav");
    int isOpen = 1;
    int ret = 0;
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("../asset/Lato-Black.ttf", 240);
    int IsSaveExist = saveExist();
    printf("save = %d \n", IsSaveExist);
    if (font == NULL)
    {
        printf("Error can't load font \n");
    }
    while (isOpen)
    {
        while (SDL_PollEvent(&events))
        {
            switch (events.type)
            {
            case SDL_QUIT:
                isOpen = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (events.button.button == SDL_BUTTON_LEFT)
                    if (isOnButton(pquit))
                    {
                        isOpen = 0;
                        play(Select, 200);
                    }
                if (isOnButton(pstart))
                {
                    isOpen = 0;
                    ret = 1;
                    play(Select, 200);
                }
                if (isOnButton(pcharge) && IsSaveExist)
                {
                    isOpen = 0;
                    ret = 2;
                    play(Select, 200);
                }
                break;
            }
        }

        SDL_SetRenderDrawColor(pRenderer, 0, 255, 0, 255);
        SDL_RenderPresent(pRenderer);

        initRect(pRenderer, &pstart, 390, 195, 300, 100, 255, 0, 0, 255);
        if(IsSaveExist)
            initRect(pRenderer, &pcharge, 390, 380, 300, 100, 255, 0, 0, 255);
        else
            initRect(pRenderer, &pcharge, 390, 380, 300, 100, 158, 158, 158, 255);
        initRect(pRenderer, &pquit, 465, 555, 150, 50, 255, 0, 0, 255);

        showText(pRenderer, &pstart, "JOUER", font, &white);
        showText(pRenderer, &pquit, "QUITTER", font, &white);
        showText(pRenderer, &pcharge, "CHARGER", font, &white);

        SDL_RenderPresent(pRenderer);
    }
    TTF_CloseFont(font);
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    Mix_FreeChunk(Select);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    return ret;
}

int nameVirus(SDL_Renderer *Render, SDL_Window *Window, char **textaa, TTF_Font *font)
{
    size_t len = 0;
    size_t l = 0;
    size_t lcp = 0;
    SDL_Color white = {255, 255, 255};
    SDL_Color black = {0, 0, 0};

    SDL_RenderClear(Render);
    SDL_Event events;
    SDL_Rect pQ, inputB, inputT, valider, titre;
    char buffer[50];
    int isOpen = 1;
    SDL_StartTextInput();
    while (isOpen)
    {
        while (SDL_PollEvent(&events))
        {
            switch (events.type)
            {
            case SDL_QUIT:
                isOpen = 0;
                return 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (events.button.button == SDL_BUTTON_LEFT)
                    if (isOnButton(valider))
                    {
                        printf("text : %s \n", *textaa);
                        SDL_StopTextInput();
                        isOpen = 0;
                    }
                break;
            case SDL_TEXTINPUT:
                l = strlen(events.text.text);
                lcp = len + 1 < MAX_LEN ? 1 : MAX_LEN - len;
                strncpy(*textaa + len, events.text.text, lcp);
                len += lcp;
                printf("%s \n", *textaa);
                break;
            case SDL_KEYDOWN:
                if (events.key.keysym.sym == SDLK_BACKSPACE && len)
                {
                    textaa[0][len - 1] = 0;
                    len--;
                    printf("%s \n", *textaa);
                }
                else if ((events.key.keysym.sym == SDLK_RETURN && len) || (events.key.keysym.sym == SDLK_KP_ENTER && len))
                {
                    printf("text : %s \n", *textaa);
                    SDL_StopTextInput();
                    isOpen = 0;
                }
                break;
            }
        }
        initRect(Render, &pQ, 0, 0, LONG, HAUT, 0, 137, 255, 255);

        initRect(Render, &inputB, (LONG / 2) - 300, 100, 600, 40, 255, 255, 255, 255);
        initRect(Render, &inputT, (LONG / 2) - 300, 100, 30 * len, 40, 255, 255, 255, 255);
        initRect(Render, &titre, (LONG / 2) - 150, 30, 300, 60, 0, 137, 255, 255);
        initRect(Render, &valider, (LONG / 2) - 110, 160, 220, 70, 255, 255, 255, 255);
        showText(Render, &valider, "VALIDER", font, &black);
        showText(Render, &titre, "NOMMER VOTRE VIRUS", font, &black);
        snprintf(buffer, 50, "%s", *textaa);
        showText(Render, &inputT, buffer, font, &black);
        SDL_RenderPresent(Render);
    }
    return 1;
}

int getInfo(SDL_Renderer * pRenderer, SDL_Window * pWindow, country_list_t * cl,TTF_Font * font, Mix_Chunk *Select,texture_list_t* texturesButton){
    SDL_Color white = {255 ,255 ,255};
    SDL_Color red = {255 ,71 ,71};

    SDL_RenderClear(pRenderer);
    int isOpen = 1;
    SDL_Event events;
    SDL_Rect pBG, pReturn, Rnom[cl->nb], RNInfect[cl->nb], Rinfect[cl->nb];
    while(isOpen){
        while(SDL_PollEvent(&events)){
            switch (events.type){
                case SDL_QUIT:
                    isOpen = 0;
                    return 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(events.button.button == SDL_BUTTON_LEFT){
                        if (isOnButton(pReturn))
                        {
                            isOpen = 0;
                            play(Select, 300);
                        }
                    }
            }
        }
        initRect(pRenderer, &pBG, 0,0, LONG, HAUT, 0 ,137 ,255,255);
        int i;
        int colone = 0, ligne = 0;
        for (i = 0; i < cl->nb; i++){
            initRect(pRenderer, &Rnom[i], 70 + (320 * colone ),20 + (80 * ligne), 15*strlen(cl->liste[i]->name),20 , 150,150,150, 0);
            if(cl->liste[i]->healthy_pcs_cpt != 0)
                showText(pRenderer, &Rnom[i], cl->liste[i]->name, font, &white);
            else
                showText(pRenderer, &Rnom[i], cl->liste[i]->name, font, &red);


            initRect(pRenderer, &RNInfect[i], 70 + (320 * colone ),45 + (80 * ligne), 200,20 , 13, 13, 240, 255);

            initRect(pRenderer, &Rinfect[i], 70 + (320 * colone ),45 + (80 * ligne), cl->liste[i]->compromised_pcs_cpt * 200 / (cl->liste[i]->healthy_pcs_cpt + cl->liste[i]->compromised_pcs_cpt),20 , 240, 13, 13, 255);

            ligne++;
            if(ligne == 9){
                ligne = 0;
                colone++;
            }

        }

        initRect(pRenderer, &pReturn, 1080 - 70, 720 - 70, 45, 45, 255, 0, 0, 0); //Button de retour
        SDL_RenderCopy(pRenderer, texturesButton->textures[0], NULL, &pReturn);
        SDL_RenderPresent(pRenderer);

    }
}

int shop(SDL_Renderer *Render, SDL_Window *Window, jeu_t *jeu, upgrade_list_t *up_list, TTF_Font *font, Mix_Chunk *Select, Mix_Chunk *Error, texture_list_t * texturesButton)
{
    SDL_Color white = {255, 255, 255};
    float money;
    char buffer[50];
    SDL_RenderClear(Render);

    SDL_Event events;
    SDL_Rect pQ, pReturn, pItem0, pItem1, pItem2, pItem3, pItem4, pItem5, pItem6, pItem7, Items[8];
    int isOpen = 1;

    while (isOpen)
    {
        while (SDL_PollEvent(&events))
        {
            switch (events.type)
            {
            case SDL_QUIT:
                isOpen = 0;
                return 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (events.button.button == SDL_BUTTON_LEFT)
                    money = jeu->btc;
                if (isOnButton(pReturn))
                {
                    isOpen = 0;
                    money = 0;
                    play(Select, 300);
                }
                else if (isOnButton(Items[0]))
                {
                    printf("Bought Item 0 \n");
                    buy_upgrade(jeu, up_list->liste[0]);
                    isOpen = 0;
                    if (jeu->btc == money)
                    {
                        play(Error, 300);
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Somme insuffisante", "Vous n'avez pas assez de bitcoins pour acheter cet upgrade.", Window);
                    }
                    else
                    {
                        play(Select, 300);
                    }
                }
                else if (isOnButton(Items[1]))
                {
                    printf("Bought Item 1 \n");
                    buy_upgrade(jeu, up_list->liste[1]);
                    isOpen = 0;
                    if (jeu->btc == money)
                    {
                        play(Error, 300);
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Somme insuffisante", "Vous n'avez pas assez de bitcoins pour acheter cet upgrade.", Window);
                    }
                    else
                    {
                        play(Select, 300);
                    }
                }
                else if (isOnButton(Items[2]))
                {
                    printf("Bought Item 2 \n");
                    buy_upgrade(jeu, up_list->liste[2]);
                    isOpen = 0;
                    if (jeu->btc == money)
                    {
                        play(Error, 300);
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Somme insuffisante", "Vous n'avez pas assez de bitcoins pour acheter cet upgrade.", Window);
                    }
                    else
                    {
                        play(Select, 300);
                    }
                }
                else if (isOnButton(Items[3]))
                {
                    printf("Bought Item 3 \n");
                    buy_upgrade(jeu, up_list->liste[3]);
                    isOpen = 0;
                    if (jeu->btc == money)
                    {
                        play(Error, 300);
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Somme insuffisante", "Vous n'avez pas assez de bitcoins pour acheter cet upgrade.", Window);
                    }
                    else
                    {
                        play(Select, 300);
                    }
                }
                else if (isOnButton(Items[4]))
                {
                    printf("Bought Item 4 \n");
                    buy_upgrade(jeu, up_list->liste[4]);
                    isOpen = 0;
                    if (jeu->btc == money)
                    {
                        play(Error, 300);
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Somme insuffisante", "Vous n'avez pas assez de bitcoins pour acheter cet upgrade.", Window);
                    }
                    else
                    {
                        play(Select, 300);
                    }
                }
                else if (isOnButton(Items[5]))
                {
                    printf("Bought Item 5 \n");
                    buy_upgrade(jeu, up_list->liste[5]);
                    isOpen = 0;
                    if (jeu->btc == money)
                    {
                        play(Error, 300);
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Somme insuffisante", "Vous n'avez pas assez de bitcoins pour acheter cet upgrade.", Window);
                    }
                    else
                    {
                        play(Select, 300);
                    }
                }
                else if (isOnButton(Items[6]))
                {
                    printf("Bought Item 6 \n");
                    buy_upgrade(jeu, up_list->liste[6]);
                    isOpen = 0;
                    if (jeu->btc == money)
                    {
                        play(Error, 300);
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Somme insuffisante", "Vous n'avez pas assez de bitcoins pour acheter cet upgrade.", Window);
                    }
                    else
                    {
                        play(Select, 300);
                    }
                }
                else if (isOnButton(Items[7]))
                {
                    printf("Bought Item 7 \n");
                    buy_upgrade(jeu, up_list->liste[7]);
                    isOpen = 0;
                    if (jeu->btc == money)
                    {
                        play(Error, 300);
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Somme insuffisante", "Vous n'avez pas assez de bitcoins pour acheter cet upgrade.", Window);
                    }
                    else
                    {
                        play(Select, 300);
                    }
                }
                break;
            }
        }
        initRect(Render, &pQ, 0, 0, LONG, HAUT, 0, 137, 255, 255); //backgroud de couleur BLEU CIEL
        genererShop(Items, Render, up_list, font);
        

        initRect(Render, &pReturn, 1080 - 70, 720 - 70, 45, 45, 255, 0, 0, 0); //Button de retour
        SDL_RenderCopy(Render, texturesButton->textures[0], NULL, &pReturn);

        SDL_RenderPresent(Render);
    }

    return 1;
}

void spend_day(jeu_t *jeu, country_list_t *cl)
{
    spread_world(jeu->virus, cl);
    mine_btc_world(jeu, cl);
}

void startNewGame(int new)
{
    int not_infected_countries[18] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
    char *VirusName = calloc(MAX_LEN, sizeof(char *));
    /*

    ATTENTION IL VA FALLOIR DETRUIRE VIRUSNAME A LA FIN

    */
    SDL_Color white = {255, 255, 255};

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (IMG_Init(flags))
        printf("GOOD INIT\n");
    SDL_Window *pWindow = NULL;
    SDL_Renderer *pRenderer = NULL;

    if (SDL_CreateWindowAndRenderer(LONG, HAUT, SDL_WINDOW_SHOWN, &pWindow, &pRenderer) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
        SDL_Quit();
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        printf("AUDIO FAILLED TO LAUNCH \n");
    }
    Mix_Chunk *Select = NULL, *Error = NULL, *Gagner = NULL, *Perdu = NULL;
    Select = Mix_LoadWAV("../asset/select.wav");
    Error = Mix_LoadWAV("../asset/error.wav");
    Gagner = Mix_LoadWAV("../asset/victoire.wav");
    Perdu = Mix_LoadWAV("../asset/defaite.wav");

    TTF_Init();
    SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
    TTF_Font *font = TTF_OpenFont("../asset/Lato-Black.ttf", 60);

    nameVirus(pRenderer, pWindow, &VirusName, font);
    printf("%s \n", VirusName);
    char buffer[10];
    int result;
    char *buf = malloc(200);
    result = readlink("/proc/self/exe", buf, 200);

    buf[result] = '\0';
    char *const last = strrchr(buf, '/');
    if (last != NULL)
        *last = '\0';

    chdir(buf);
    free(buf);

    virus_t *virus = virus_create(VirusName, 1, 1);

    printf("K: %s \n", virus->name);

    char WinName[MAX_LEN + 15] = "BotNet : ";
    strcat(WinName, VirusName);
    SDL_SetWindowTitle(pWindow, WinName);
    //virus_display(virus);
    long double *proportion;

    //upgrade_display(upgrade);

    jeu_t *jeu = jeu_create(virus, 200.5);
    virus_display(jeu->virus);

    /* printf("Thunes : %f \n", jeu->btc);
    buy_upgrade(jeu, upgrade);

    virus_display(jeu->virus);
    printf("Thunes : %f \n", jeu->btc);*/

    country_list_t *cl = creer_country_list();
    
    cl->liste[DEFAULT_INFECTED_COUNTRY]->compromised_pcs_cpt = DEFAULT_BOTNET_SIZE;

    upgrade_list_t *up_liste;
    printf("IMPORT UGRADE \n");

    if (new == 0)
    {
        load_country_list(cl);
        load_jeu(jeu);
        up_liste = upgrade_liste_charger("../datas/upgradeSAVE");
    }
    else
    {
        up_liste = upgrade_liste_charger("../datas/upgrade.data");
    }
    afficher_upgrade_list(up_liste);
    /*
    Textures des points sur la map 1 par pays
    */
    
    texture_list_t *texturesMap = creer_texture_list(pRenderer, "../datas/texturesMap");
    texture_list_t *texturesButton = creer_texture_list(pRenderer, "../datas/texturesButton");
    printf("TEXTURE IMPORTED \n");
    
    /*

    LOGO

    */

    SDL_Rect pLoInfect, pLoRecherche, pSelectHit, pRateSlider, pBottom, pbg, pmoney, pboutique, prpour, prr, prpoub, prb, pRecMap, pRecBit, pinfo;
    SDL_Event events;
    int isOpen = 1;
    int ret = 0;

    SDL_Surface *test = IMG_Load("../asset/map.png");
    SDL_Texture *testt = SDL_CreateTextureFromSurface(pRenderer, test);

    int i = 20, j = 35;

    pRecMap.x = 100;
    pRecMap.y = 50;
    pRecMap.w = 1080 / 1.3;
    pRecMap.h = 580 / 1.3;

    pRecBit.x = 80;
    pRecBit.y = HAUT - 180 + 120;
    pRecBit.w = 20;
    pRecBit.h = 20;
    unsigned long int time_ref = (unsigned long int)time(NULL);
    int game_st = game_state(jeu, cl);
    while (isOpen && game_st == 0)
    {
        //printf(" Avancée recherche virus %f \n", jeu->virus_research);
        while (SDL_PollEvent(&events))
        {
            switch (events.type)
            {
            case SDL_QUIT:
                save_country_list(cl);
                save_jeu(jeu);
                upgrade_liste_sauv(up_liste, "../datas/upgradeSAVE");

                isOpen = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (events.button.button == SDL_BUTTON_LEFT)
                {
                    if (isOnButton(pboutique))
                    {
                        printf("SHOP \n");
                        play(Select, 200);
                        shop(pRenderer, pWindow, jeu, up_liste, font, Select, Error, texturesButton);
                    }
                    else if (isOnButton(pRateSlider))
                    {
                        int xS, yS;
                        SDL_GetMouseState(&xS, &yS);
                        edit_mining_rate(jeu, (xS - 350) / 200.0);
                    }
                    else if (isOnButton(pinfo))
                    {
                        play(Select, 200);
                        printf("INFO \n");
                        getInfo(pRenderer, pWindow, cl, font, Select, texturesButton);

                    }
                }

                break;
            }
        }
        if ((unsigned long int)time(NULL) - time_ref >= 1)
        {
            game_st = game_state(jeu, cl);
            spend_day(jeu, cl);
            for (int i = 0; i < 18; i++)
            {
                if (not_infected_countries[i] != -1)
                {
                    if (cl->liste[i]->compromised_pcs_cpt > 0)
                    {
                        not_infected_countries[i] = -1;
                        char str[80];
                        strcpy(str, cl->liste[i]->name);
                        strcat(str, " a été infecté");
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Nouvelle infection", str, pWindow);
                    }
                }
            }
            time_ref = (unsigned long int)time(NULL);
        }
        initRect(pRenderer, &pbg, 0, 0, LONG, HAUT, 0, 137, 255, 255);
        int i;
        SDL_RenderCopy(pRenderer, texturesButton->textures[4], NULL, &pRecMap);
        for (i = 0; i < texturesMap->nb; i++)
        {
            SDL_RenderCopy(pRenderer, texturesMap->textures[i], NULL, &pRecMap);
        }
        initRect(pRenderer, &pBottom, 0, HAUT - 180, LONG, 200, 91, 91, 91, 255);

        initRect(pRenderer, &prpour, 20, HAUT - 180 + 30, 200, 20, 255, 155, 155, 255);
        initRect(pRenderer, &prr, 20, HAUT - 180 + 30, 2 * ((int)(compromised_healthy_proportion(cl) * 100)), 20, 240, 13, 13, 255);
        initRect(pRenderer, &prpoub, 20, HAUT - 180 + 70, 200, 20, 155, 155, 255, 255);
        initRect(pRenderer, &prb, 20, HAUT - 180 + 70, 2 * ((int)(jeu->virus_research * 100)), 20, 13, 13, 240, 255);

        initRect(pRenderer, &pRateSlider, 350, HAUT - 180 + 35, 200, 10, 255, 255, 255, 255);
        SDL_SetRenderDrawColor(pRenderer, 20, 20, 20, 255);
        SDL_RenderDrawCircle(pRenderer, 350 + (200 * jeu->mining_rate), HAUT - 180 + 40, 20);
        SDL_RenderFillCircle(pRenderer, 350 + (200 * jeu->mining_rate), HAUT - 180 + 40, 20);

        snprintf(buffer, 10, "%.2f", jeu->btc);
        initRect(pRenderer, &pmoney, 20, HAUT - 180 + 120, 50, 20, 91, 91, 91, 255);
        showText(pRenderer, &pmoney, buffer, font, &white);
        SDL_RenderCopy(pRenderer, texturesButton->textures[5], NULL, &pRecBit);

        initRect(pRenderer, &pboutique, 1080 - 100, 720 - 100, 37, 37, 0, 0, 0, 0);
        SDL_RenderCopy(pRenderer, texturesButton->textures[3], NULL, &pboutique);

        initRect(pRenderer, &pinfo, 1080 - 200, 720 - 100, 37, 37, 0, 0, 0, 0);
        SDL_RenderCopy(pRenderer, texturesButton->textures[6], NULL, &pinfo);

        drawCountryPoint(cl, texturesMap);

        initRect(pRenderer, &pLoRecherche, 230, HAUT - 180 + 25, 30, 35, 255, 255, 255, 0);
        SDL_RenderCopy(pRenderer, texturesButton->textures[1], NULL, &pLoInfect);
        initRect(pRenderer, &pLoInfect, 230, HAUT - 180 + 65, 30, 33, 155, 155, 255, 0);
        SDL_RenderCopy(pRenderer, texturesButton->textures[2], NULL, &pLoRecherche);

        SDL_RenderPresent(pRenderer);
    }
    if (game_st == 1)
    {
        play(Gagner, 500);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Victoire", "Vous avez gagné la partie ! :)", pWindow);
        printf("Vous avez gagné,wow !");
        SDL_Quit();
    }
    else if (game_st == -1)
    {
        play(Perdu, 500);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Défaite", "Vous avez perdu la partie ! :(", pWindow);
        printf("Vous avez perdu,mince !");
        SDL_Quit();
    }
    destroyTexture(&texturesButton);
    destroyTexture(&texturesMap);
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    Mix_FreeChunk(Select);
    Mix_CloseAudio();
    SDL_Quit();
    virus_destroy(&jeu->virus);
    detruire_country_list(&cl);
}

int main()
{
    int result;
    char *buf = malloc(200);
    result = readlink("/proc/self/exe", buf, 200);

    buf[result] = '\0';
    char *const last = strrchr(buf, '/');
    if (last != NULL)
        *last = '\0';

    chdir(buf);
    free(buf);

    int a = mainMenu();
    printf("a %i\n", a);
    switch (a)
    {
    case 0:
        printf("QUIT ! \n");
        break;
    case 1:
        startNewGame(1);
        break;
    case 2:
        startNewGame(0);
        break;
    }
    return EXIT_SUCCESS;
}