#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "mitr.h"

/**Autores: José María Amusquívar Poppe y Eduardo Maldonado Fernández
  *Programa que reemplaza y elimina caracteres de un fichero sobre otro.
  *01/04/19*/

//Variables usadas en la ejecución del programa.
char* mitr = "mitr"; //Cadena de comparación para "mitr".
int fichUno, fichDos; //Identificadores de cada uno de los dos ficheros disponibles.
char test = 'x'; //Identificador de opción, puede valer tanto 'd', 's' o 'x'.

//Variables que recogen los argumentos pasados por el main.
char* cad1;
char* cad2;
char* fichero1;
char* fichero2;

//Método que abre un fichero en modo lectura, habilita los permisos de RW_R__R__. Devuelve un identificador. 
int abrir(char* nombre){
	int abierto = open(nombre, O_RDONLY, 0644);
	if(abierto == -1){
		perror("Fallo al abrir el fichero\n");
		exit(EXIT_FAILURE);
	}else{
		char aux = abierto + '0';
		write(STDOUT_FILENO, "Se ha abierto el fichero correctamente, id: ", 45);
		write(STDOUT_FILENO, &aux, sizeof(aux));
		write(STDOUT_FILENO, "\n", 1);
	}
	return (abierto);
}

//Método para cerrar un fichero. Devuelve 0 si lo cierra, o -1 si ha habido un problema al cerrar.
int cerrar(int fd){
	int cerrado = close(fd);
	if(cerrado == -1){
		perror("Fallo al cerrar el fichero\n");
		exit(EXIT_FAILURE);	
	}
	return (cerrado);
}

//Método para leer cada caracter del fichero origen. Devuelve el caracter leído actualmente.
char leer(int fd){
char car;
int nbytes;
nbytes=read(fd,&car,sizeof(char));
	if(nbytes == -1){
		perror("Error en la lectura del fichero..\n");
		exit(EXIT_FAILURE);
	}
	if(nbytes == 0) car = '\0';
return car;
}

//Método para crear un nuevo fichero sobre el que se escribirá. Devuelve un identificador.
int crear(char* creacion){
	int creado = creat(creacion, O_WRONLY);
	
	if(creado == -1){
		perror("Error en la creacion del fichero destino..\n");
		exit(EXIT_FAILURE);
	}else{
		char aux = creado + '0';
		write(STDOUT_FILENO, "Se ha creado correctamente el fichero destino, id: ", 52);	
		write(STDOUT_FILENO, &aux, sizeof(aux));
		write(STDOUT_FILENO, "\n", 1);
	}
	return (creado);
}

//Método que sobreescribe un caracter sobre otro pasado por parámetro, se habilita cuando no se pasa opción.
void sobreescribir(){
char byteLeido;
int avanza = 0;
	while((byteLeido = leer(fichUno)) != '\0'){
		while(*cad1 != '\0'){
			if(byteLeido == *cad1){
				cad2 += avanza;
				byteLeido = *cad2;
				cad2 -= avanza;
			}
			avanza++; cad1++;
		}
		write(fichDos, &byteLeido, 1);
		cad1 -= avanza;
		avanza = 0;
	}
}

//Método que elimina todos las repeticiones de un mismo caracter, se habilita con la opción -d. 
void borrarCaracteres(){
char byteLeido;
int i = 0;
int avanza = 0;
	while((byteLeido = leer(fichUno)) != '\0'){
		while(*cad2 != '\0'){
			if(byteLeido == *cad2){
			/*Para poner un espacio en lugar de poner un vacío, byteLeido = ' ';*/
				i = -1;
				break;
			}
			avanza++; cad2++;
		}
		if(i == 0){
			write(fichDos, &byteLeido, 1);
		}else{
			i = 0;
		}
		cad2 -= avanza;
		avanza = 0;
	}
}

//Método que elimina las repeticiones superfluas, se utiliza cuando se elige la opción -s.
void borrarRepes(){
char byteLeido;
char anterior = '\0';
int avanza = 0;
	while((byteLeido = leer(fichUno)) != '\0'){
		while(*cad2 != '\0'){
			if(byteLeido == *cad2 && byteLeido != anterior){
				anterior = byteLeido;	
				write(fichDos, &byteLeido, 1);			
				break;
			}
			avanza++; cad2++;
		}
		if(byteLeido != anterior){
			write(fichDos, &byteLeido, 1);
			anterior = '\0';
		}
		cad2 -= avanza;
		avanza = 0;
	}	
}

//Método auxiliar que comprueba si dos cadenas son iguales. Devuelve 0 si son iguales, -1 si no.
int comprueba(char* aux1, char* aux2){
	while(*aux1 != '\0' && *aux1 == *aux2){
		aux1++; aux2++;
	}
	if(*aux2 != '\0'){
		return (-1);
	}
	return (0);
}

main(int argc, char* argv[]){

//Se comprueba que se han introducido 6 argumentos exactamente.
if(argc <= 1){
	perror("No ha introducido argumentos..\n");
	exit(EXIT_FAILURE);
}else if(argc > 1 && argc <= 5){
	perror("Sintaxis erronea. No ha introducido suficientes argumentos..\n");
	exit(EXIT_FAILURE);
}else if(argc > 6){
	perror("Sintaxis erronea. Ha introducido demasiados argumentos..\n");
	exit(EXIT_FAILURE);	
}

//Se comprueba que se ha introducido correctamente "mitr".
if(comprueba(mitr, argv[1]) == -1){
	perror("Sintaxis erronea..\n");
	exit(EXIT_FAILURE);
}

//Se asigna las cadenas a dos punteros distintos, sin importar cuales sean.
cad1 = argv[2];
cad2 = argv[3]; 

//Se analiza la opción requerida con getopt, tanto "-d" como "-s".
int opt;
int dflag = 0;
int sflag = 0;
while((opt = getopt(argc, argv, "ds")) != -1){
	//Se almacena en "test" la opvión escogida para anlizarla más adelante.
	switch(opt){
		case 'd':
			if(dflag){
				write(STDOUT_FILENO, "Usage: mitr -d <temp> | mitr -s <temp>..\n", 41);
				exit(EXIT_FAILURE);
			}else{				
				dflag++;
				sflag++;
				test = 'd'; 
				break;
			}
		case 's':
			if(sflag){
				write(STDOUT_FILENO, "Usage: mitr -d <temp> | mitr -s <temp>..\n", 41);
				exit(EXIT_FAILURE);
			}else{
				sflag++;
				dflag++;
				test = 's';
				break;
			}
		default :
			write(STDOUT_FILENO, "Pruebe a cambiar el primer '-' a otra posicion...\n", 51);
			exit(EXIT_FAILURE);
	}
}

/**Si "opt" es distinto de -1 significa que no entró a getopt(), es decir, no ha seleccionado 
 * una opción, por tanto, se asume que se aplicará la sustitución de caracteres.*/
if(test == 'x'){
	while(*argv[2] != '\0' && *argv[3] != '\0'){ //Se comprueba longitudes iguales.
		argv[2]++; argv[3]++;
	}
	if(*argv[2] != '\0' || *argv[3] != '\0'){
		perror("Los cadenas de caracteres tienen distintas longitudes..\n");
		exit(EXIT_FAILURE);
	}
}

//Se asigna a dos punteros, las rutas o nombres de los ficheros.
fichero1 = argv[4];
fichero2 = argv[5];

//Si se elige el mismo nombre o ruta de destino y origen, se aplica una sobreescritura.
int comp = comprueba(argv[4], argv[5]);
if(comp == 0){
	//En este caso, se renombra el fichero origen, y se reasigna su puntero.
	rename(fichero1, ".txt");
	fichero1 = ".txt";
}

/*Si el fichero destino ya existe, deberemos sobreescribirlo, pues lo que hacemos es eliminarlo, y 
 *posteriormente se crea el nuevo fichero destino.*/
if(access(argv[5], F_OK ) != -1){ //La función access() devuelve un -1 si ha podido encontrar el fichero.	
	remove(argv[5]);
}

//Por tanto, sin importar si se sobreescribe o no, se obtiene dos ficheros de distinto nombre.
fichUno = abrir(fichero1);
fichDos = crear(fichero2);


//Obtención de los mismos permisos del fichero origen para el fichero destino.
/**********Esta parte es un añadido nuestro, copia los permisos de origen a destino.******/
struct stat buffer;
if (stat(fichero1, &buffer)){
	perror("Fallo al obtener los datos del fichero origen..\n");
	exit(EXIT_FAILURE);	
}else{
    if (chmod(fichero2, buffer.st_mode & 07777)){
        perror("Fallo al definir los permisos del fichero destino..\n");
	exit(EXIT_FAILURE);
    }
}
/*Si se desea, se puede quitar esta parte, pero se deberá modificar el método crear(), 
 *sustituyendo int creado = creat(creacion, O_WRONLY); (línea 63), por la instrucción
 *int creado = open(creacion, O_CREAT | O_WRONLY, 0644);
 *Con esta línea se asegura que se crea o se abre en modo escritura con unos permisos 
 *predeterminados de 0644(RW_R__R__)
/*****************************************************************************************/

//Se analiza la opción escogida.
if(test == 'd'){
	borrarCaracteres();
}else if(test == 's'){
	borrarRepes();
}else{
	sobreescribir();
}

cerrar(fichDos);
cerrar(fichUno);

if(comp == 0) remove(fichero1); //Si se ha sobreescrito, eliminamos el fichero origen.

exit(EXIT_SUCCESS);
}
