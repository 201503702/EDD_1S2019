// [EDD]FINAL_201503702_AVL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

class AVL;

class Nodo {
public:
	Nodo(const int dat, Nodo *pad = NULL, Nodo *izq = NULL, Nodo *der = NULL) :
		dato(dat), padre(pad), izquierdo(izq), derecho(der), FE(0) {}
	int dato;
	int FE;
	Nodo *izquierdo;
	Nodo *derecho;
	Nodo *padre;
	friend class AVL;
};

class AVL {
private:
	enum { IZQUIERDO, DERECHO };
	Nodo *raiz;
	Nodo *actual;
	int contador;
	int altura;

public:
	AVL() : raiz(NULL), actual(NULL) {}
	~AVL() { Podar(raiz); }
	void Insertar(const int dat);
	void Borrar(const int dat);
	bool Buscar(const int dat);
	bool Vacio(Nodo *r) { return r == NULL; }
	bool EsHoja(Nodo *r) { return !r->derecho && !r->izquierdo; }
	const int NumeroNodos();
	const int AlturaArbol();
	int Altura(const int dat);
	int &ValorActual() { return actual->dato; }
	void Raiz() { actual = raiz; }
	void InOrden(void(*func)(int&, int), Nodo *nodo = NULL, bool r = true);
	void PreOrden(void(*func)(int&, int), Nodo *nodo = NULL, bool r = true);
	void PostOrden(void(*func)(int&, int), Nodo *nodo = NULL, bool r = true);
	void graficarArbol();
	int* linealizarHijos(int* lista[], Nodo *nodoAux, int pos);
	int* AVL::linealizar(int* lista[], Nodo* raiz);
private:
	void Equilibrar(Nodo *nodo, int, bool);
	void RSI(Nodo* nodo);
	void RSD(Nodo* nodo);
	void RDI(Nodo* nodo);
	void RDD(Nodo* nodo);
	void Podar(Nodo* &);
	void auxContador(Nodo*);
	void auxAltura(Nodo*, int);
	void generarArbol(FILE **archivoGrafico);
	void generarArbolEnlaces(FILE **archivoGrafico, Nodo *nodoAux);
	void generarArbolLabel(FILE **archivoGrafico, Nodo *nodoAux);
};

void AVL::Podar(Nodo* &nodo)
{
	if (nodo) {
		Podar(nodo->izquierdo);
		Podar(nodo->derecho);
		delete nodo;
		nodo = NULL;
	}
}

void AVL::Insertar(const int dat)
{
	Nodo *padre = NULL;

	cout << "Insertar: " << dat << endl;
	actual = raiz;

	while (!Vacio(actual) && dat != actual->dato) {
		padre = actual;
		if (dat > actual->dato) actual = actual->derecho;
		else if (dat < actual->dato) actual = actual->izquierdo;
	}

	if (!Vacio(actual)) return;

	if (Vacio(padre)) raiz = new Nodo(dat);

	else if (dat < padre->dato) {
		padre->izquierdo = new Nodo(dat, padre);
		Equilibrar(padre, IZQUIERDO, true);
	}
	else if (dat > padre->dato) {
		padre->derecho = new Nodo(dat, padre);
		Equilibrar(padre, DERECHO, true);
	}
}

void AVL::Equilibrar(Nodo *nodo, int rama, bool nuevo)
{
	bool salir = false;

	while (nodo && !salir) {
		if (nuevo)
			if (rama == IZQUIERDO) nodo->FE--;
			else                  nodo->FE++;
		else
			if (rama == IZQUIERDO) nodo->FE++;
			else                  nodo->FE--;
			if (nodo->FE == 0) salir = true;
			else if (nodo->FE == -2) { 
				if (nodo->izquierdo->FE == 1) RDD(nodo);
				else RSD(nodo);
				salir = true;
			}
			else if (nodo->FE == 2) { 
				if (nodo->derecho->FE == -1) RDI(nodo);
				else RSI(nodo);                        
				salir = true;
			}
			if (nodo->padre)
				if (nodo->padre->derecho == nodo) rama = DERECHO; else rama = IZQUIERDO;
			nodo = nodo->padre; 
	}
}

void AVL::RDD(Nodo* nodo)
{
	cout << "RDD" << endl;
	Nodo *Padre = nodo->padre;
	Nodo *P = nodo;
	Nodo *Q = P->izquierdo;
	Nodo *R = Q->derecho;
	Nodo *B = R->izquierdo;
	Nodo *C = R->derecho;

	if (Padre)
		if (Padre->derecho == nodo) Padre->derecho = R;
		else Padre->izquierdo = R;
	else raiz = R;

	Q->derecho = B;
	P->izquierdo = C;
	R->izquierdo = Q;
	R->derecho = P;

	R->padre = Padre;
	P->padre = Q->padre = R;
	if (B) B->padre = Q;
	if (C) C->padre = P;

	switch (R->FE) {
		case -1: Q->FE = 0; P->FE = 1; break;
		case 0:  Q->FE = 0; P->FE = 0; break;
		case 1:  Q->FE = -1; P->FE = 0; break;
	}
	R->FE = 0;
}

void AVL::RDI(Nodo* nodo)
{
	cout << "RDI" << endl;
	Nodo *Padre = nodo->padre;
	Nodo *P = nodo;
	Nodo *Q = P->derecho;
	Nodo *R = Q->izquierdo;
	Nodo *B = R->izquierdo;
	Nodo *C = R->derecho;

	if (Padre)
		if (Padre->derecho == nodo) Padre->derecho = R;
		else Padre->izquierdo = R;
	else raiz = R;

	P->derecho = B;
	Q->izquierdo = C;
	R->izquierdo = P;
	R->derecho = Q;

	R->padre = Padre;
	P->padre = Q->padre = R;
	if (B) B->padre = P;
	if (C) C->padre = Q;

	switch (R->FE) {
		case -1: P->FE = 0; Q->FE = 1; break;
		case 0:  P->FE = 0; Q->FE = 0; break;
		case 1:  P->FE = -1; Q->FE = 0; break;
	}
	R->FE = 0;
}

void AVL::RSD(Nodo* nodo)
{
	cout << "RSD" << endl;
	Nodo *Padre = nodo->padre;
	Nodo *P = nodo;
	Nodo *Q = P->izquierdo;
	Nodo *B = Q->derecho;

	if (Padre)
		if (Padre->derecho == P) Padre->derecho = Q;
		else Padre->izquierdo = Q;
	else raiz = Q;

	P->izquierdo = B;
	Q->derecho = P;

	P->padre = Q;
	if (B) B->padre = P;
	Q->padre = Padre;

	P->FE = 0;
	Q->FE = 0;
}

void AVL::RSI(Nodo* nodo)
{
	cout << "RSI" << endl;
	Nodo *Padre = nodo->padre;
	Nodo *P = nodo;
	Nodo *Q = P->derecho;
	Nodo *B = Q->izquierdo;

	if (Padre)
		if (Padre->derecho == P) Padre->derecho = Q;
		else Padre->izquierdo = Q;
	else raiz = Q;

	P->derecho = B;
	Q->izquierdo = P;

	P->padre = Q;
	if (B) B->padre = P;
	Q->padre = Padre;

	P->FE = 0;
	Q->FE = 0;
}

void AVL::Borrar(const int dat)
{
	Nodo *padre = NULL;
	Nodo *nodo;
	int aux;

	actual = raiz;
	while (!Vacio(actual)) {
		if (dat == actual->dato) { 
			if (EsHoja(actual)) { 
				if (padre) 
					if (padre->derecho == actual) padre->derecho = NULL;
					else if (padre->izquierdo == actual) padre->izquierdo = NULL;
					delete actual; 
					actual = NULL;
					if ((padre->derecho == actual && padre->FE == 1) ||
						(padre->izquierdo == actual && padre->FE == -1)) {
						padre->FE = 0;
						actual = padre;
						padre = actual->padre;
					}
					if (padre)
						if (padre->derecho == actual) Equilibrar(padre, DERECHO, false);
						else                         Equilibrar(padre, IZQUIERDO, false);
						return;
			}
			else { 
				padre = actual;
				if (actual->derecho) {
					nodo = actual->derecho;
					while (nodo->izquierdo) {
						padre = nodo;
						nodo = nodo->izquierdo;
					}
				}
				else {
					nodo = actual->izquierdo;
					while (nodo->derecho) {
						padre = nodo;
						nodo = nodo->derecho;
					}
				}
				aux = actual->dato;
				actual->dato = nodo->dato;
				nodo->dato = aux;
				actual = nodo;
			}
		}
		else { 
			padre = actual;
			if (dat > actual->dato) actual = actual->derecho;
			else if (dat < actual->dato) actual = actual->izquierdo;
		}
	}
}

void AVL::InOrden(void(*func)(int&, int), Nodo *nodo, bool r)
{
	if (r) nodo = raiz;
	if (nodo->izquierdo) InOrden(func, nodo->izquierdo, false);
	func(nodo->dato, nodo->FE);
	if (nodo->derecho) InOrden(func, nodo->derecho, false);
}

void AVL::PreOrden(void(*func)(int&, int), Nodo *nodo, bool r)
{
	if (r) nodo = raiz;
	func(nodo->dato, nodo->FE);
	if (nodo->izquierdo) PreOrden(func, nodo->izquierdo, false);
	if (nodo->derecho) PreOrden(func, nodo->derecho, false);
}

void AVL::PostOrden(void(*func)(int&, int), Nodo *nodo, bool r)
{
	if (r) nodo = raiz;
	if (nodo->izquierdo) PostOrden(func, nodo->izquierdo, false);
	if (nodo->derecho) PostOrden(func, nodo->derecho, false);
	func(nodo->dato, nodo->FE);
}

bool AVL::Buscar(const int dat)
{
	actual = raiz;

	while (!Vacio(actual)) {
		if (dat == actual->dato) return true;
		else if (dat > actual->dato) actual = actual->derecho;
		else if (dat < actual->dato) actual = actual->izquierdo;
	}
	return false;
}

int AVL::Altura(const int dat)
{
	int altura = 0;
	actual = raiz;

	while (!Vacio(actual)) {
		if (dat == actual->dato) return altura;
		else {
			altura++; 
			if (dat > actual->dato) actual = actual->derecho;
			else if (dat < actual->dato) actual = actual->izquierdo;
		}
	}
	return -1;
}

const int AVL::NumeroNodos()
{
	contador = 0;

	auxContador(raiz);
	return contador;
}

void AVL::auxContador(Nodo *nodo)
{
	contador++;  
	if (nodo->izquierdo) auxContador(nodo->izquierdo);
	if (nodo->derecho)   auxContador(nodo->derecho);
}

const int AVL::AlturaArbol()
{
	altura = 0;

	auxAltura(raiz, 0);
	return altura;
}

void AVL::auxAltura(Nodo *nodo, int a)
{
	if (nodo->izquierdo) auxAltura(nodo->izquierdo, a + 1);
	if (nodo->derecho)   auxAltura(nodo->derecho, a + 1);
	if (EsHoja(nodo) && a > altura) altura = a;
}

void AVL::graficarArbol() {
	FILE *archivoGrafico;
	archivoGrafico = fopen("AVL.dot", "w+");

	if (archivoGrafico) {
		fprintf(archivoGrafico, "%s", "digraph AVL{ \n");
		fprintf(archivoGrafico, "%s", "00000000[label=\"Arbol_Avl\", fillcolor=\"lightblue\", style =\"filled\", shape=\"circle\"]; \n");
		generarArbol(&archivoGrafico);
		if (!Vacio(raiz)) {
			fprintf(archivoGrafico, "\"00000000\"->\"%p\"; \n", (void*)raiz);
		}
		fprintf(archivoGrafico, "%s", "\n overlap=false \n fontsize=15; \n }");
		fclose(archivoGrafico);

		system("dot AVL.dot -Tpng -o AVL.png");
	}
	else {
		printf("%s\n", "Error abriendo el archivo :(");
	}
}

void AVL::generarArbolLabel(FILE **archivoGrafico, Nodo *nodoAux) {
	//Generando Filas :)
	//Genera El label de cada nodo
	if (nodoAux != NULL) {
		fprintf((*archivoGrafico), "\"%p\"[label=\"Dato: %d\", fillcolor=\"lightblue\", style =\"filled\", shape=\"circle\"]; \n", (void*)nodoAux, nodoAux->dato);
		if (nodoAux->izquierdo) {
			generarArbolLabel(archivoGrafico, nodoAux->izquierdo);
		}
		if (nodoAux->derecho) {
			generarArbolLabel(archivoGrafico, nodoAux->derecho);
		}
	}
}

void AVL::generarArbolEnlaces(FILE **archivoGrafico, Nodo *nodoAux) {

	Nodo *act = nodoAux;

	if (act->izquierdo) {
		fprintf((*archivoGrafico), "\"%p\"", (void*)act);
		fprintf((*archivoGrafico), "->\"%p\"", (void*)act->izquierdo);
		generarArbolEnlaces(archivoGrafico, act->izquierdo);
	}

	if (act->derecho) {
		fprintf((*archivoGrafico), "\"%p\"", (void*)act);
		fprintf((*archivoGrafico), "->\"%p\"", (void*)act->derecho);
		generarArbolEnlaces(archivoGrafico, act->derecho);
	}
}

void AVL::generarArbol(FILE **archivoGrafico) {
	Nodo* aux = raiz;
	generarArbolLabel(archivoGrafico, aux);
	aux = raiz;
	generarArbolEnlaces(archivoGrafico, aux);
}

int* AVL::linealizarHijos(int* lista[], Nodo *nodoAux, int pos) 
{
	if (nodoAux != NULL) {
		pos = pos + 1;
		*lista[pos] = nodoAux->dato;
		//lista.push_back(nodoAux->getDato());
		linealizarHijos(lista, nodoAux->izquierdo, pos);
		linealizarHijos(lista, nodoAux->derecho, pos);
	}
	return *lista;
}

int* AVL::linealizar(int* lista[], Nodo* raiz) {
	Nodo* tmp = raiz;
	linealizarHijos(lista, tmp, 0);
	return *lista;
}

void Mostrar(int &d, int FE)
{
	cout << d << "(" << "FE: " << FE << "),";
}

int main()
{
	AVL ArbolInt;

	ArbolInt.Insertar(5);
	ArbolInt.Insertar(10);
	ArbolInt.Insertar(96);
	ArbolInt.Insertar(30);
	ArbolInt.Insertar(2);
	ArbolInt.Insertar(63);
	ArbolInt.Insertar(85);
	ArbolInt.Insertar(47);
	ArbolInt.Insertar(20);
	ArbolInt.Insertar(3);
	ArbolInt.Insertar(4);
	ArbolInt.Insertar(19);

	ArbolInt.graficarArbol();

	cin.get();
	return 0;
}