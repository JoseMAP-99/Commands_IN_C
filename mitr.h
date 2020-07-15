/**Autores: Eduardo Maldonado Fernández y José María AMusquívar Poppe
  *Cabecera del programa mitr.c.
  *01/04/19.*/

//HEADER: 

#ifndef MITR_H_ 

#define MITR_H_    

int abrir(char* nombre);   

int cerrar(int fd);   

char leer(int fd);   

int crear(char* creacion);   

void sobreescribir();   

void borrarCaracteres();   

void borrarRepes();   

#endif 
