#include <cassert>
#include <string>
#include <iostream>
#include <limits>
#include "funciones/enteros.cpp" 
#include <cmath>

using namespace std;

bool esPrimo (int n){
    if (n <= 1 || n % 2 == 0 && n != 2) return false;
    if (n == 2) return true;
    for (int i = 3; i < n / 2; i += 2){
        {
            if (n % i == 0) return false;
        }
    }
    return true;
}

int primoSupMinimo (int n){
    while (!esPrimo(++ n));
    return n;
}

int fHashColisiones(int tope, int n, int i) {
    return hacerPositivo((n + (int)pow(i, 2)) % tope);
}

int fHashPrincipal (int tope, string clave){
    int ret = 0;
    int i = 0;

    while (clave [i] != '\0'){
        ret = (ret + int (clave[i])) * 37; //Funcion Horner
        i++;
    }
    return hacerPositivo(ret % tope);
}

struct nodoPath {
    string path;
    string titulo;
    int tiempo;
    bool estaBorrado; //Porque pide borrado logico
    nodoPath *sig; //Para poder seguir recorriendo la lista
    nodoPath *ant;

    nodoPath (string path, string titulo, int tiempo): path (path), titulo (titulo), tiempo (tiempo), estaBorrado (false), sig (NULL), ant (NULL){}
};

struct nodoDominio {
    string nombreDominio;
    int cantElementos;
    int tope;
    nodoPath *primero; //Para poder hacer LISTDOMAIN en O(k), es el primer elemento de la lista
    nodoPath **tablaPath;

    nodoDominio (string nombreDominio, int topeInicial){
        this -> nombreDominio = nombreDominio;
        cantElementos = 0;
        tope = primoSupMinimo (topeInicial * 2);
        tablaPath = new nodoPath*[tope];
        primero = NULL;
        for (int i = 0; i < tope; i++) tablaPath [i] = NULL;
    }
};

struct representacionTabla {
    int cantElementos;
    int tope;
    nodoDominio **tablaDominios;

    representacionTabla (int topeMinimo){
        cantElementos = 0;
        tope = primoSupMinimo (topeMinimo * 2);
        tablaDominios = new nodoDominio*[tope];
        for (int i = 0; i < tope; i++) tablaDominios [i] = NULL;
    }
};

typedef representacionTabla *Tabla;

Tabla crear (int topeMinimo){
    return new representacionTabla (topeMinimo);
}

//PUT
void PUT (Tabla tabla, string dominio, string path, string titulo, int tiempo){
    nodoDominio **tablaDominios = tabla -> tablaDominios;
    int posicion = fHashPrincipal (tabla -> tope, dominio);
    int i = 1;

    while (tablaDominios [posicion] && tablaDominios [posicion] -> nombreDominio != dominio){
        posicion = fHashColisiones (tabla -> tope, posicion, i);
        i++;
    }

    if (!tablaDominios [posicion]) tablaDominios [posicion] = new nodoDominio (dominio, tabla -> tope);

    nodoDominio *dominioActual = tablaDominios [posicion];
    
    nodoPath **tablaPaths = dominioActual -> tablaPath;
    posicion = fHashPrincipal (dominioActual -> tope, path);
    i = 1;

    while (tablaPaths [posicion] && tablaPaths [posicion] -> path != path){
        posicion = fHashColisiones (dominioActual -> tope, posicion, i);
        i++;
    }

    nodoPath *pathActual = tablaPaths [posicion];

    if (!pathActual){
        pathActual = new nodoPath (path, titulo, tiempo);
        tablaPaths [posicion] = pathActual;
        tabla -> cantElementos++;
        dominioActual -> cantElementos++;
    }
    else{
        pathActual -> titulo = titulo;
        if (pathActual -> estaBorrado){
            pathActual -> estaBorrado = false;
            tabla -> cantElementos++;
            dominioActual -> cantElementos++;
        }
        else{
            if (pathActual -> tiempo == tiempo) return;

            if (pathActual -> ant) pathActual -> ant -> sig = pathActual -> sig;
            else dominioActual -> primero = pathActual -> sig;
            if (pathActual -> sig) pathActual -> sig -> ant = pathActual -> ant;
            pathActual -> sig = NULL;
            pathActual -> ant = NULL;
        }
        pathActual -> tiempo = tiempo;
    }

    nodoPath *anterior = NULL;
    nodoPath *actual = dominioActual -> primero;
    while (actual && actual -> tiempo >= tiempo){
        anterior = actual;
        actual = actual -> sig;
    }

    pathActual -> sig = actual;
    pathActual -> ant = anterior;
    if (actual) actual -> ant = pathActual;
    if (anterior) anterior -> sig = pathActual;
    else dominioActual -> primero = pathActual;
}

//GET
nodoPath* GET (Tabla tabla, string dominio, string path){
    nodoDominio **tablaDominios = tabla -> tablaDominios;
    int posicion = fHashPrincipal (tabla -> tope, dominio);
    int i = 1;

    while (tablaDominios [posicion] && tablaDominios [posicion] -> nombreDominio != dominio){
        posicion = fHashColisiones (tabla -> tope, posicion, i);
        i++;
    }

    nodoDominio *dominioActual = tablaDominios [posicion];
    if (dominioActual){
        nodoPath **tablaPaths = dominioActual -> tablaPath;
        posicion = fHashPrincipal (dominioActual -> tope, path);
        i = 1;

        while (tablaPaths [posicion] && tablaPaths [posicion] -> path != path){
            posicion = fHashColisiones (dominioActual -> tope, posicion, i);
            i++;
        }

        nodoPath *pathActual = tablaPaths [posicion];
        if(pathActual && !pathActual -> estaBorrado){
            return pathActual;
        }
    }

    return NULL;
}

//REMOVE
void REMOVE(Tabla datos, string dominio, string path){
    nodoDominio **tablaDominios = datos -> tablaDominios;
    int posicion = fHashPrincipal (datos -> tope, dominio);
    int i = 1;

    while (tablaDominios [posicion] && tablaDominios [posicion] -> nombreDominio != dominio){
        posicion = fHashColisiones (datos -> tope, posicion, i);
        i++;
    }

    nodoDominio *dominioActual = tablaDominios [posicion];
    if (dominioActual){
        nodoPath **tablaPaths = dominioActual -> tablaPath;
        posicion = fHashPrincipal (dominioActual -> tope, path);
        i = 1;

        while (tablaPaths [posicion] && tablaPaths [posicion] -> path != path){
            posicion = fHashColisiones (dominioActual -> tope, posicion, i);
            i++;
        }

        nodoPath *pathActual = tablaPaths [posicion];
        if (pathActual && !pathActual -> estaBorrado){
            pathActual -> estaBorrado = true;
            dominioActual -> cantElementos --;
            datos -> cantElementos --;
            if (pathActual -> ant) pathActual -> ant -> sig = pathActual -> sig;
            else dominioActual -> primero = pathActual -> sig;
            if (pathActual -> sig) pathActual -> sig -> ant = pathActual -> ant;
            pathActual -> sig = NULL;
            pathActual -> ant = NULL;
        }
    }
}

//CONTAINS
bool CONTAINS(Tabla datos, string dom, string path){
    nodoDominio **tablaDominios = datos -> tablaDominios;
    int posicion = fHashPrincipal (datos -> tope, dom);
    int i = 1;
    
    while (tablaDominios [posicion] && tablaDominios [posicion] -> nombreDominio != dom){
        posicion = fHashColisiones(datos -> tope, posicion, i);
        i++;
    }

    nodoDominio *dominioActual = tablaDominios [posicion];
    if (dominioActual){
        nodoPath **tablaPaths = dominioActual -> tablaPath;
        posicion = fHashPrincipal(dominioActual -> tope, path);
        i = 1;

        while (tablaPaths [posicion] && tablaPaths [posicion] -> path != path){
            posicion = fHashColisiones (dominioActual -> tope, posicion, i);
            i++;
        }

        if (tablaPaths [posicion] && !tablaPaths [posicion] -> estaBorrado){
            return true;
        }
    }

    return false;
}

//COUNT_DOMAIN
int COUNT_DOMAIN(Tabla datos, string dom){
    nodoDominio **tablaDominios = datos -> tablaDominios;
    int posicion = fHashPrincipal (datos -> tope, dom);
    int i = 1;

    while (tablaDominios [posicion] && tablaDominios [posicion] -> nombreDominio != dom){
        posicion = fHashColisiones (datos -> tope, posicion, i);
        i++;
    }

    if (!tablaDominios [posicion]) return 0;
    return tablaDominios [posicion] -> cantElementos;
}

//LIST_DOMAIN
void LIST_DOMAIN(Tabla datos, string dom){
    nodoDominio **tablaDominios = datos -> tablaDominios;
    int posicion = fHashPrincipal (datos -> tope, dom);
    int i = 1;

    while (tablaDominios [posicion] && tablaDominios [posicion] -> nombreDominio != dom){
        posicion = fHashColisiones (datos -> tope, posicion, i);
        i++;
    }

    if (tablaDominios [posicion] && tablaDominios [posicion] -> cantElementos != 0){
        nodoPath *pathActual = tablaDominios [posicion] -> primero;
        while (pathActual){
            cout << pathActual -> path << " ";
            pathActual = pathActual -> sig;
        }
        cout << endl;
    } else {
        cout << "sin_informacion" << endl;
    }
}

//CLEAR_DOMAIN
void CLEAR_DOMAIN(Tabla datos, string dom){
    nodoDominio **tablaDominios = datos -> tablaDominios;
    int posicion = fHashPrincipal (datos -> tope, dom);
    int i = 1;

    while (tablaDominios [posicion] && tablaDominios [posicion] -> nombreDominio != dom){
        posicion = fHashColisiones (datos -> tope, posicion, i);
        i++;
    }

    if (tablaDominios [posicion]){
        nodoDominio *dominioActual = tablaDominios [posicion];
        nodoPath **tablaPath = dominioActual -> tablaPath;
        for (int j = 0; j < dominioActual -> tope; j++){
            nodoPath *pathActual = tablaPath [j];
            if (pathActual){
                delete pathActual;
                tablaPath [j] = NULL;
            }
        }
        datos -> cantElementos -= dominioActual -> cantElementos;
        dominioActual -> cantElementos = 0;
        dominioActual -> primero = NULL;
    }
}

//SIZE
int SIZE(Tabla datos){
    return datos -> cantElementos;
}

//CLEAR
void CLEAR(Tabla &datos){
    nodoDominio **tablaDominios = datos -> tablaDominios;

    for (int i = 0; i < datos -> tope; i++){
        nodoDominio *dominioActual = tablaDominios [i];
        if (dominioActual){
            nodoPath **tablaPathActual = dominioActual -> tablaPath;

            for (int j = 0; j < dominioActual -> tope; j++){
                nodoPath *pathActual = tablaPathActual [j];
                if (pathActual){
                    delete pathActual;
                    tablaPathActual [j] = NULL;
                }
            }
            delete [] dominioActual -> tablaPath;
            delete dominioActual;
            tablaDominios [i] = NULL;
        }
    }
    datos -> cantElementos = 0;
}

//DESTRUIR
void DESTRUIR (Tabla &datos){
    CLEAR (datos);
    delete [] datos -> tablaDominios;
    datos -> tablaDominios = NULL;
    delete datos;
    datos = NULL;
}


int main()
{
    /*ifstream myFile("100.in.txt");
    cin.rdbuf(myFile.rdbuf());
    // Si desean tirar la salida a un archivo, usen las siguientes líneas (si no, sáquenlas):
    ofstream myFile2("100.mine.txt");*/

    int cantAcciones;
    cin >> cantAcciones;
    Tabla datos = crear(cantAcciones);
    string accion;

    for (int i = 0; i < cantAcciones; i++){
        cin >> accion;
        if (accion == "PUT") {
            string dom, path, titulo;
            int tiempo;
            cin >> dom >> path >> titulo >> tiempo;
            PUT(datos, dom, path, titulo, tiempo);
        }
        else if (accion == "GET") {
            string dom, path;
            cin >> dom >> path;
            nodoPath* nodo = GET(datos, dom, path);
            if (nodo) cout << nodo -> titulo << " " << nodo -> tiempo << endl;
            else cout << "recurso_no_encontrado" << endl;
        }
        else if (accion == "REMOVE") {
            string dom, path;
            cin >> dom >> path;
            REMOVE(datos, dom, path);
        }
        else if (accion == "CONTAINS") {
            string dom, path;
            cin >> dom >> path;
            if (CONTAINS(datos, dom, path)) cout << "true" << endl;
            else cout << "false" << endl;
        }
        else if (accion == "COUNT_DOMAIN") {
            string dom;
            cin >> dom;
            cout << COUNT_DOMAIN(datos, dom) << endl;
        }
        else if (accion == "LIST_DOMAIN") {
            string dom;
            cin >> dom;
            LIST_DOMAIN(datos, dom);
        }
        else if (accion == "CLEAR_DOMAIN") {
            string dom;
            cin >> dom;
            CLEAR_DOMAIN(datos, dom);
        }
        else if (accion == "SIZE") {
            cout << SIZE(datos) << endl;
        }
        else { 
            CLEAR(datos);
        }
    }

    DESTRUIR (datos);
    return 0;
}