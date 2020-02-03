#include <stdlib.h>
#include "fila.h"

Fila* fila_cria()
{
	Fila* f = (Fila*)malloc(sizeof(Fila));
	f->begin = NULL;
	f->size = 0;
	return f;
}
void fila_delete_(No* p)
{
	if(p == NULL) return;
	fila_delete_(p->next);
	free(p->data);
	free(p);
}
void fila_deleta(Fila* f)
{
	fila_delete_(f->begin);
	free(f);
}
void fila_insere(Fila* f,void* data)
{
	No* no =(No*)malloc(sizeof(No));
	no->data = data;
	no->next = NULL;
	No* p = f->begin;
	if (p == NULL)
		f->begin = no;
	else{
		while (p->next!=NULL)
			p = p->next;
		p->next = no;
	}		
	f->size++;	
}
void* fila_peek(Fila* f,int index)
{
	No* p = f->begin;
	if (f->size <= index)
		return NULL;
	for(int i = 0; i < index; i++)
		p = p->next;
	return p->data;
}
void* fila_remove(Fila* f)
{
	No* p = f->begin;
	void* data = p->data;
	f->begin = p->next;
	
	f->size--;
	free(p);
	return data;
}
