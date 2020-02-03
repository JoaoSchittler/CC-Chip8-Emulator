#ifndef FILA_H
#define FILA_H

typedef struct no{
	struct no* next;
	void* data;	
}No;
typedef struct fila{
	No* begin;
	int size;
}Fila;

Fila* fila_cria();
void  fila_deleta(Fila* f);
void  fila_insere(Fila* f,void* data);
void* fila_peek(Fila* f,int index);
void* fila_remove(Fila* f);
#endif
