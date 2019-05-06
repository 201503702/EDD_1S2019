// [EDD]FINAL_201503702.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

class node
{
public:
	int clave;
	node *izquierda, *derecha;
};

class ARBOLSPL {
public:
	int contador;
	int altura;
	int arreglo[];
	node* nuevoNodo(int clave);
	node *derechaRotate(node *x);
	node *izquierdaRotate(node *x);
	node *splay(node *root, int clave);
	node *insert(node *root, int k);
	void preOrder(node *root);
	bool EsHoja(node *r) { return !r->derecha && !r->izquierda; }
	void graficarArbol(node* root);
	void generarArbol(FILE **archivoGrafico, node* root);
	void generarArbolLabel(FILE **archivoGrafico, node *nodoAux);
	void generarArbolEnlaces(FILE **archivoGrafico, node *nodoAux);
	const int AlturaArbol(node *nodo);
	void auxAltura(node *nodo, int a);
	const int NumeroNodos(node* raiz);
	void auxContador(node *nodo);
};

/*Función auxiliar que asigna.
un nuevo nodo con la clave dada y
NULL punteros izquierda y derecha. */
node* ARBOLSPL::nuevoNodo(int clave)
{
	node* Node = new node();
	Node->clave = clave;
	Node->izquierda = Node->derecha = NULL;
	return (Node);
}

// Una función de utilidad para la derecha.
// rotar subárbol arraigado con y
// Ver el diagrama dado arriba.
node *ARBOLSPL::derechaRotate(node *x)
{
	node *y = x->izquierda;
	x->izquierda = y->derecha;
	y->derecha = x;
	return y;
}

// Una función de utilidad a la izquierda.
// rotar subárbol enraizado con x
// Ver el diagrama dado arriba.
node *ARBOLSPL::izquierdaRotate(node *x)
{
	node *y = x->derecha;
	x->derecha = y->izquierda;
	y->izquierda = x;
	return y;
}

// Esta función trae la clave a
// root si la clave está presente en el árbol.
// Si la clave no está presente, entonces
// trae el último elemento accedido a
// raíz. Esta función modifica la
// árbol y devuelve la nueva raíz
node *ARBOLSPL::splay(node *root, int clave)
{
	if (root == NULL || root->clave == clave)
		return root;

	if (root->clave > clave)
	{
		if (root->izquierda == NULL) return root;

		// Zig-Zig (izquierda izquierda) 
		if (root->izquierda->clave > clave)
		{
			root->izquierda->izquierda = splay(root->izquierda->izquierda, clave);

			root = derechaRotate(root);
		}
		else if (root->izquierda->clave < clave) // Zig-Zag (izquierda derecha) 
		{
			root->izquierda->derecha = splay(root->izquierda->derecha, clave);

			if (root->izquierda->derecha != NULL)
				root->izquierda = izquierdaRotate(root->izquierda);
		}

		return (root->izquierda == NULL) ? root : derechaRotate(root);
	}
	else
	{
		if (root->derecha == NULL) return root;

		// Zig-Zag (derecha izquierda) 
		if (root->derecha->clave > clave)
		{
			root->derecha->izquierda = splay(root->derecha->izquierda, clave);

			if (root->derecha->izquierda != NULL)
				root->derecha = derechaRotate(root->derecha);
		}
		else if (root->derecha->clave < clave)// Zag-Zag (derecha derecha) 
		{
			root->derecha->derecha = splay(root->derecha->derecha, clave);
			root = izquierdaRotate(root);
		}

		return (root->derecha == NULL) ? root : izquierdaRotate(root);
	}
}

node *ARBOLSPL::insert(node *root, int k)
{
	if (root == NULL) return nuevoNodo(k);

	root = splay(root, k);

	if (root->clave == k) return root;

	node *newnode = nuevoNodo(k);

	if (root->clave > k)
	{
		newnode->derecha = root;
		newnode->izquierda = root->izquierda;
		root->izquierda = NULL;
	}
	else
	{
		newnode->izquierda = root;
		newnode->derecha = root->derecha;
		root->derecha = NULL;
	}

	return newnode;
}

void ARBOLSPL::preOrder(node *root)
{
	if (root != NULL)
	{
		cout << root->clave << " ";
		preOrder(root->izquierda);
		preOrder(root->derecha);
	}
}

const int ARBOLSPL::AlturaArbol(node *nodo)
{
	altura = 0;

	auxAltura(nodo, 0);
	return altura;
}

void ARBOLSPL::auxAltura(node *nodo, int a)
{
	if (nodo->izquierda) auxAltura(nodo->izquierda, a + 1);
	if (nodo->derecha)   auxAltura(nodo->derecha, a + 1);
	if (EsHoja(nodo) && a > altura) altura = a;
}

void ARBOLSPL::graficarArbol(node* root)
{
	FILE *archivoGrafico;
	archivoGrafico = fopen("arbolSPL.dot", "w+");

	if (archivoGrafico) {
		fprintf(archivoGrafico, "%s", "digraph arbolSPL{ \n");
		fprintf(archivoGrafico, "%s", "00000000[label=\"Arbol_SPL\", fillcolor=\"lightblue\", style =\"filled\", shape=\"circle\"]; \n");
		generarArbol(&archivoGrafico, root);
		if (root != NULL) {
			fprintf(archivoGrafico, "\"00000000\"->\"%p\"; \n", (void*)root);
		}
		fprintf(archivoGrafico, "%s", "\n overlap=false \n fontsize=15; \n }");
		fclose(archivoGrafico);

		system("dot arbolSPL.dot -Tpng -o arbolSPL.png");
	}
	else {
		printf("%s\n", "Error abriendo el archivo :(");
	}
}

void ARBOLSPL::generarArbol(FILE **archivoGrafico, node* root)
{
	node* aux = root;
	generarArbolLabel(archivoGrafico, aux);
	aux = root;
	generarArbolEnlaces(archivoGrafico, aux);
}

void ARBOLSPL::generarArbolLabel(FILE **archivoGrafico, node *nodoAux)
{
	if (nodoAux != NULL) {
		fprintf((*archivoGrafico), "\"%p\"[label=\"Dato: %d\", fillcolor=\"lightblue\", style =\"filled\", shape=\"circle\"]; \n", (void*)nodoAux, nodoAux->clave);
		if (nodoAux->izquierda) {
			generarArbolLabel(archivoGrafico, nodoAux->izquierda);
		}
		if (nodoAux->derecha) {
			generarArbolLabel(archivoGrafico, nodoAux->derecha);
		}
	}
}

void ARBOLSPL::generarArbolEnlaces(FILE **archivoGrafico, node *nodoAux)
{
	node *act = nodoAux;

	if (act->izquierda) {
		fprintf((*archivoGrafico), "\"%p\"", (void*)act);
		fprintf((*archivoGrafico), "->\"%p\"", (void*)act->izquierda);
		generarArbolEnlaces(archivoGrafico, act->izquierda);
	}

	if (act->derecha) {
		fprintf((*archivoGrafico), "\"%p\"", (void*)act);
		fprintf((*archivoGrafico), "->\"%p\"", (void*)act->derecha);
		generarArbolEnlaces(archivoGrafico, act->derecha);
	}
}

const int ARBOLSPL::NumeroNodos(node* raiz)
{
	contador = 0;
	auxContador(raiz);
	return contador;
}

void ARBOLSPL::auxContador(node *nodo)
{
	contador++;
	if (nodo->izquierda) auxContador(nodo->izquierda);
	if (nodo->derecha)   auxContador(nodo->derecha);
}

int main()
{
	ARBOLSPL arbolspl;

	node *root = arbolspl.nuevoNodo(5);
	root = arbolspl.insert(root, 10);
	root = arbolspl.insert(root, 96);
	root = arbolspl.insert(root, 30);
	root = arbolspl.insert(root, 2);
	root = arbolspl.insert(root, 63);
	root = arbolspl.insert(root, 85);
	root = arbolspl.insert(root, 47);
	root = arbolspl.insert(root, 20);
	root = arbolspl.insert(root, 3);
	root = arbolspl.insert(root, 4);
	root = arbolspl.insert(root, 19);
	cout << "El resultado en preorden es \n";
	arbolspl.preOrder(root);
	arbolspl.graficarArbol(root);

	cin.get();
	return 0;
}

// This code is contributed by rathbhupendra 
