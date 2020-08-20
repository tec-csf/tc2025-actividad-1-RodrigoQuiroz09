/**
 * Autor - Rodrigo Quiroz Reyes
 * Fecha - 20/08/2020
 * Actividad individual 1: Aritmética de apuntadores
 * Ejercicio - Control de pacientes ingresados en un hospital
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
    char *nombre; 
    char *apellido;
    int edad;
    char *telefono;
} Paciente; //Datos básicos del paciente que nos pide el programa

typedef struct{
    int id;
    int estado;
  
    Paciente *paciente;

} Cama; //Estructura que van a contener los pacientes (Apuntador a pacientes para luego usar malloc para generarlos)

Cama* lista_camas; //Apuntador global para array de camas (Se puso de manera global porque el paso por referencia en métodos me causaba un realloc mal hecho)

int no_init_camas = 5; //No. inicial de camas con los que empieza el hospital


typedef void (*opcion_t) (); //Apuntadores a funciones para simplificar el menu

void tomar_datos_paciente();
void ver_camas();
void dar_alta();
void lista_pacientes();
void disponibilidad();
void incrementar_camas();
void liberar_memoria();
void init_hospital();

int main(){

    
    opcion_t * menu = (opcion_t *) malloc( 6 * sizeof(opcion_t));
    
    *menu = tomar_datos_paciente;
    *(menu+1) = ver_camas;
    *(menu+2) = dar_alta;
    *(menu+3) = lista_pacientes;
    *(menu+4) = disponibilidad;
    *(menu+5) = incrementar_camas;

    lista_camas = (Cama*)calloc(no_init_camas, sizeof(Cama)); //Un memory allocation para formar un array de camas del hospital
    
    init_hospital();

    int opcion=1; //Variable para escoger las opciones del menu
    while (opcion)
    {
    printf("Bienvenido al registro de pacientes del Hospital\n");
    printf("1.- Ingresar paciente\n");
    printf("2.- Ver registro de camas\n");
    printf("3.- Dar de alta a un paciente\n");
    printf("4.- Lista de pacientes\n");
    printf("5.- Camas disponibles\n");
    printf("6.- Incementar el no. de camas\n");
    printf("0.- EXIT\n");
    scanf("%i",&opcion);

    if(opcion>0 && opcion<7){
         (*(menu+opcion-1))();// Apuntador de funciones
    }
    else if(opcion!=0){
        printf("------------------------------------------------\n");
        continue;
    }else{
        break;
    }

    printf("------------------------------------------------\n");
    }



liberar_memoria(lista_camas);
free(lista_camas); //Liberar el espacio del array de las camas 
free(menu);//Liberar el espacio que ocupa el apuntador de funciones
    return 0;
}

/**
 * Inicializa las camas iniciales con las cuales 
 * el hospital empieza
 */
void init_hospital()
{
    for(int i = 0; i<no_init_camas; i++)
    {
        (lista_camas + i)->estado = 1;//ESTADO - 1 -> Desocupado ; ESTADO - 0 ->Ocupado
        (lista_camas + i)->id = i + 1;//Id para identificar la cama
    }
}
/**
 * Metodo para ver el estado de la cama y 
 * si esta ocupada ver el nombre del paciente
 */
void ver_camas(){
    printf("-------------------------------------------\n");
 
    int cama=0; //Integer para escoger el id de la cama
    printf("Inserte el no. de cama\n");
    scanf("%d",&cama); 

    if(cama<=no_init_camas){

        for(int i=0;i<no_init_camas;i++){ //Búsqueda lineal a través de todas las camas buscando el ida
            if((lista_camas + i)->id == cama){
                if((lista_camas + i)->estado){ //Condicion para indicar al usuario si esta disponible la cama u ocupada
                    printf("Cama disponible\n");
                    return;
                }
                else{
                    printf("El paciente %s esta en la cama %d\n", (lista_camas + i)->paciente->nombre,(lista_camas+i)->id);
                    return;
                }
            }
        }   
        
    }
    
    printf("El id insertado no existe en nuestro inventario\n");
}

/**
 * Metodo para liberar la memoria los apuntadores usados en
 * la estructura de Paciente
 * 
 * @param Paciente* se utiliza como param un apuntador
 * a los pacientes para liberar cada propiedad por 
 * separado
 */
void liberar_paciente(Paciente *paciente){
    free(paciente->nombre);
    free(paciente->apellido);
    free(paciente->telefono);
    
}

/**
 * Metodo para liberar la memoria mediante un loop
 * por todo el arreglo para luego liberar el 
 * apuntador de Paciente en la estructura Cama
 * 
 */
void liberar_memoria(){
Cama * aux;// Apuntador al inicio del arreglo de Cama
Cama *final=lista_camas+no_init_camas; //Apuntador al final del arreglo de Cama

    for(aux=lista_camas;aux<final;++aux){
        
        if(!aux->estado){
        liberar_paciente(aux->paciente);
        free(aux->paciente);
        }
    }   
}

/**
 * Metodo para insertar los datos del paciente a través
 * del input de consola. Cada variable se utiliza un malloc
 * para reservar memoria para después asignarla a su respectiva cama
 * disponible; en caso de no haber espacio se generan más camas de
 * manera automática
 * 
 */
void tomar_datos_paciente(){

    Paciente *nuevo =(Paciente*)malloc(sizeof(Paciente)); //Malloc para generar un nuevo paciente

    int i=0;
    printf("-------------------------------------------\n");
    nuevo->nombre=(char*)malloc((sizeof(char) * 25)); //Malloc para un nombre de tamaño 25
    printf("Ingrese nombre\n");
    scanf(" %[^\n]",nuevo->nombre);

    nuevo->apellido=(char*)malloc((sizeof(char) * 40)); //Malloc para un apellido de tamaño 40
    printf("Ingrese apellido\n");
    scanf(" %[^\n]",nuevo->apellido);

    printf("Ingrese edad\n");
    scanf("%d", &nuevo->edad);

    printf("Ingrese telefono\n"); 
    nuevo->telefono=(char*)malloc((sizeof(char) * 15));  //Malloc para un no. de telefono de tamaño 10
    scanf(" %[^\n]",nuevo->telefono);

    for(i=0;i<no_init_camas;i++){ //Búsqueda de una cama disponible
        if((lista_camas + i)->estado){
            (lista_camas + i)->paciente = nuevo;
            (lista_camas + i)->estado = 0;
            printf("El paciente %s se puso en la cama %d\n", (lista_camas + i)->paciente->nombre,(lista_camas+i)->id);
            return;
        }
        printf("%d",i);
    }
            incrementar_camas(lista_camas); //En caso de no encontrar una cama disponible genera más camas y se le asigna al nuevo paciente
            (lista_camas + i)->paciente = nuevo;
            (lista_camas + i)->estado = 0;
            printf("El paciente %s se puso en la cama %d\n", (lista_camas + i)->paciente->nombre,(lista_camas+i)->id);

}

/**
 * Metodo para liberar la memoria de un paciente y poner su cama 
 * en estado disponible. Se usa el método liberar paciente para
 * acceder a cada apuntador. En caso de insertar un id que no 
 * se encuentra en el inventario, se imprime un mensaje de error
 * y si la cama no contiene paciente el codigo sigue su curso
 * 
 */
void dar_alta(){

    int cama=0; //índice para la busqueda de la cama
    printf("Inserte el no. de cama\n");
    scanf("%d",&cama);

    if(cama<=no_init_camas){
        for(int i=0;i<no_init_camas;i++){
            if((lista_camas + i)->id == cama && !(lista_camas + i)->estado){
                printf("El paciente %s %s se dio de alta en el hospital\n", (lista_camas + i)->paciente->nombre,(lista_camas+i)->paciente->apellido);
                liberar_paciente((lista_camas + i)->paciente); //Método para liberar a los apuntadores de la estructura paciente
                free((lista_camas + i)->paciente);//Libera el apuntador de paciente
                (lista_camas + i)->estado =1;//Actualiza el estado de la cama
                return;
            }
        }   
    }

    printf("No hay paciente asignado a esa id de cama\n");
}

/**
 * Metodo para imprimir toda la lista de pacientes que se 
 * encuentran en el hospital junto con su respectiva cama
 * 
 */
void lista_pacientes(){

     for(int i=0;i<no_init_camas;i++){
            if(!((lista_camas + i)->estado)){
                printf("El paciente %s %s esta en la cama %d\n", (lista_camas + i)->paciente->nombre,(lista_camas + i)->paciente->apellido,(lista_camas+i)->id);
   
            }
        }   
}

/**
 * Metodo que muestra el no. disponible de las camas del hospital 
 */
void disponibilidad(){
    int disponible=0; //Contador de camas disponibles
    int no_disponible=0;//Contador de camas NO disponibles
     for(int i=0;i<no_init_camas;i++){
            if((lista_camas + i)->estado){
               ++disponible;
            }
            else{
                ++no_disponible;
            }
        }   
        printf("Se encuentram %d disponibles y %d no disponibles\n", disponible,no_disponible);
}

/**
 * Metodo para realizar un realloc a la variable de el array de camas
 * e inicializar las variables básicas de las camas. 
 * PD - Por como funciona el compilador se tuvo que crear una variable
 * auxiliar para asignar el realloc de manera correcta.
 */
void incrementar_camas(){

    int temp = no_init_camas; //Variable temporal para guardar el no. más reciente de camas en el hospital

    no_init_camas = no_init_camas + 5;//Se hace un incremento en 5 (Definición del problema) en la variable global para mantener un status del no. total de camas

    Cama *list = realloc(lista_camas, no_init_camas * sizeof(Cama)); //Variable auxiliar para realiza el realloc de las camas
    lista_camas=list;
    printf("Se ha incrementado 5 camas más \n");
    for(temp; temp<no_init_camas; temp++)
    {
        (lista_camas + temp)->estado = 1;
        (lista_camas + temp)->id = temp + 1;
    }
}

