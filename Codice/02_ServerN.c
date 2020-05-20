/*ServerN avrà una socket su ServerM e una su ClientN e invia richieste al ServerM
  di eliminazione e inserimento (sulle liste negozi virtuali e prodotti)
*/
#include"header_fun.h"

int main(int argc, char **argv)
{
    int sockfd;
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));
    
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PO_SERVER_N);
    servaddr.sin_addr.s_addr = inet_addr(IP_SERVER_N); // Accetto tutte le connessioni

    Bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    Listen(sockfd, 2048); // Crea una coda di attesa per le richieste

    ///////////////////////////////////////////////////////////////////////
    // ISTRUZIONI PER IL SERVER //

    /*  Da ServerN
            1 - eliminazione_prodotto
            2 - inserimento_prodotto
                        
            3 - eliminazione_negozio
            4 - inserimento_negozio
    */

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

                Pacchetto pacch_ricevuto;
                pacch_ricevuto.n_richiesta = -1;
                char risposta[len_stringa];
                int n_read;
                n_read = FullRead(i, &pacch_ricevuto, sizeof(pacch_ricevuto));

                int sockfd_cl = Socket(AF_INET, SOCK_STREAM, 0);

                struct sockaddr_in cliaddr2;
                cliaddr2.sin_family = AF_INET;
                cliaddr2.sin_port = htons(PO_SERVER_M);
                cliaddr2.sin_addr.s_addr = inet_addr(IP_SERVER_M);

                Connect(sockfd_cl, (struct sockaddr*)&cliaddr2, sizeof(cliaddr2));

                FullWrite(sockfd_cl, &pacch_ricevuto, sizeof(pacch_ricevuto));
                FullRead(sockfd_cl, &risposta, sizeof(risposta));

                FullWrite(i, &risposta, sizeof(risposta));

                if(n_read > 0 || pacch_ricevuto.n_richiesta == 0)
                {
                    printf("Chiusa connessione sul Client %d.\n", i);
                    close(sockfd_cl);
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
    }

    printf("FINE!");
    exit(0);
}
