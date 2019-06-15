#include<stdio.h>
int main()
{
	int windowsize,sent=0,ack,i; // Declaração de variaveis

	printf("enter window size\n");
	scanf("%d",&windowsize);  // leitura do tamanho da janela deslizante

/* Transmite os quadros até chegar ao número de quadros da janela
depois perguntar qual o último quadro trasmitido e enviar a partir 
do uúltimo ACK recebido, o loop faz isso até todos os quadros serem
entregues*/
	while(1)
	{
		for( i = 0; i < windowsize; i++)
			{
				printf("Frame %d has been transmitted.\n",sent);
				sent++;
				if(sent == windowsize)
					break;
			}
			printf("\nPlease enter the last Acknowledgement received.\n");
			scanf("%d",&ack);
			
			if(ack == windowsize)
				break;
			else
				sent = ack;
	}
return 0;
}
