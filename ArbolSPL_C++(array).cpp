// [EDD]FINAL_201503702.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

class ARBOLSPL {
public:
	/*EL ARREGLO SE GRAFICA SIN CEROS PARA TENER MEJOR VISIBILIDAD*/
	int contador;
	int altura;
	int arreglo[80] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	void insertar(int clave);
	void registrar(int inicio, int clave);
	void derechaRotate();
	void izquierdaRotate();
	void splay(int inicio, int clave);
	void graficarArbol();
	void generarArbol(FILE **archivoGrafico);
	void generarArbolLabel(FILE **archivoGrafico, int indice);
	void generarArbolEnlaces(FILE **archivoGrafico, int indice);
	void ARBOLSPL::graficarArreglo();
};

// Una función de utilidad para la derecha.
// rotar subárbol arraigado con y
// Ver el diagrama dado arriba.
void ARBOLSPL::derechaRotate()
{
	/*node *y = x->izquierda;
	x->izquierda = y->derecha;
	y->derecha = x;*/
}

// Una función de utilidad a la izquierda.
// rotar subárbol enraizado con x
// Ver el diagrama dado arriba.
void ARBOLSPL::izquierdaRotate()
{
	/*node *y = x->derecha;
	x->derecha = y->izquierda;
	y->izquierda = x;*/
}

// Esta función trae la clave a
// root si la clave está presente en el árbol.
// Si la clave no está presente, entonces
// trae el último elemento accedido a
// raíz. Esta función modifica la
// árbol y devuelve la nueva raíz
void ARBOLSPL::splay(int inicio, int clave)
{
	int izquierda = (inicio * 2) + 1;
	int derecha = (inicio * 2) + 2;
	/*if (arreglo[inicio] > clave)
	{
		if (root[izquierda] == 0) return;

		// Zig-Zig (izquierda izquierda) 
		if (root[izquierda] > clave)
		{
			root[] = splay(root->izquierda->izquierda, clave);

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
	}*/
}

void ARBOLSPL::insertar(int clave) {
	if (arreglo[1] != 0) {
		registrar(1, clave);
	}
	else {
		arreglo[1] = clave;
	}
}

void ARBOLSPL::registrar(int inicio, int clave) {
	if (arreglo[inicio] != 0) {
		if (arreglo[inicio] != clave) {
			if (clave < arreglo[inicio]) {
				//izquierdo
				registrar((inicio * 2)+1, clave);
			}
			else {
				//derecho
				int sigr = (inicio * 2) + 2;
				registrar(sigr, clave);
			}
		}
	}
	else {
		arreglo[inicio] = clave;
		//splay(inicio, clave);
	}
}

void ARBOLSPL::graficarArbol()
{
	FILE *archivoGrafico;
	archivoGrafico = fopen("arbolSPL.dot", "w+");

	if (archivoGrafico) {
		fprintf(archivoGrafico, "%s", "digraph arbolSPL{ \n");
		fprintf(archivoGrafico, "%s", "00000000[label=\"Arbol_SPL\", fillcolor=\"lightblue\", style =\"filled\", shape=\"circle\"]; \n");
		generarArbol(&archivoGrafico);
		if (arreglo[1] > 0 && arreglo[1] < 1050) {
			fprintf(archivoGrafico, "\"00000000\"->\"%p\"; \n", (void*)arreglo[1]);
		}
		fprintf(archivoGrafico, "%s", "\n overlap=false \n fontsize=15; \n }");
		fclose(archivoGrafico);

		system("dot arbolSPL.dot -Tpng -o arbolSPL.png");
	}
	else {
		printf("%s\n", "Error abriendo el archivo :(");
	}
}

void ARBOLSPL::generarArbol(FILE **archivoGrafico)
{
	generarArbolLabel(archivoGrafico, 1);
	generarArbolEnlaces(archivoGrafico, 1);
}

void ARBOLSPL::generarArbolLabel(FILE **archivoGrafico, int indice)
{
	int izquierda = (indice * 2) + 1;
	int derecha = (indice * 2) + 2;

	if (arreglo[indice] > 0 && arreglo[1] < 1050) {
		fprintf((*archivoGrafico), "\"%p\"[label=\"Dato: %d\", fillcolor=\"lightblue\", style =\"filled\", shape=\"circle\"]; \n", (void*)arreglo[indice], arreglo[indice]);
		if (arreglo[izquierda] > 0 && arreglo[izquierda] < 1050) {
			generarArbolLabel(archivoGrafico, izquierda);
		}
		if (arreglo[derecha] > 0 && arreglo[derecha] < 1050) {
			generarArbolLabel(archivoGrafico, derecha);
		}
	}
}

void ARBOLSPL::generarArbolEnlaces(FILE **archivoGrafico, int indice)
{
	int izquierda = (indice * 2) + 1;
	int derecha = (indice * 2) + 2;

	if (arreglo[izquierda] > 0 && arreglo[izquierda] < 1050) {
		fprintf((*archivoGrafico), "\"%p\"", (void*)arreglo[indice]);
		fprintf((*archivoGrafico), "->\"%p\"", (void*)arreglo[izquierda]);
		generarArbolEnlaces(archivoGrafico, arreglo[izquierda]);
	}

	if (arreglo[derecha] > 0 && arreglo[derecha] < 1050) {
		fprintf((*archivoGrafico), "\"%p\"", (void*)arreglo[indice]);
		fprintf((*archivoGrafico), "->\"%p\"", (void*)arreglo[derecha]);
		generarArbolEnlaces(archivoGrafico, arreglo[derecha]);
	}
}

void ARBOLSPL::graficarArreglo()
{
	FILE *archivoGrafico;
	archivoGrafico = fopen("arbolSPLArr.dot", "w+");

	if (archivoGrafico) {
		fprintf(archivoGrafico, "%s", "digraph ArregloSPL{ rankdir=LR \n");
		fprintf(archivoGrafico, "%s", "00000000[label=\"Arreglo\", fillcolor=\"blue\", style =\"filled\", shape=\"circle\"]; \n");
		
		for (int i = 1; i < 80; i++)
		{
			if (arreglo[i] != 0) {
				fprintf(archivoGrafico, "\"%p\"[label=\"Dato: %d\", fillcolor=\"lightblue\", style =\"filled\", shape=\"box\"]; \n", (void*)arreglo[i], arreglo[i]);
			}
		}

		if (arreglo[3] != 0 || arreglo[4] != 0) {
			fprintf(archivoGrafico, "\"%p\"", (void*)arreglo[1]);

			for (int i = 2; i < 80; i++)
			{
				if (arreglo[i] != 0) {
					fprintf(archivoGrafico, "->\"%p\"", (void*)arreglo[i]);

				}
			}
		}

		if (arreglo[1] > 0) {
			fprintf(archivoGrafico, "\"00000000\"->\"%p\"; \n", (void*)arreglo[1]);
		}
		fprintf(archivoGrafico, "%s", "\n overlap=false \n fontsize=15; \n }");
		fclose(archivoGrafico);

		system("dot arbolSPLArr.dot -Tpng -o arbolSPLArr.png");
	}
	else {
		printf("%s\n", "Error abriendo el archivo :(");
	}
}

int main()
{
	ARBOLSPL arbolspl;

	arbolspl.insertar(5);
	arbolspl.insertar(10);
	arbolspl.insertar(96);
	arbolspl.insertar(30);
	arbolspl.insertar(2);
	arbolspl.insertar(63);
	arbolspl.insertar(85);
	arbolspl.insertar(47);
	arbolspl.insertar(20);
	arbolspl.insertar(3);
	
	arbolspl.graficarArbol();
	arbolspl.graficarArreglo();

	cout << "FIN" << endl;

	cin.get();
	return 0;
}

// This code is contributed by rathbhupendra 
