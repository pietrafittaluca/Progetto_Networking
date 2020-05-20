/*ServerM ha una socket su ServerN e una su ServerC e gestirà due liste (una per negozi virtuali e una su prodotti)
  su cui effettuare ricerca, inserimento ed eliminazione
*/
#include"header_fun.h"

int main(int argc, char **argv)
{
    inizializzazione();
    visualizza_tutto();

    int sockfd;
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
    
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PO_SERVER_M);
    servaddr.sin_addr.s_addr = inet_addr(IP_SERVER_M); // Accetto tutte le connessioni

    Bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    Listen(sockfd, 2048); // Crea una coda di attesa per le richieste

    
    ///////////////////////////////////////////////////////////////////////
    // ISTRUZIONI PER IL SERVER //

    fd_set set;
    int fd_open[FD_SETSIZE] = {0};
    int i, max_fd, n;
    struct sockaddr_in cliaddr;

    // FD_ZERO(&set);
    // FD_SET(sockfd, &set);

    max_fd = sockfd;
    fd_open[max_fd] = 1;
    for( ; ; )
    {
        FD_ZERO(&set);
        ////// Inizializzazione
        for(i = sockfd; i <= max_fd; i++)
            if(fd_open[i] != 0)
                FD_SET(i, &set);
        
        n = Select(max_fd+1, &set, NULL, NULL, NULL);

        ////// Accetto la connessione
        if(FD_ISSET(sockfd, &set))
        {
            n--;

            int new_fd = Accept(sockfd, (struct sockaddr*)&cliaddr, &(int){sizeof(cliaddr)});

            fd_open[new_fd] = 1;

            if(max_fd < new_fd)
                max_fd = new_fd;
        }

        ////// Istruzioni per servire ogni client
        i = sockfd;
        while(n != 0)
        {
            i++;

            if(fd_open[i] == 0) // Va al prossimo ciclo senza proseguire
                continue;
            
            // SERVO IL CLIENT
            if(FD_ISSET(i, &set))
            {
                n--;

                /*  Da ServerN
                        1 - eliminazione_prodotto
                        2 - inserimento_prodotto
                        
                        3 - eliminazione_negozio
                        4 - inserimento_negozio
                    Da ServerC
                        5 - ricerca_prodotto
                        6 - elenco_prodotti_negozio
                        
                        7 - ricerca_negozio
                        8 - elenco_negozi
                */

                Pacchetto pacch_ricevuto;
                char risposta[len_stringa], risposta_2[len_stringa*len_stringa];
                char *temp;
                int n_read;
                n_read = FullRead(i, &pacch_ricevuto, sizeof(pacch_ricevuto));
                
                switch (pacch_ricevuto.n_richiesta)
                {
                case 1:
                    // Eliminazione Prodotto
                    
                    if(eliminazione_prodotto(pacch_ricevuto.nome_prodotto) == -1)
                    {
                        sprintf(risposta, "Eliminazione del prodotto fallita.\n");
                        FullWrite(i, &risposta, sizeof(risposta));
                    }
                    else
                    {
                        sprintf(risposta, "Eliminazione del prodotto effettuata.\n");
                        FullWrite(i, &risposta, sizeof(risposta));
                    }
                    
                    break;
                case 2:
                    // Inserimento Prodotto

                    if(inserimento_prodotto(pacch_ricevuto.nome_negozio, pacch_ricevuto.nome_prodotto, pacch_ricevuto.prezzo) == -1)
                    {    
                        sprintf(risposta, "Inserimento del prodotto fallito.\n");
                        FullWrite(i, &risposta, sizeof(risposta));
                    }
                    else
                    {
                        sprintf(risposta, "Inserimento del prodotto effettuato.\n");
                        FullWrite(i, &risposta, sizeof(risposta));
                    }

                    break;
                case 3:
                    // Eliminazione Negozio

                    if(eliminazione_negozio(pacch_ricevuto.nome_negozio) == -1)
                    {
                        sprintf(risposta, "Eliminazione del negozio fallita.\n");
                        FullWrite(i, &risposta, sizeof(risposta));
                    }
                    else
                    {
                        sprintf(risposta, "Negozio eliminato.\n");
                        FullWrite(i, &risposta, sizeof(risposta));
                    }

                    break;
                case 4:
                    // Inserimento Negozio

                    if(inserimento_negozio(pacch_ricevuto.nome_negozio) == -1)
                    {
                        sprintf(risposta, "Inserimento del negozio fallito.\n");
                        FullWrite(i, &risposta, sizeof(risposta));
                    }
                    else
                    {
                        sprintf(risposta, "Inserimento del negozio effettuato.\n");
                        FullWrite(i, &risposta, sizeof(risposta));
                    }

                    break;
                case 5:
                    // Ricerca Prodotto

                    if(ricerca_prodotto(pacch_ricevuto.nome_prodotto) == NULL)
                    {
                        sprintf(risposta_2, "Prodotto non presente.\n");
                        FullWrite(i, &risposta_2, sizeof(risposta_2));
                    }
                    else
                    {
                        sprintf(risposta_2, "Prodotto trovato.\n");
                        FullWrite(i, &risposta_2, sizeof(risposta_2));
                    }
                    
                    break;
                case 6:
                    // Elenco Prodotti Negozio

                    temp = elenco_prodotti_del_negozio(pacch_ricevuto.nome_negozio);
                    if(temp == NULL)
                    {
                        sprintf(risposta_2, "Negozio non presente.\n");
                        FullWrite(i, &risposta_2, sizeof(risposta_2));
                    }
                    else
                    {
                        strcpy(risposta_2, temp);
                        FullWrite(i, &risposta_2, sizeof(risposta_2));
                    }
                    
                    break;
                case 7:
                    // Ricerca Negozio

                    if(ricerca_negozio(pacch_ricevuto.nome_negozio) == NULL)
                    {
                        sprintf(risposta_2, "Negozio non presente.\n");
                        FullWrite(i, &risposta_2, sizeof(risposta_2));
                    }
                    else
                    {
                        sprintf(risposta_2, "Negozio trovato.\n");
                        FullWrite(i, &risposta_2, sizeof(risposta_2));
                    }

                    break;
                case 8:
                    // Elenco Negozi

                    temp = elenco_negozi();
                    if(temp == NULL)
                    {
                        sprintf(risposta_2, "Non ci sono negozi.\n");
                        FullWrite(i, &risposta_2, sizeof(risposta_2));
                    }
                    else
                    {
                        strcpy(risposta_2, temp);
                        FullWrite(i, &risposta_2, sizeof(risposta_2));
                    }

                    break;

                case 9:
                    // Inserisci prodotto nella lista acquisti

                    if(ricerca_prodotto(pacch_ricevuto.nome_prodotto) == NULL)
                    {
                        sprintf(risposta_2, "Prodotto non trovato.\n");
                        FullWrite(i, &risposta_2, sizeof(risposta_2));
                    }
                    else
                    {
                        strcpy(risposta_2, "TRUE\0");
                        FullWrite(i, &risposta_2, sizeof(risposta_2));
                        
                        strcpy(risposta_2, pacch_ricevuto.nome_prodotto);
                        FullWrite(i, &risposta_2, sizeof(risposta_2));
                    }

                    break;

                default:
                    break;
                }

                visualizza_tutto();
                    
                printf("Chiusa connessione sul Client %d.\n", i);
                close(i);

                fd_open[i] = 0;
                if(max_fd == i)
                {
                    while(fd_open[i] == 0)
                        i--;
                    max_fd = i;
                    break;
                }
            }
        }
    }

    exit(0);
}
