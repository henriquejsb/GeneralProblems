#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <string>

int nchanges;
int tulips[1500];
int ntulips;
int best_case = 1500;

void print_tulips(){
	int soma = 0;
	printf("%d | %d :: ",nchanges,ntulips);
	for(int i = 0; i < ntulips; i++){
		printf("%d ",tulips[i]);
		soma += tulips[i];
	}
	printf("\n%d\n",soma);
	
}

void solution(int i, bool odd, int changes_left, int wrong_aux){
	if(i == ntulips){
			if(wrong_aux < best_case){
				best_case = wrong_aux;

			}
			//printf("->%d\n",wrong_aux);
			return;
	}
	if(odd){
		if(i%2 == 0){
			wrong_aux += tulips[i];
		}
	}
	else{
		if(i%2 == 1){
			wrong_aux += tulips[i];
		}
	}
	if(changes_left - 1 >= 0){
		solution(i+1,!odd,changes_left-1,wrong_aux);
	}
	solution(i+1,odd,changes_left, wrong_aux);
}




int main(){
	int aux;
	char aux2;
	int conta = 0;
	ntulips = 1;
	bool zero = false;
	//scanf("%d ",&nchanges);
	while(scanf("%d ",&nchanges) != EOF){

		//clear_dp();
		best_case = 1500;
		ntulips = 1;
		conta = 0;
		//printf("----%d\n",nchanges);
		zero = false;
		while(scanf("%c ",&aux2) != EOF){
			//printf("%c",aux2);
			if(aux2 == '\n'){
				//printf("\n");
				break;
			} 
			//printf("%c\n",aux2);
			aux = aux2 - '0';
			
			//printf("%c\n",aux);
			if(aux == 0){
				if(zero){
					conta +=1;
					
				}
				else{
					if(conta != 0){
						tulips[ntulips-1] = conta;
						//printf("%d - ",conta);
						ntulips += 1;
						}
					
						conta = 1;
						
						zero = true;
					
				}
			}
			else{
				if(zero){
					tulips[ntulips-1] = conta;
					//printf("%d - ",conta);
					ntulips += 1;
					conta = 1;
					zero = false;
				}
				else{
					conta +=1;
				}
			}

		}
		tulips[ntulips-1]= conta;
		/*
		printf(" %d\n------------------------------------\n\n",conta);
		int ntulips_aux = ntulips;
		printf("\t");
		for(int i = 0; i <= ntulips; i++){
			printf("%d\t",i);
		}
		printf("\n----------------------------------------------------------------------------------------------------------------\n");
		for(int i = 1; i <= nchanges; i++){
			printf("%03d||",i);
			for(int j = 0; j <= ntulips_aux; j++){
				ntulips = j;
				best_case = 1500;
				solution(0,true,i-1,0);
				solution(0,false,i-1,0);
				printf("\t%d",best_case);
			}
			printf("\n");
		}
		*/
		solution(0,true,nchanges-1,0);
		solution(0,false,nchanges-1,0);
		printf("%d\n",best_case);
		
	}
	
	//print_tulips();
	return 0;
}