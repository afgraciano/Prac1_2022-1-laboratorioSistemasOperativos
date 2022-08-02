#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "clientBridge.h"

void write_struct(int fd, unsigned long command, struct complex_struct * struct_address){
    if (ioctl(fd, command, struct_address) == -1){
        perror("Write message error at ioctl");
    }
}

void write_message(int fd, unsigned long command, char * message){
    if (ioctl(fd, command, message) == -1){
        perror("Write message error at ioctl");
    }
}

void read_message(int fd, unsigned long command, char * message){
    if(ioctl(fd, command, message) == -1){
	perror("Read message error at ioctl");
    }
}

void read_message_param(int fd, unsigned long command, int * value){
    if(ioctl(fd, command, value) == -1){
        perror("Read message param error at ioctl");
    }else{
        printf("Copy the messsage from the kernel\n");
    }
}

void write_int(int fd, unsigned long command, int * value){
    if (ioctl(fd, command, value) == -1){
        perror("Write int error at ioctl");
    }
}

void read_int(int fd, unsigned long command, int * value){
    if(ioctl(fd, command, value) == -1){
        perror("Read int error at ioctl");
    }else{
        printf("Copy the int from the kernel\n");
    }
}

int send_empty_command(int fd, unsigned long command){
    int returned_value = ioctl(fd, command);
    if(returned_value == -1){
       perror("Send command error at ioctl");
    }else{
	return returned_value;
        printf("Command OK to the kernel\n");
    }
}

void write_several_messages(int fd){
    write_message(fd, BRIDGE_W_S, "Message 1");
    write_message(fd, BRIDGE_W_S, "Message 2");
    write_message(fd, BRIDGE_W_S, "Message 3");
}


void read_all_messages_alta(int fd){


}

void read_all_messages_colas(int fd){
	char message[100];
	int estadoCola = send_empty_command(fd, BRIDGE_STATE_Q);
	while( estadoCola > 0){
		if(estadoCola == 1){
		read_message(fd, BRIDGE_R_HIGH_PRIOR_Q, message);
		 printf("%s\n", message);
		 
		} else if (estadoCola == 2){
		 
		read_message(fd, BRIDGE_R_MIDDLE_PRIOR_Q, message);
		 printf("%s\n", message);
		}else if (estadoCola ==3) {
		
		read_message(fd, BRIDGE_R_LOW_PRIOR_Q, message);
		 printf("%s\n", message);
		}
	   estadoCola = send_empty_command(fd, BRIDGE_STATE_Q); 
	}
}

void read_all_messages(int fd){
	char message[100];
	while( send_empty_command(fd, BRIDGE_STATE_S) > 0){
	    read_message(fd, BRIDGE_R_S, message);
	    printf("Message: %s\n", message);
	}
}

void read_all_messages_stack(int fd){
	char message[100];
	while( send_empty_command(fd, BRIDGE_STATE_S) > 0){
	    read_message(fd, BRIDGE_R_S, message);
	    printf("Message: %s\n", message);
	}
}


void read_all_messages_list(int fd){
	char message[100];
	while( send_empty_command(fd, BRIDGE_STATE_S) > 0){
	    read_message(fd, BRIDGE_R_S, message);
	    printf("Message: %s\n", message);
	}
}

int main(int argc, char *argv[]){

    const char *file_name = "/dev/bridge"; //used by ioctl
    int fd;

    fd = open(file_name, O_RDWR);
    if (fd == -1){
        perror("Bridge ioctl file open");
        return 2;
    }

    char message[] = "mensajeEspecifico";
    int value = 10;
 
 
 int opcion;
 char temp [11];
  do {
  printf("Practica 1 escoja una de las 10 opciones a ejecutar\n");
  printf("1) Leer las lineas de archivo y mostrarlas en orden invertido\n");
  printf("2) Barajar aleatoriamente las lineas de un archivo\n");
  printf("3) Analizar archivo verificando que cada { y (  tiene sus respectivos } y ) \n");
  printf("4) Cola con prioridades de alto, medio, bajo\n");
  printf("5) Destruccion de lista Completa\n");
  printf("6) Invertir nodos de una lista\n");
  printf("7) Concatenar 2 listas\n");
  printf("8) Rotacion N veces a la derecha de una lista\n");
  printf("9) Limpiar lista de valores identicos\n");
  printf("10) Muestra el valor mayor en lista\n");
  printf("0) Salir 0\n");
  fgets(temp,11,stdin);
  opcion=atoi(temp);
  switch(opcion){
  	
  	case 1:
  		printf("usted selecciono %i\n",opcion);
  		primerPunto(fd);
  	break;
    	
    	case 2:
    		printf("usted selecciono %i\n",opcion);
  		//segundoPunto(fd);
  	break;
  	
  	case 3:
  		printf("usted selecciono %i\n",opcion);
  		tercerPunto(fd);
  	break;
  	
  	case 4:
  		printf("usted selecciono %i\n",opcion);
  		cuartoPunto(fd);
  	break;
  	
  	case 5:
  		printf("usted selecciono %i\n",opcion);
  		//quintoPunto(fd);
  	break;
  	
  	case 6:
  		printf("usted selecciono %i\n",opcion);
  		//sextoPunto(fd);
  	break;
  	
  	case 7:
  		printf("usted selecciono %i\n",opcion);
  		//septimoPunto(fd);
  	break;
  	
  	case 8:
  		printf("usted selecciono %i\n",opcion);
  		//octavoPunto(fd);
  	break;
  	
  	case 9:
  		printf("usted selecciono %i\n",opcion);
  		//novenoPunto(fd);
  	break;
  	
  	case 10:
  		printf("usted selecciono %i\n",opcion);
  		puntoDiez(fd);
  	break;
  	
  	case 0:
  	break;
  	
  	default:
  		printf("Opcion incorrecta, debe escojer entre 1 y 10 o cero para salir\n");
  	break;
  	
  
  }
  
  } while(opcion!=0);  
  /*
  
    //primerPunto(fd);
    //segundoPunto(fd);
    //tercerPunto(fd);
    //cuartoPunto(fd);
    //quintoPunto(fd);
    //sextoPunto(fd);
    //septimoPunto(fd);
    //octavoPunto(fd);
    //novenoPunto(fd);
   puntoDiez(fd);

   

    close (fd);
    return 0;*/
}








void primerPunto(int fd) {
    char ruta[100];
    printf("1. Ingrese la ruta del archivo que será impreso de manera inversa.\n");
    scanf("%s", ruta);

    FILE * fp;
    char * linea = NULL;
    size_t len = 0;
    ssize_t lectura;

    fp = fopen(ruta, "r");
    if (fp == NULL) {
        perror("No se pudo abrir el archivo");
    }

    printf("Contenido del archivo :\n");
    while ((lectura = getline(&linea, &len, fp)) != -1) {
        printf("%s", linea);
        write_message(fd, BRIDGE_W_S, linea);
    }
    printf("--Archivo Inverso:\n");
    read_all_messages_stack(fd);
    printf("--Fin del Archivo Inverso--\n");

    fclose(fp);
    if (linea)
        free(linea);
}


void tercerPunto(int fd){
    char ruta[100];
    printf("3. Ingrese la ruta del archivo para verificar parentesis y corchetes\n");
    scanf("%s", ruta);

    FILE *punteroArchivo;
    char ch;
    punteroArchivo = fopen(ruta, "r");
    if (punteroArchivo == NULL){
        printf("No se pudo abrir el archivo\n");
        return;
    } else {
        read_all_messages(fd);
        while ((ch = fgetc(punteroArchivo)) != EOF){
            if(ch == '(' || ch == '{'){
                char msgIn[2] = {ch, '\0'};
                write_message(fd, BRIDGE_W_S, msgIn);
            }
            else if(ch == ')'){
                if(send_empty_command(fd, BRIDGE_STATE_S) > 0){
                    char msgOut[2];
                    read_message(fd, BRIDGE_R_S, msgOut);
                    if(msgOut[0] != '('){
                    	printf("El ) encontrado no tiene ( de apertura\n");
                        break;
                    }
                } else {
                  printf("El ) encontrado no tiene ( de apertura y la cola ya esta vacia\n");
                  break;
                }
            } else if(ch == '}'){
                if(send_empty_command(fd, BRIDGE_STATE_S) > 0){
                    char msgOut[2];
                    read_message(fd, BRIDGE_R_S, msgOut);
                    if(msgOut[0] != '{'){
                        printf("El } encontrado no tiene { de apertura\n");
                        break;
                    }
                } else {
                  printf("El } encontrado no tiene { de apertura y la cola ya esta vacia\n");
                  break;
                }
            }
        }
        if(send_empty_command(fd, BRIDGE_STATE_S) > 0){
            printf("Quedaron elementos sin pareja\n");
        } else {
            printf("Archivo OK de () y {}\n");
        }
    }
    send_empty_command(fd, BRIDGE_DESTROY_S);
    fclose(punteroArchivo);
}



void cuartoPunto(int fd){
    printf("\n4. Cola de varios niveles");
    printf("\nAgreguemos algunos elementos\n");
    char ans[100]; // no empieces con 0 por favor
    int x = 0;
    while(strcmp(ans, "EXIT") != 0){
        printf("\nEscriba la palabra que desea agregar a la cola o EXIT en mayusculas para salir.\n");
        scanf("%s", ans);
        printf("\nInput: %s\n", ans);
        if(strcmp(ans, "EXIT") == 0){
            break;
        }
        printf("\n¿Cuál es la prioridad? Escriba el número del 1 al 3 para elegir uno:\n");
        printf(" 1. Alta prioridad.\n 2. Media prioridad.\n 3. baja prioridad.\n ");
        scanf("%d", &x);
        switch(x){
            case 1:
                write_message(fd, BRIDGE_W_HIGH_PRIOR_Q, ans);
                printf("\n%s Elemento agregado con éxito a la cola de alta prioridad.\n", &ans[0]);
                break;
            case 2:
                write_message(fd, BRIDGE_W_MIDDLE_PRIOR_Q, ans);
                printf("\n%s Elemento agregado con éxito a la cola de media prioridad.", &ans[0]);
                break;
            case 3:
                write_message(fd, BRIDGE_W_LOW_PRIOR_Q, ans);
                printf("\n%s Elemento agregado con éxito a la cola de baja prioridad.", &ans[0]);
                break;
        }
    }
    printf("\nLos artículos impresos según su prioridad\n");
    read_all_messages_colas(fd);
  
}


void quintopunto(int fd){
    printf("\n5. Destruir lista o pila.");
    int ans;
    printf("\nEscriba 1 para destruir la lista o 2 para destruir la pila.\n");
    scanf("%d", &ans);
    if(ans == 1){
        send_empty_command(fd, BRIDGE_DESTROY_L);
        read_all_messages_list(fd);
        printf("\nDone\n");
    } else if (ans == 2){
        send_empty_command(fd, BRIDGE_DESTROY_S);
        read_all_messages_stack(fd);
        printf("\nDone\n");
    } else {
        printf("\nERROR\n");
    }
}

void sextopunto(int fd)
{
    printf("\n6. Invertir Lista.");
    printf("\nAgreguemos algunos elementos\n");
    char ans[100]; // Just don't init with 0 plz
    while(strcmp(ans, "EXIT") != 0){
        printf("\nEscriba la palabra que desea agregar a la lista o EXIT en mayusculas para salir.\n");
        scanf("%s", ans);
        printf("\nInput: %s\n", ans);
        if(strcmp(ans, "EXIT") != 0){
            // CREATE LIST ITEMS LOGIC
            strcat(ans,"\n");
            write_message(fd, BRIDGE_W_L, ans);
        }
    }
    printf("\nlista invertida:\n");
    send_empty_command(fd, BRIDGE_INVERT_L);
    read_all_messages_list(fd);
}


void septimopunto(int fd)
{
    printf("\n7. Concatenar Lista.");
    printf("\nAgreguemos algunos elementos a la primera lista.\n");
    char ans[100]; // Just don't init with 0 plz
    while(strcmp(ans, "EXIT") != 0){
        printf("\nEscriba la palabra que desea agregar a la lista o EXIT en mayusculas para salir.\n");
        scanf("%s", ans);
        printf("\nInput: %s\n", ans);
        if(strcmp(ans, "EXIT") != 0){
            // CREATE LIST ITEMS LOGIC
            strcat(ans,"\n");
            write_message(fd, BRIDGE_W_L, ans);
        }
    }
    printf("\nAgreguemos algunos elementos a la segunda lista.\n");
    ans[0] = 'n';
    while(strcmp(ans, "EXIT") != 0){
        printf("\nEscriba la palabra que desea agregar a la lista o EXIT en mayusculas para salir.\n");
        scanf("%s", ans);
        printf("\nInput: %s\n", ans);
        if(strcmp(ans, "EXIT") != 0){
            // CREATE LIST ITEMS LOGIC
            strcat(ans,"\n");
            write_message(fd, BRIDGE_W_HIGH_PRIOR_Q, ans);
        }
    }
    printf("\nlistas concatenadas:\n");
    send_empty_command(fd, BRIDGE_CONCAT_L);
    read_all_messages_list(fd);
}

void octavopunto(int fd){
    printf("\n8. Rotar lista a la derecha.");
    printf("\nAgreguemos algunos elementos a la primera lista.\n");
    char ans[100]; // Just don't init with 0 plz
    while(strcmp(ans, "EXIT") != 0){
        printf("\nEscriba la palabra que desea agregar a la lista o EXIT en mayusculas para salir.\n");
        scanf("%s", ans);
        printf("\nInput: %s\n", ans);
        if(strcmp(ans, "EXIT") != 0){
            // CREATE LIST ITEMS LOGIC
            strcat(ans,"\n");
            write_message(fd, BRIDGE_W_L, ans);
        }
    }
    printf("lista rotada\n");
    send_empty_command(fd, BRIDGE_ROTATE_L);
    read_all_messages_list(fd);
}


void novenopunto(int fd){
    printf("\n9. Eliminar duplicados en la lista.");
    printf("\nAgreguemos algunos elementos a la primera lista.\n");
    char ans[100]; // Just don't init with 0 plz
    while(strcmp(ans, "EXIT") != 0){
        printf("\nEscriba la palabra que desea agregar a la lista o EXIT en mayusculas para salir.\n");
        scanf("%s", ans);
        printf("\nInput: %s\n", ans);
        if(strcmp(ans, "EXIT") != 0){
            strcat(ans,"\n");
            write_message(fd, BRIDGE_CLEAN_L, ans);
        }
    }
    printf("\nlista limpia:\n");
    read_all_messages_list(fd);
}


void puntoDiez(int fd){
    printf("\n10. Devuelve el mayor valor de la lista.");
    printf("\nAgreguemos algunos elementos a la primera lista.\n");
    char ans[100]; // Just don't init with 0 plz
    while(strcmp(ans, "EXIT") != 0){
        printf("\nEscriba la palabra que desea agregar a la lista o EXIT en mayusculas para salir.\n");
        scanf("%s", ans);
        printf("\nInput: %s\n", ans);
        if(strcmp(ans, "EXIT") != 0){
            write_message(fd, BRIDGE_W_L, ans);
        }
    }
    char  major[100];
    read_message(fd, BRIDGE_R_L, major);
   
    do{
        char input[100];
        read_message(fd, BRIDGE_R_L, input);
        
        if(strcmp(input,major) > 0)
        {

            memcpy(major, input,strlen(input)+1);

        }
    }while(send_empty_command(fd, BRIDGE_STATE_L)!=0);
    printf("\nEl mayor valor es: %s\n", major);
}
