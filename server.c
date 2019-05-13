#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>


#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>


int main(int argc, char const *argv[]) {

        //-- Definição das estruturas de endereço do cliente e do servidor
        struct sockaddr_in adressServer;
        struct sockaddr_in adressClient;

        int port, buffer_size;
        int header[3];
        int server_sock;
        int confirm_recv;

        //--- inteiro que armazena o tamanho da estrutra de endereço do cliente
        socklen_t csize = sizeof adressClient;

        //-- Estrutura para a temporização
        struct timeval start, end;
        float time_waiting;

        char fileName[255];

        memset(fileName,0,255);


        int openSocket(int port){
			

                /*--Aqui temos a definição do nosso socket e os parementros quem definem
                   AF_INET - Define que trabalhara com a internet, SOCK_DGRAM - Uso do UDP
                   0 - Defini o uso do IP*/
                int sockfd;   
                   
                sockfd = socket(AF_INET, SOCK_DGRAM, 0);

                adressServer.sin_family = AF_INET;
                adressServer.sin_port = htons(port);
                adressServer.sin_addr.s_addr = htonl(INADDR_ANY);

                //-- Testa se o socket foi criado
                if(sockfd == -1) {
                        printf("\n Erro na criação do socket");
                        exit(1);
                }else
                        printf("\n Socket Criado com sucesso");

                //--Vincula um socket com um nome
                if(bind(sockfd,(struct sockaddr*)&adressServer, sizeof(adressServer)) == -1) {
                        printf("\n Erro de vinculação");
                        exit(1);
                }
                return sockfd;
        }

        //-- Intreção para definir a porta que será usada a mensagem e o tamanho do buffer
        printf("\n\n Digite a porta que deseja abrir:");
        scanf("%d",&port);
        printf("\n\n Digite o tamanho do buffer:");
        scanf("%d",&buffer_size);
        server_sock = openSocket(port);


        //-- Servidor ativo
        while(1)
        {
                printf("\n\nServidor esperando conexões...\n");

                //-- Recebe mensagem do cliente contendo nome do arquivo a ser enviado:
                memset(&fileName,0,255);
                int mensagem_len;
                if((mensagem_len = recvfrom(server_sock, fileName, 255, 0, (struct sockaddr *)&adressClient, &csize)) > 0) {
                        fileName[mensagem_len] = '\0';
                        printf("O arquivo solicidado pelo cliente é : %s\n",fileName);
                }

                //-- Enviando arquivo para o cliente.

                // -- cria o pronteiro de arquvo
                FILE *arq;

                arq = fopen(fileName, "r");
                if (arq == NULL) {
                        printf("\nERRO! O arquivo não foi aberto");
                }

                memset(&fileName,0,255);

                char buffer[buffer_size];



                while(!feof(arq)) {
                        memset(&buffer,0,buffer_size);
                        fread(&buffer,1,buffer_size,arq );
                        
                        gettimeofday(&start, NULL);
                        sendto(server_sock, buffer, buffer_size,0, (struct sockaddr *)&adressClient, sizeof(struct sockaddr_in));
                        while (1) {
                                gettimeofday(&end, NULL);
                                time_waiting = (float)((((end.tv_sec - start.tv_sec)*1000000.0 + end.tv_usec - start.tv_usec))/1000000.0);
                                if (time_waiting >= 0.1) {
                                        do {
                                                confirm_recv = recvfrom(server_sock, header, sizeof(header),0, (struct sockaddr *)&adressClient, &csize);

                                        } while (confirm_recv <= 0);

                                        if(header[2] == '1') {
                                                printf("\n[Servidor] Cliente confirmou recebimento...");

                                                break;
                                        }else{
                                                printf("\n[Servidor] Reenviando...");

                                                sendto(server_sock, buffer, buffer_size,0, (struct sockaddr *)&adressClient, sizeof(struct sockaddr_in));

                                                gettimeofday(&start, NULL);
                                        }
                                }




                        }
                }
                

                fclose(arq);

        }

        return 0;
}
