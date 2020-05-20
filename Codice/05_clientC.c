/*Client avrà una socket su ServerC e consente di inserire prodotti di diversi
  negozi virtuali in una lista degli acquisti e di visualizzare la lista degli
  acquisti.
*/
#include"header_fun.h"

int main(int argc, char **argv)
{
    //1-CREO LA SOCKET
    int sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PO_SERVER_C);
    servaddr.sin_addr.s_addr = inet_addr(IP_SERVER_C);

    //2-CONNECTION
    Connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    ///////////////////////////////////////////////////////////////////////
    // ISTRUZIONI PER IL CLIENT //

    /*  Da ClientC
            5 - ricerca_prodotto
            6 - elenco_prodotti_negozio
            
            7 - ricerca_negozio
            8 - elenco_negozi
    */
    Nodo_prodotto *Lista_acquisti = NULL;
    fd_set set;
    int max_fd, scelta = -1;

    while(scelta != 0)
    {
        FD_ZERO(&set); //Reset dei bit
        FD_SET(STDIN_FILENO, &set); //Inserisco in set il fd dello STDIN
        FD_SET(sockfd, &set); //Inserisco in set il fd del socket

        if(sockfd > STDIN_FILENO)
            max_fd = sockfd;
        else
            max_fd = STDIN_FILENO;

        fputs("\nPremi [0] per uscire.\nPremi [5] per ricercare un prodotto.\nPremi [6] per visualizzare l'elenco prodotti.\nPremi [7] per ricercare un negozio.\nPremi [8] per visualizzare l'elenco negozi.\n", stdout);
        fputs("\nPremi [9] per inserire un prodotto nella lista degli acquisti.\nPremi [10] per visualizzare la lista degli acquisti.\n", stdout);
        
        Select(max_fd+1, &set, NULL, NULL, NULL);
        
        system("clear");
        fflush(stdin);
        fflush(stdout);
        
        /*Invio la richiesta*/
        if(FD_ISSET(STDIN_FILENO, &set))
        {
            Pacchetto pacch_inviato;
            Nodo_prodotto *temp;
            char buf[len_stringa];
            fgets(buf, sizeof(buf), stdin);
            scelta = atoi(buf);
            //scanf("%d", &scelta);Nodo_prodotto *Lista_acquisti = NULL;

            switch (scelta)
            {
            case 0:
                pacch_inviato.n_richiesta = scelta;
                FullWrite(sockfd, &pacch_inviato, sizeof(pacch_inviato));
                exit(0);
            case 5:
                // Ricerca Prodotto

                fputs("Inserisci il nome del prodotto da cercare: ", stdout);
                fgets(pacch_inviato.nome_prodotto, sizeof(pacch_inviato.nome_prodotto), stdin);
                //scanf("%s", buf);

                pacch_inviato.nome_prodotto[strlen(pacch_inviato.nome_prodotto) - 1] = '\0';
                pacch_inviato.n_richiesta = scelta;
                FullWrite(sockfd, &pacch_inviato, sizeof(pacch_inviato));
                
                break;
            case 6:
                // Visualizza elenco prodotti del negozio
                
                fputs("Inserisci il nome del negozio di cui visualizzare i prodotti: ", stdout);
                fgets(pacch_inviato.nome_negozio, sizeof(pacch_inviato.nome_negozio), stdin);

                pacch_inviato.nome_negozio[strlen(pacch_inviato.nome_negozio) - 1] = '\0';
                pacch_inviato.n_richiesta = scelta;
                FullWrite(sockfd, &pacch_inviato, sizeof(pacch_inviato));

                break;
            case 7:
                // Ricerca negozio

                fputs("Inserisci il nome del negozio da cercare: ", stdout);
                fgets(pacch_inviato.nome_negozio, sizeof(pacch_inviato.nome_negozio), stdin);

                pacch_inviato.nome_negozio[strlen(pacch_inviato.nome_negozio) - 1] = '\0';
                pacch_inviato.n_richiesta = scelta;
                FullWrite(sockfd, &pacch_inviato, sizeof(pacch_inviato));

                break;
            case 8:
                // Visualizza elenco negozi

                pacch_inviato.n_richiesta = scelta;
                FullWrite(sockfd, &pacch_inviato, sizeof(pacch_inviato));
                
                break;

            case 9:
                // Inserisci prodotto nella lista acquisti

                fputs("Inserisci il nome del prodotto da inserire nella lista acquisti: ", stdout);
                fgets(pacch_inviato.nome_prodotto, sizeof(pacch_inviato.nome_prodotto), stdin);

                pacch_inviato.nome_prodotto[strlen(pacch_inviato.nome_prodotto) - 1] = '\0';
                pacch_inviato.n_richiesta = scelta;
                FullWrite(sockfd, &pacch_inviato, sizeof(pacch_inviato));

                break;
            case 10:
                // Visualizza lista degli acquisti

                temp = Lista_acquisti;
                while(temp != NULL)
                {
                    printf("%s\n", temp->nome_prodotto);
                    temp = temp->prev;
                }

                break;

            default:
                printf("Scelta sbagliata!\n");
                break;
            }
        }

        /*Ricevo la risposta*/
        if(FD_ISSET(sockfd, &set))
        {
            char buf[len_stringa*len_stringa];

            FullRead(sockfd, &buf, sizeof(buf));

            if(strcmp(buf, "TRUE") == 0)
            {
                FullRead(sockfd, &buf, sizeof(buf));
                Lista_acquisti =  inserimento_in_lista(buf, Lista_acquisti);
            }
            else
                fputs(buf, stdout);
        }
    }
    
    exit(0);
}
