/*ClientN  avrà una socket su ServerN e consente di gestire
  i negozi virtuali.
*/
#include"header_fun.h"

int main(int argc, char **argv)
{
    //1-CREO LA SOCKET
    int sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PO_SERVER_N);
    servaddr.sin_addr.s_addr = inet_addr(IP_SERVER_N);

    //2-CONNECTION
    Connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    ///////////////////////////////////////////////////////////////////////
    // ISTRUZIONI PER IL CLIENT //

    /*  Da ClientN
            1 - eliminazione_prodotto
            2 - inserimento_prodotto
                        
            3 - eliminazione_negozio
            4 - inserimento_negozio
    */

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

        fputs("\nPremi [0] per uscire.\nPremi [1] per eliminare un prodotto.\nPremi [2] per inserire un prodotto.\nPremi [3] per eliminare un negozio.\nPremi [4] per inserire un negozio.\n", stdout);

        Select(max_fd+1, &set, NULL, NULL, NULL);
        
        system("clear");
        fflush(stdin);
        fflush(stdout);
        
        /*Invio la richiesta*/
        if(FD_ISSET(STDIN_FILENO, &set))
        {
            Pacchetto pacch_inviato;
            char buf[len_stringa];
            fgets(buf, sizeof(buf), stdin);
            scelta = atoi(buf);
            //scanf("%d", &scelta);

            switch (scelta)
            {
            case 0:
                pacch_inviato.n_richiesta = scelta;
                FullWrite(sockfd, &pacch_inviato, sizeof(pacch_inviato));
                exit(0);
            case 1:
                // Elimina un prodotto

                fputs("Inserisci il nome del prodotto da eliminare: ", stdout);
                fgets(pacch_inviato.nome_prodotto, sizeof(pacch_inviato.nome_prodotto), stdin);
                //scanf("%s", buf);

                pacch_inviato.nome_prodotto[strlen(pacch_inviato.nome_prodotto) - 1] = '\0';
                pacch_inviato.n_richiesta = scelta;
                FullWrite(sockfd, &pacch_inviato, sizeof(pacch_inviato));
                
                break;
            case 2:
                // Inserisci prodotto

                fputs("Inserisci il nome del negozio: ", stdout);
                fgets(pacch_inviato.nome_negozio, sizeof(pacch_inviato.nome_negozio), stdin);
                fputs("Inserisci il nome del prodotto: ", stdout);
                fgets(pacch_inviato.nome_prodotto, sizeof(pacch_inviato.nome_prodotto), stdin);

                pacch_inviato.nome_negozio[strlen(pacch_inviato.nome_negozio) - 1] = '\0';
                pacch_inviato.nome_prodotto[strlen(pacch_inviato.nome_prodotto) - 1] = '\0';
                pacch_inviato.n_richiesta = scelta;
                FullWrite(sockfd, &pacch_inviato, sizeof(pacch_inviato));

                break;
            case 3:
                // Elimina negozio

                fputs("Inserisci il nome del negozio da eliminare: ", stdout);
                fgets(pacch_inviato.nome_negozio, sizeof(pacch_inviato.nome_negozio), stdin);

                pacch_inviato.nome_negozio[strlen(pacch_inviato.nome_negozio) - 1] = '\0';
                pacch_inviato.n_richiesta = scelta;
                FullWrite(sockfd, &pacch_inviato, sizeof(pacch_inviato));

                break;
            case 4:
                // Inserisci negozio

                fputs("Inserisci il nome del negozio: ", stdout);
                fgets(pacch_inviato.nome_negozio, sizeof(pacch_inviato.nome_negozio), stdin);

                pacch_inviato.nome_negozio[strlen(pacch_inviato.nome_negozio) - 1] = '\0';
                pacch_inviato.n_richiesta = scelta;
                FullWrite(sockfd, &pacch_inviato, sizeof(pacch_inviato));
                
                break;
            default:
                printf("Scelta sbagliata!\n");
                break;
            }
        }

        /*Ricevo la risposta*/
        if(FD_ISSET(sockfd, &set))
        {
            char buf[len_stringa];

            FullRead(sockfd, &buf, sizeof(buf));
            fputs(buf, stdout);
        }
    }
    
    exit(0);
}
