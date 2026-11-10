#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct NodoAVL {
    int id;
    string nombre; 
    NodoAVL *izq;
    NodoAVL *der;
    int fVal;
    int cantSubarbol;

    NodoAVL (int id, string nombre): id(id), nombre (nombre), izq (NULL), der (NULL), fVal (0), cantSubarbol (1){} 
};

struct representacionAVL {
    NodoAVL *raiz;
    int cantElementos;

    representacionAVL (): raiz (NULL), cantElementos (0) {}
};

typedef representacionAVL *AVL;

AVL CrearAVL (){
    return new representacionAVL ();
}

//ADD
int contar(NodoAVL* nodo) {
    if (!nodo) return 0;
    return nodo->cantSubarbol;     
}

void actualizarSubArbol(NodoAVL *nodo) {
    if (nodo) nodo -> cantSubarbol = 1 + contar(nodo -> izq) + contar(nodo -> der);
}

bool insertarAVLRecursivo (NodoAVL *&nodo, int id, string nombre, bool &varioAltura){
    if (!nodo) {
        nodo = new NodoAVL (id, nombre);
        varioAltura = true;
        return true;
    }

    bool insertado = false;

    if (id < nodo -> id) {
        insertado = insertarAVLRecursivo(nodo ->izq, id, nombre, varioAltura);

        if (varioAltura) {
            switch (nodo -> fVal) {
                case 1: nodo -> fVal = 0; varioAltura = false; break;
                case 0: nodo -> fVal = -1; break;
                case -1: {
                    NodoAVL *p1 = nodo -> izq;
                    if (p1 -> fVal == -1) {
                        nodo -> izq = p1 -> der;
                        p1 -> der = nodo;
                        actualizarSubArbol(nodo);
                        actualizarSubArbol(p1);
                        nodo -> fVal = 0; p1 -> fVal = 0;
                        nodo = p1;
                    } else {
                        NodoAVL *p2 = p1 -> der;
                        p1 -> der = p2 -> izq;
                        p2 -> izq = p1;
                        nodo -> izq = p2 -> der;
                        p2 -> der = nodo;
                        actualizarSubArbol(nodo);
                        actualizarSubArbol(p1);
                        actualizarSubArbol(p2);
                        nodo -> fVal = (p2 -> fVal == -1) ? 1 : 0;
                        p1 -> fVal = (p2 -> fVal == 1) ? -1 : 0;
                        p2 -> fVal = 0;
                        nodo = p2;
                    }
                    varioAltura = false;
                    break;
                }
            }
        }
    } else if (nodo -> id < id) {
        insertado = insertarAVLRecursivo(nodo -> der, id, nombre, varioAltura);

        if (varioAltura) {
            switch (nodo -> fVal) {
                case -1: nodo -> fVal = 0; varioAltura = false; break;
                case 0: nodo -> fVal = 1; break;
                case 1: {
                    NodoAVL *p1 = nodo -> der;
                    if (p1->fVal == 1) {
                        nodo -> der = p1 -> izq;
                        p1 -> izq = nodo;
                        actualizarSubArbol(nodo);
                        actualizarSubArbol(p1);
                        nodo -> fVal = 0; p1 -> fVal = 0;
                        nodo = p1;
                    } else {
                        NodoAVL *p2 = p1 -> izq;
                        p1 -> izq = p2 -> der;
                        p2 -> der = p1;
                        nodo -> der = p2 -> izq;
                        p2 -> izq = nodo;
                        actualizarSubArbol(nodo);
                        actualizarSubArbol(p1);
                        actualizarSubArbol(p2);
                        nodo -> fVal = (p2 -> fVal == 1) ? -1 : 0;
                        p1 -> fVal = (p2->fVal == -1) ? 1 : 0;
                        p2 -> fVal = 0;
                        nodo = p2;
                    }
                    varioAltura = false;
                    break;
                }
            }
        }
    } else {
        nodo -> nombre = nombre;
        return false;
    }
    actualizarSubArbol(nodo);
    return insertado;
}

void ADD (AVL arbol, int id, string nombre){
    bool varioAltura = false;
    bool insertadoId = insertarAVLRecursivo(arbol ->raiz, id, nombre, varioAltura);
    if (insertadoId) {
        arbol -> cantElementos++;;
    }
}

//FIND
NodoAVL* findRecursivo (NodoAVL* nodo, int id){
    if(!nodo) return NULL;
    if(nodo -> id < id) return findRecursivo(nodo -> der, id);
    if(nodo -> id > id) return findRecursivo (nodo -> izq, id);
    return nodo;
}

NodoAVL* FIND (AVL arbol, int id){
    return findRecursivo (arbol -> raiz, id);
}

//RANK
int rankRecursivo(NodoAVL* nodo, int idMax) {
    if (!nodo) return 0;
    if (nodo -> id >= idMax) return rankRecursivo(nodo->izq, idMax);
    return 1 + contar(nodo->izq) + rankRecursivo(nodo->der, idMax);
}

int RANK (AVL arbol, int idMax){
    return rankRecursivo (arbol -> raiz, idMax);
}

//COUNT
int COUNT (AVL arbol){
    return arbol -> cantElementos;
}

//DESTRUIR
void destruirRecursivo (NodoAVL *&nodo){
    if(!nodo) return;
    destruirRecursivo (nodo -> izq);
    destruirRecursivo (nodo -> der);
    delete nodo;
    nodo = NULL;
}

void DESTRUIR (AVL& arbol){
    destruirRecursivo (arbol -> raiz);
    delete arbol;
    arbol = NULL;
}

int main() {
    /*ifstream myFile("100.inej1.txt");
    if (myFile.good()) cin.rdbuf(myFile.rdbuf());
    ofstream myFile2("100.mine.txt");
    if (myFile2.good()) cout.rdbuf(myFile2.rdbuf());*/

    int cantAcciones;
    cin >> cantAcciones;

    AVL arbol = CrearAVL();

    for (int i = 0; i < cantAcciones; i++) {
        string accion; cin >> accion;
        if (accion == "ADD") {
            int id; string nombre;
            cin >> id >> nombre;
            ADD(arbol, id, nombre);
        } else if (accion == "FIND") {
            int id; cin >> id;
            NodoAVL *encontrado = FIND(arbol, id);
            if (encontrado) cout << encontrado -> nombre << endl;
            else cout << "jugador_no_encontrado" << endl;
        } else if (accion == "RANK") {
            int id; cin >> id;
            cout << RANK(arbol, id) << endl;
        } else {
            cout << COUNT(arbol) << endl;
        }
    }

    DESTRUIR(arbol);
    return 0;
}