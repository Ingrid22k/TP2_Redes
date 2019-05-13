#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[]) {

        //-- Definição das estruturas de endereço do cliente
        struct sockaddr_in adressServer;


        int port, buffer_size, num_bytes=0;
        int sockfd;
        int confirm_send, confirm_recv;
        int header[3];


        clock_t tempo;

        //--- inteiro que armazena o tamanho da estrutra de endereço do cliente
        socklen_t csize = sizeof adressServer;


        char fileName[255];
        char server_adress[15];


        //-- Intreção para definir a porta que será usada a mensagem e o tamanho do buffer
        printf("\n\n Qual o enredereço(IP) do servidor que deseja acessar:");
        scanf("%s",server_adress);
        printf("\n\n Qual porta será usada na comunicação:");
        scanf("%d",&port);
        printf("\n\n Digite o tamanho do buffer:");
        scanf("%d",&buffer_size);
        buffer_size = buffer_size*8;

        /*--Aqui temos a definição do nosso socket e os parementros quem definem
           AF_INET - Define que trabalhara com a internet, DGRAM - Uso do UDP
           0 - Defini o uso do IP*/


        sockfd = socket(AF_INET, SOCK_DGRAM, 0);

        adressServer.sin_family = AF_INET;
        adressServer.sin_port = htons(port);
        adressServer.sin_addr.s_addr = inet_addr(server_adress);

        //-- Testa se o socket foi criado
        if(sockfd == -1) {
                printf("\n Erro na criação do socket");
                exit(1);
        }else
                printf("\n Socket Criado com sucesso");


        memset(fileName,0x0,255);

        printf("\n\n Qual o nome do arquivo que deseja baixar:");
        scanf("%s",fileName);

        tempo=clock();

		sendto(sockfd, fileName, sizeof(fileName),0, (struct sockaddr *)&adressServer, sizeof(struct sockaddr_in));
        printf("%s\n",fileName);

        // -- cria o pronteiro de arquvo
        FILE *arq;

        arq = fopen(fileName, "w+");


        if(arq == NULL) {
                printf("Erro na criação do arquivo");
                return 1;
        }

        char buffer[buffer_size];


        while(1) {
                memset(&buffer,0,buffer_size);
                //---- Recebe datagrama
                do {
                        confirm_recv = recvfrom(sockfd, buffer, sizeof(buffer),0, (struct sockaddr *)&adressServer, &csize);
                } while (confirm_recv<=0);

                header[2]='1';

                do {
                        confirm_send = sendto(sockfd, header, sizeof(header),0, (struct sockaddr *)&adressServer, sizeof(struct sockaddr_in));
                        if (confirm_send <= 0);
                        printf("\n[Cliente] Enviando confirmação... ");

                } while (confirm_send <= 0);

                // ESCREVE NO ARQUIVO
                fprintf(arq, "%s", buffer);
                printf("\nBytes já tranferidos: %d",num_bytes);
                num_bytes += sizeof(arq);
                if(feof(arq)) break;
               
        }


        num_bytes = sizeof(arq);
        printf("\nBytes já tranferidos: %d",num_bytes);

        fclose(arq);

        printf("\n\nArquivo tranferido com sucesso.\nTempo gasto: %f segundos \n",(clock()-tempo)/(double)CLOCKS_PER_SEC);

        if(fputs(buffer, stdout) == EOF)
        {
                printf("\nStandard output error");
        }


        return 0;
}
