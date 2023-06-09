#include "../lib/data_sdl.h"
#include "../lib/country_list.h"
#include "../lib/config.h"
#include <unistd.h>
#include <stdio.h> 

void showText(SDL_Renderer *render, SDL_Rect *rect, const char *text, TTF_Font *font, SDL_Color *color)
{
    SDL_Surface *surface;
    SDL_Texture *texture;

    surface = TTF_RenderText_Solid(font, text, *color);
    texture = SDL_CreateTextureFromSurface(render, surface);

    SDL_Rect message_rect;
    message_rect.x = rect->x;
    message_rect.y = rect->y;
    message_rect.w = rect->w;
    message_rect.h = rect->h;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(render, texture, NULL, &message_rect);
    SDL_DestroyTexture(texture);
}
void showSmallerText(SDL_Renderer *render, SDL_Rect *rect, const char *text, TTF_Font *font, SDL_Color *color, int y)
{
    SDL_Surface *surface;
    SDL_Texture *texture;

    surface = TTF_RenderText_Solid(font, text, *color);
    texture = SDL_CreateTextureFromSurface(render, surface);

    SDL_Rect message_rect;
    message_rect.x = rect->x;
    message_rect.y = y;
    message_rect.w = rect->w;
    message_rect.h = rect->h / 4;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(render, texture, NULL, &message_rect);
    SDL_DestroyTexture(texture);
}

/*
void initText(SDL_Surface * surface, SDL_Texture * texture, TTF_Font * font, SDL_Renderer * render, SDL_Color color ,char * text){
    surface = TTF_RenderText_Blended(font , text, color);
    texture = SDL_CreateTextureFromSurface(render, surface);
}*/

void initRect(SDL_Renderer *render, SDL_Rect *rect, int x, int y, int w, int h, int r, int g, int b, int alpha)
{
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;

    SDL_SetRenderDrawColor(render, r, g, b, alpha);
    SDL_RenderFillRect(render, rect);
}

int isOnButton(SDL_Rect rect)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    //printf("x = %i , y = %i \n", x,y);
    return (x > rect.x && x < (rect.x + rect.w)) && (y > rect.y && y < (rect.y + rect.h));
}

int SDL_RenderDrawCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;
    status = 0;

    while (offsety >= offsetx)
    {
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (status < 0)
        {
            status = -1;
            break;
        }

        if (d >= 2 * offsetx)
        {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (d < 2 * (radius - offsety))
        {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else
        {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

int SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;
    status = 0;

    while (offsety >= offsetx)
    {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx, x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety, x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety, x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx, x + offsety, y - offsetx);

        if (status < 0)
        {
            status = -1;
            break;
        }

        if (d >= 2 * offsetx)
        {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (d < 2 * (radius - offsety))
        {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else
        {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

void drawCountryPoint(country_list_t *cl, texture_list_t *textures)
{   
    int i;
    for(i = 0 ; i<cl->nb; i++){
        SDL_SetTextureAlphaMod(textures->textures[i], cl->liste[i]->compromised_pcs_cpt * 255 / (cl->liste[i]->healthy_pcs_cpt + cl->liste[i]->compromised_pcs_cpt));
    }
    
    
}

texture_list_t *creer_texture_list(SDL_Renderer *render, char fichierloc[25])
{
    FILE *fichier;
    int read;
    int nblignes = compter_lignes(fichierloc);


    fichier = fopen(fichierloc, "r");
    printf("Nombre de textures : %i \n", nblignes + 1);
    texture_list_t *texture_list = malloc(sizeof(texture_list_t));

    texture_list->textures = malloc(sizeof(texture_list_t) * nblignes);
    texture_list->nb = nblignes;
    int i = 0;
    char passTexture[40];
    SDL_Texture *texture = NULL;

    while (read = fscanf(fichier, "%[^\n]\n", passTexture) != EOF)
    {
        printf("passTexture : %s \n", passTexture);
        texture = IMG_LoadTexture(render, passTexture);
        printf("%p \n", texture);
        texture_list->textures[i] = texture;
        i++;
    }
    printf("I : %d\n", i);
    fclose(fichier);
    return texture_list;
}

void destroyTexture(texture_list_t **texture)
{
    int i;
    for (i = 0; i < (*texture)->nb; i++)
    {
        SDL_DestroyTexture((*texture)->textures[i]);
    }
    free((*texture)->textures);
    free((*texture));
}

void genererShop(SDL_Rect *Items, SDL_Renderer * Render, upgrade_list_t * up_list, TTF_Font *font)
{
    SDL_Color white = {255, 255, 255};
    char buffer[50];
    int index, posX, posY, ligne = 0, colone = 0;
    int nbMax = up_list->nb <= 8 ? up_list->nb : 8;
    for (index = 0; index < nbMax; index++)
    {
        initRect(Render, &Items[index], 136 + (100 * colone) + (136 * colone), 80 + 200 * (ligne), 150, 150, 118, 118, 118, 255);
        snprintf(buffer, 50, "%s", up_list->liste[index]->name);
        showSmallerText(Render, &Items[index], buffer, font, &white, Items[index].y);
        snprintf(buffer, 50, "Prix:%.2f", up_list->liste[index]->price);
        showSmallerText(Render, &Items[index], buffer, font, &white, Items[index].y + 30);
        snprintf(buffer, 50, "Propagation:%.2f", up_list->liste[index]->spreading_rate);
        showSmallerText(Render, &Items[index], buffer, font, &white, Items[index].y + 60);
        snprintf(buffer, 50, "Recherche:%.2f", up_list->liste[index]->research_rate);
        showSmallerText(Render, &Items[index], buffer, font, &white, Items[index].y + 90);
        colone++;
        if (colone == 4) /*chaque ligne est composé de 4 colones toutes le 4 colones on retourne a la ligne*/
        {
            colone = 0;
            ligne++;
        }
    }
}

int saveExist(){
    FILE * jeu, *virus, *country;
    jeu = fopen("../datas/jeuSAVE", "r");
    virus = fopen("../datas/virusSAVE", "r");
    country = fopen("../datas/countrylistSAVE", "r");
    if(jeu == NULL && virus == NULL && country == NULL)
        return 0;
    return 1;

}