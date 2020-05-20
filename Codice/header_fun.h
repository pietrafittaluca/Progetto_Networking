#include"header.h"
#define len_stringa 50

#define IP_SERVER_M "127.0.0.1"
#define PO_SERVER_M 1025

#define IP_SERVER_N "127.0.0.1"
#define PO_SERVER_N 1026

#define IP_SERVER_C "127.0.0.1"
#define PO_SERVER_C 1027

//////////////////////////////////////////////////////////////////////
//////////////////// Struttura Pacchetto
///////////////////

typedef struct pacchetto_prodotto
{
    int n_richiesta;

    //Prodotto
    char nome_prodotto[len_stringa];
    float prezzo;

    //Negozio
    char nome_negozio[len_stringa];
}Pacchetto;

//////////////////////////////////////////////////////////////////////
//////////////////// LISTA
///////////////////

typedef struct nodo_prodotto
{
    char nome_prodotto[len_stringa];
    float prezzo;
    struct nodo_prodotto *prev, *next;
}Nodo_prodotto;

typedef struct nodo_negozio
{
    char nome_negozio[len_stringa];
    Nodo_prodotto *Lista_prodotti_head, *Lista_prodotti_coda, *Lista_prodotti_work;

    struct nodo_negozio *prev, *next;
}Nodo_negozio;

Nodo_negozio *Lista_negozi_head = NULL, *Lista_negozi_coda = NULL, *Lista_negozi_work = NULL;

//////////////////////////////////////////////////////////////////////
//////////////////// FUNCTION LISTA
///////////////////

Nodo_negozio* ricerca_negozio(char nome_negozio[len_stringa])
{
    Nodo_negozio *temp = Lista_negozi_head;

    while(temp != NULL)
    {
        if(strcmp(temp->nome_negozio, nome_negozio) == 0)
            return temp;
        
        temp = temp->next;
    }

    return NULL;
}

Nodo_prodotto* ricerca_prodotto(char nome_prodotto[len_stringa])
{
    Nodo_negozio *temp_negozio = Lista_negozi_head;

    while (temp_negozio != NULL)
    {
        Nodo_prodotto *temp_prodotto = temp_negozio->Lista_prodotti_head;
        while(temp_prodotto != NULL)
        {
            if(strcmp(temp_prodotto->nome_prodotto, nome_prodotto) == 0)
                return temp_prodotto;
            
            temp_prodotto = temp_prodotto->next;
        }

        temp_negozio = temp_negozio->next;
    }
    
    return NULL;
}

int inserimento_negozio(char nome_negozio[len_stringa])
{
    if(ricerca_negozio(nome_negozio) != NULL)
    {
        printf("Negozio gia' presente.\n");
        return -1;
    }

    Nodo_negozio *nuovo_nodo;
	nuovo_nodo = calloc(1, sizeof(Nodo_negozio));
	strcpy(nuovo_nodo->nome_negozio, nome_negozio);
    nuovo_nodo->Lista_prodotti_head = NULL;
    nuovo_nodo->Lista_prodotti_coda = NULL;
    nuovo_nodo->Lista_prodotti_work = NULL;

	if (Lista_negozi_head == NULL)///Inserimento primo elemento
	{
		Lista_negozi_head = nuovo_nodo;
		Lista_negozi_coda = nuovo_nodo;

		Lista_negozi_head->next = NULL;
		Lista_negozi_coda->next = NULL;
		Lista_negozi_work = Lista_negozi_head;///In work ho l'ultimo elemento inserito
	}
	else
	{
		Lista_negozi_work->next = nuovo_nodo;
		Lista_negozi_coda = Lista_negozi_work->next;
		Lista_negozi_coda->prev = Lista_negozi_work;

		Lista_negozi_work = Lista_negozi_coda;
		Lista_negozi_work->next = NULL;
	}

    return 0;
}

int inserimento_prodotto(char nome_negozio[len_stringa], char nome_prodotto[len_stringa], float prezzo)
{
    Nodo_negozio *nodo_negozio = ricerca_negozio(nome_negozio);
    Nodo_prodotto *nodo_prodotto = ricerca_prodotto(nome_prodotto);

    if(nodo_negozio == NULL)
    {
        printf("Il negozio non esiste.\n");
        return -1;
    }
    else if(nodo_prodotto != NULL)
    {
        printf("Prodotto gia' presente.\n");
        return -1;
    }
    else
    {
        Nodo_prodotto *nuovo_nodo;
        nuovo_nodo = calloc(1, sizeof(Nodo_prodotto));
        strcpy(nuovo_nodo->nome_prodotto, nome_prodotto);
        nuovo_nodo->prezzo = prezzo;

        if (nodo_negozio->Lista_prodotti_head == NULL)///Inserimento primo elemento
        {
            nodo_negozio->Lista_prodotti_head = nuovo_nodo;
            nodo_negozio->Lista_prodotti_coda = nuovo_nodo;

            nodo_negozio->Lista_prodotti_head->next = NULL;
            nodo_negozio->Lista_prodotti_coda->next = NULL;
            nodo_negozio->Lista_prodotti_work = nodo_negozio->Lista_prodotti_head;///In work ho l'ultimo elemento inserito
        }
        else
        {
            nodo_negozio->Lista_prodotti_work->next = nuovo_nodo;
            nodo_negozio->Lista_prodotti_coda = nodo_negozio->Lista_prodotti_work->next;
            nodo_negozio->Lista_prodotti_coda->prev = nodo_negozio->Lista_prodotti_work;

            nodo_negozio->Lista_prodotti_work = nodo_negozio->Lista_prodotti_coda;
            nodo_negozio->Lista_prodotti_work->next = NULL;
        }
    }

    return 0;
}

Nodo_prodotto* inserimento_in_lista(char nome_prodotto[len_stringa], Nodo_prodotto *L)
{
    Nodo_prodotto *nuovo_nodo, *Lista;
    Lista = L;
	nuovo_nodo = calloc(1, sizeof(Nodo_prodotto));
	strcpy(nuovo_nodo->nome_prodotto, nome_prodotto);
    nuovo_nodo->next = NULL;
    nuovo_nodo->prev = NULL;

	if (Lista == NULL)///Inserimento primo elemento
	{
        Lista = nuovo_nodo;
        Lista->next = NULL;
        Lista->prev = NULL;
	}
	else
	{
        Lista->next = nuovo_nodo;
        Lista->next->next = NULL;
        Lista->next->prev = Lista;

        Lista = Lista->next;
	}

    return Lista;
}

int eliminazione_negozio(char nome_negozio[len_stringa])
{
	Nodo_negozio *temp, *prev, *curr;

	if (Lista_negozi_head != NULL)
	{
		for (prev = NULL, curr = Lista_negozi_head; curr != NULL && (strcmp(curr->nome_negozio, nome_negozio) != 0); prev = curr, curr = curr->next);
		//corr = nodo che sto cercando, prev = precedente al nodo che sto cercando
		
        if (curr == NULL)
		{
			printf("Negozio non trovato.\n");
            return -1;
		}
		else if (prev == NULL)///Eliminazione in testa
		{
			temp = Lista_negozi_head->next;
			free(Lista_negozi_head);
			Lista_negozi_head = temp;
			Lista_negozi_head->prev = NULL;
            return 0;
		}
		else///Eliminazione in mezzo
		{
			if (curr->next == NULL)
			{
                Lista_negozi_coda = prev;
                Lista_negozi_coda->next = NULL;
                Lista_negozi_work = Lista_negozi_coda;
                free(curr);
				// Lista_negozi_head = prev;
				// prev->next = NULL;
                // free(curr);
			}
			else
			{
                prev->next = curr->next;
                curr->next->prev = prev;
                free(curr);
			}
            return 0;
		}
	}
    else
		printf("\t\t\tNON CI SONO ELEMENTI DA ELIMINARE\n"); // Lista vuota
    
    return -1;
}

int eliminazione_prodotto(char nome_prodotto[len_stringa])
{
    Nodo_negozio *temp_negozio = Lista_negozi_head;

    while(temp_negozio != NULL)
    {
        Nodo_prodotto *temp, *prev, *curr;
    
        if (temp_negozio->Lista_prodotti_head != NULL)
        {
            for (prev = NULL, curr = temp_negozio->Lista_prodotti_head; curr != NULL && (strcmp(curr->nome_prodotto, nome_prodotto) != 0); prev = curr, curr = curr->next);
            //corr = nodo che sto cercando, prev = precedente al nodo che sto cercando
            
            if (curr == NULL)
            {
                printf("Prodotto non trovato nel negozio '%s'.\n", temp_negozio->nome_negozio);
            }
            else if (prev == NULL)///Eliminazione in testa
            {
                temp = temp_negozio->Lista_prodotti_head->next;
                free(temp_negozio->Lista_prodotti_head);
                temp_negozio->Lista_prodotti_head = temp;
                temp_negozio->Lista_prodotti_head->prev = NULL;
                return 0;
            }
            else///Eliminazione in mezzo
            {
                if (curr->next == NULL)
                {
                    temp_negozio->Lista_prodotti_coda = prev;
                    temp_negozio->Lista_prodotti_coda->next = NULL;
                    temp_negozio->Lista_prodotti_work = temp_negozio->Lista_prodotti_coda;
                    free(curr);
                    // temp_negozio->Lista_prodotti_head = prev;
                    // prev->next = NULL;
                }
                else
                {
                    prev->next = curr->next;
                    curr->next->prev = prev;
                    free(curr);
                }
                return 0;
            }
        }
        else
            printf("Lista prodotti vuota nel negozio '%s'.\n", temp_negozio->nome_negozio); // Lista vuota
        
        temp_negozio = temp_negozio->next;
    }

    return -1;
}

//////////////////////////////////////////////////////////////////////
//////////////////// VISUALIZZAZIONI
///////////////////

char* elenco_prodotti_del_negozio(char nome_negozio[len_stringa])
{
    char *elenco = calloc(len_stringa*len_stringa, sizeof(char));
    Nodo_negozio *temp = ricerca_negozio(nome_negozio);

    if(temp == NULL)
        return NULL;

    Nodo_prodotto *ttemp = temp->Lista_prodotti_head;
    while(ttemp != NULL)
    {
        strcat(elenco, ttemp->nome_prodotto);
        strcat(elenco, "\n");
        ttemp = ttemp->next;
    }

    return elenco;
}

char* elenco_negozi()
{
    char *elenco = calloc(len_stringa*len_stringa, sizeof(char));
    Nodo_negozio *temp = Lista_negozi_head;

    if(temp == NULL)
        return NULL;

    while(temp != NULL)
    {
        strcat(elenco, temp->nome_negozio);
        strcat(elenco, "\n");
        temp = temp->next;
    }

    return elenco;
}

void visualizza_tutto()
{
    Nodo_negozio *temp_negozio = Lista_negozi_head;
    while (temp_negozio != NULL)
    {
        Nodo_prodotto *temp_prodotto = temp_negozio->Lista_prodotti_head;
        printf("Nome Negozio: %s\n", temp_negozio->nome_negozio);
        while(temp_prodotto != NULL)
        {
            printf("\t\t%s\n", temp_prodotto->nome_prodotto);
            temp_prodotto = temp_prodotto->next;
        }

        temp_negozio = temp_negozio->next;
    }
}

//////////////////////////////////////////////////////////////////////
//////////////////// INIZIALIZZAZIONE
///////////////////

void inizializzazione()
{
    inserimento_negozio("Negozio_dolci");
    inserimento_negozio("Negozio_casa");
    inserimento_negozio("Negozio_abbigliamento");
    inserimento_negozio("Negozio_elettronica");
    inserimento_negozio("Negozio_farmacia");

    inserimento_prodotto("Negozio_dolci", "Caramelle 1 Kg", 8.99F);
    inserimento_prodotto("Negozio_dolci", "Gomme 500g", 5.99F);
    inserimento_prodotto("Negozio_dolci", "Cioccolato 250g", 3.99F);
    inserimento_prodotto("Negozio_dolci", "Biscotti 1 Kg", 2.99F);
    inserimento_prodotto("Negozio_dolci", "Torte 1 Kg", 15.99F);

    inserimento_prodotto("Negozio_casa", "Tavolo", 50.99F);
    inserimento_prodotto("Negozio_casa", "Sedia", 15.99F);
    inserimento_prodotto("Negozio_casa", "Tovaglia", 15.99F);

    inserimento_prodotto("Negozio_abbigliamento", "Maglia L", 10.99F);
    inserimento_prodotto("Negozio_abbigliamento", "Maglia M", 10.99F);
    inserimento_prodotto("Negozio_abbigliamento", "Maglia XS", 10.99F);
    inserimento_prodotto("Negozio_abbigliamento", "Maglia XL", 10.99F);
    inserimento_prodotto("Negozio_abbigliamento", "Pantalone M", 22.99F);
    inserimento_prodotto("Negozio_abbigliamento", "Pantalone L", 22.99F);
    inserimento_prodotto("Negozio_abbigliamento", "Pantalone XL", 21.99F);
    inserimento_prodotto("Negozio_abbigliamento", "Scarpa 42", 55.99F);
    inserimento_prodotto("Negozio_abbigliamento", "Scarpa 46", 55.99F);

    inserimento_prodotto("Negozio_elettronica", "HardDisk 1 TB", 49.99F);
    inserimento_prodotto("Negozio_elettronica", "Mouse", 39.99F);
    inserimento_prodotto("Negozio_elettronica", "Tastiera", 69.99F);
    inserimento_prodotto("Negozio_elettronica", "Stampante Laser", 105.99F);
    inserimento_prodotto("Negozio_elettronica", "SSD 256 GB", 79.99F);

    inserimento_prodotto("Negozio_farmacia", "Tachipirina 1000", 12.99F);
    inserimento_prodotto("Negozio_farmacia", "Neambutal 100g", 150.99F);
    inserimento_prodotto("Negozio_farmacia", "OKi 15 bustine", 15.99F);
}
