/* Todo lo que necesitamos y más */
#include <bits/stdc++.h>

/* Para no estar usando std:: todo el tiempo */
using namespace std;

/* Clase vértice */
class Vertice
{
    /* Cosas privadas */
    private:
        /* Identificador del vértice */
        string id;
        /* Conjunto con los identificadores de los vecinos del vértice
           nos permite buscar en O(log n) sin usar tanta memoria
           y evitar duplicados */
        set<string> vecinosid;

    /* Cosas públicas */
    public:
        /* Constructor vacío */
        Vertice()
        {

        };

        /* Constructor con id */
        Vertice(string iid)
        {
            id = iid;
        };

        /* Obtener copia de la propiedad id */
        string readId()
        {
            return id;
        };

        /* Asignar valor a id */
        void setId(string iid)
        {
            id = iid;
        };

        /* Agregar identificador de vecino al conjunto de identificadores de vecinos */
        void addVecino(string vid)
        {
            /* Si ya está no hace nada */
            vecinosid.insert(vid);
        };

        /* Checar si el vértice tiene a otro como vecíno */
        bool hasVecino(string vid)
        {
            return vecinosid.count(vid) > 0;
        };
};

/* Clase arista */
class Arista
{
    /* Cosas privadas */
    private:
        /* Peso de la arista */
        int peso;

    /* Cosas públicas */
    public:
        /* Constructor vacío */
        Arista()
        {

        };

        /* Constructor con peso */
        Arista(int p)
        {
            peso = p;
        };

        /* Obtener copia de la propiedad peso */
        int readPeso()
        {
            return peso;
        };

        /* Asignar valor a peso */
        void setPeso(int p)
        {
            peso = p;
        };
};

/* Definimos una adjacencia como un vértice vecino y la arista que lleva a el */
typedef pair<Vertice, Arista> adjacencia;

/* Clase para poder comparar pares en el algoritmo de prim */
class comprim
{
    public:
        /* Nos permitirá ordenar de menor a mayor */
        int operator() (pair<int, string>& p1,  pair<int, string>& p2)
        {
            return p1.first > p2.first;
        };
};

/* Clase gráfica */
class Grafica
{
    /* Cosas privadas */
    public:
        /* Un diccionario con la lista de adjacencias de cada vértice */
        map<string, vector<adjacencia>> ladjacencias;
        /* Un diccionario con los vertices */
        map<string, Vertice> vertices;

    /* Cosas públicas */
    public:
        /* Constructor vacío */
        Grafica()
        {

        };

        /* Checar si la gráfica contiene al vértice con ese identificador */
        bool hasVertice(string vid)
        {
            return vertices.count(vid) > 0;
        };

        /* Añadir un vértice con ese identificador a la gráfica */
        void addVertice(string vid)
        {
            /* Solo añadirlo si no existe aún */
            if (!hasVertice(vid))
            {
                /* Instanciar vértice con el identificador */
                Vertice v(vid);
                /* Crear su lista de adjacencias */
                vector<adjacencia> adjacencias;
                ladjacencias[vid] = adjacencias;
                /* Añadirlo al diccionario de vertices */
                vertices[vid] = v;
            }
        };

        /* Obtener una copia del vértice con ese identificador */
        Vertice readVertice(string vid)
        {
            return vertices[vid];
        };

        /* Obtener acceso al vértice con ese identificador */
        Vertice* getVertice(string vid)
        {
            return &vertices[vid];
        };

        /* Añadir una arista con ese peso entre dos vértices con esos identificadores */
        void addArista(string v1id, string v2id, int p)
        {
            /* Acceder a los vertices con esos identificadores */
            Vertice* v1 = &vertices[v1id];
            Vertice* v2 = &vertices[v2id];

            /* Checar que ambos vertices estén en la gráfica y que la arista no exista aún */
            if (hasVertice(v1id) && hasVertice(v2id) && !v1->hasVecino(v2id) && !v2->hasVecino(v1id))
            {
                /* Instanciar arista con peso */
                Arista a(p);
                /* Actualizar las listas de adjacencias */
                ladjacencias[v1id].push_back(make_pair(vertices[v2id], a));
                ladjacencias[v2id].push_back(make_pair(vertices[v1id], a));
                /* Añadir al conjunto de vecinos de cada vértice */
                v1->addVecino(v2id);
                v2->addVecino(v1id);
            }
        };

        /* Mostrar la gráfica como listas de adyacencia */
        void show()
        {
            cout << "Gráfica:" << '\n';
            /* Recorrer las listas de adjacencias */
            for (auto e : ladjacencias)
            {
                cout << "Vertice " << e.first << ":\n\tAdj: ";
                /* Para cada adjacencia obtener el identificador del vecino y el peso de la arista que los conecta */
                for (auto adj : e.second)
                {
                    cout << adj.first.readId() << "(" << adj.second.readPeso() << "), ";
                }
                cout << '\n';
            }
        };

        /* Algoritmo de prim con vértice default */
        map<string, pair<string, int>> prim()
        {
            /* Elegimos el primer vértice para ejecutar el algoritmo */
            return prim(vertices.begin()->first);
        };

        /* Algoritmo de prim iniciado por el vértice con ese identificador */
        map<string, pair<string, int>> prim(string iid)
        {
            /* Creamos un heap que contendrá el peso para llegar al vertice con un identificador
               (incluimos nuestra clase de comparación para obtener un min heap) */
            priority_queue<pair<int, string>, vector<pair<int, string>>, comprim> heap;

            /* Identificador del vértice inicial */
            string iniid = iid;

            /* Diccionario con peso mínimo al momento para llegar al vértice con ese identificador desde alguno de sus vecinos */
            map<string, int> pesoa;
            /* Inicializarlos con el máximo (infinito) */
            for (auto e : vertices)
            {
                pesoa[e.first] = INT_MAX;
            }
            /* Diccionario con el padre de cada vértice y el peso de la arista que los conecta */
            map<string, pair<string, int>> padre;
            /* Diccionario para saber si un vértice ya está en el árbol */
            map<string, bool> enArbol;

            /* Meter el vértice inicial al heap con peso 0 */
            heap.push(make_pair(0, iniid));
            pesoa[iniid] = 0;

            /* Mientras tengamos elementos en el heap ejecutar */
            while (!heap.empty())
            {
                /* Obtener el elemento mínimo del heap (el tope) y obtener el identificador del vértice de ahí */
                string vid = heap.top().second;
                /* Eliminarlo del heap */
                heap.pop();

                /* Si el vértice ya está en el árbol pasar al siguiente vértice */
                if (enArbol[vid] == true)
                {
                    continue;
                }

                /* Añadir al vértice al árbol */
                enArbol[vid] = true;

                /* Checar sus adjacencias con peso */
                for (auto adj : ladjacencias[vid])
                {
                    /* Obtener el identificador del vecino */
                    string vecinoid = adj.first.readId();
                    /* Obtener el peso de la arista que los conecta */
                    int peso = adj.second.readPeso();

                    /* Si el vecino no está en el árbol y el peso para llegar al vecino es mayor al peso de la
                       arista actual, entonces actualizar el peso para llegar, actualizar el padre y añadir el vecino al heap */
                    if (!enArbol[vecinoid] && pesoa[vecinoid] > peso)
                    {
                        pesoa[vecinoid] = peso;
                        heap.push(make_pair(peso, vecinoid));
                        padre[vecinoid] = make_pair(vid, peso);
                    }
                }
                /* Caso en el que solo tengamos un vértice en la componente conexa */
                if (ladjacencias[vid].size() == 0)
                {
                    padre[vid] = make_pair(vid, 0);
                }
            }

            /* Devolver los padres de donde podemos reconstruir el árbol */
            return padre;
        };

        /* Mostrar el resultado del algoritmo */
        void showPrim(map<string, pair<string, int>> r)
        {
            /* Peso total */
            int pt = 0;
            /* Guardamos las aristas para imprimir después de los vértices */
            string aristas = "";
            /* Conjunto para guardar los vértices sin duplicados */
            set<string> vs;
            /* Recorrer los resultados */
            for (auto e : r)
            {
                /* Añadir al conjunto de vertices ambos lados de la arista */
                vs.insert(e.first);
                vs.insert(e.second.first);
                /* Añadir la arista en formato al string */
                aristas += e.first+","+e.second.first+":"+to_string(e.second.second)+"\n";
                /* Sumar el peso de la arista */
                pt += e.second.second;
            }
            /* Imprimir los vértices */
            for (string vid : vs)
            {
                cout << vid;
                /* Solo imprimir , si no es el último elemento */
                if (vid != *(--vs.end()))
                {
                    cout << ",";
                }
            }
            cout << '\n';
            /* Imprimir aristas */
            cout << aristas;
            /* Imprimir peso total */
            cout << "Peso total: " << pt << '\n';
        }

        /* Algoritmo de prim para multiples componentes conexas */
        vector<map<string, pair<string, int>>> multiprim()
        {
            /* Lista de resultados de cada ejecución individual */
            vector<map<string, pair<string, int>>> r;

            /* Ejecutar prim para algún vértice inicial */
            map<string, pair<string, int>> pr = prim();
            /* Guardar los resultados */
            r.push_back(pr);

            /* Obtener los vértices que se visitaron en la ejecución */
            set<string> vs;
            for (auto e : pr)
            {
                vs.insert(e.first);
                vs.insert(e.second.first);
            }

            /* Mientras no se hayan visitado todos los vértices ejecutar prim iniciando por
               el primer vértice que no haya sido visitado aún */
            while (vs.size() != vertices.size())
            {
                /* Recorrer los vértices para encontrar el primero no visitado */
                for (auto e : vertices)
                {
                    /* Si no ha sido visitado ejecutar prim iniciando en ese */
                    if (vs.count(e.first) == 0)
                    {
                        pr = prim(e.first);
                    }
                }
                /* Guardar los resultados */
                r.push_back(pr);
                /* Actualizar el conjunto de vértices visitados */
                for (auto e : pr)
                {
                    vs.insert(e.first);
                    vs.insert(e.second.first);
                }
            }

            /* Devolver la lista de resultados */
            return r;
        };

        /* Para cada componente conexa mostar el resultado */
        void showMultiPrim(vector<map<string, pair<string, int>>> v)
        {
            cout << "Hay " << v.size() << " componentes conexas\n";
            for (int i = 0; i < v.size(); i++)
            {
                cout << "Componente conexa " << (i+1) << ":\n";
                showPrim(v[i]);
            }
        };
};

/* Main */
int main()
{
    /* Crear nueva gráfica */
    Grafica g;

    /* Obtener línea con los vértices */
    string l;
    getline(cin, l);
    stringstream ss(l);

    /* Separar los vértices */
    string vid;
    while (getline(ss, vid, ','))
    {
        /* Añadirlos a la gráfica */
        g.addVertice(vid);
    }

    /* Mostrar la gráfica */
    g.show();

    /* Obtener las aristas */
    while (getline(cin, l))
    {
        if (l == "")
        {
            continue;
        }
        /* Separar los vértices */
        stringstream ss1(l);
        string v1id;
        string l2;
        getline(ss1, v1id, ',');
        getline(ss1, l2, ',');

        if (v1id == "" || l2 == "")
        {
            continue;
        }

        /* Separar el peso */
        stringstream ss2(l2);
        string v2id;
        string ps;
        getline(ss2, v2id, ':');
        getline(ss2, ps, ':');

        if (v2id == "" || ps == "")
        {
            continue;
        }

        /* Añadir arista */
        g.addArista(v1id, v2id, stoi(ps));
    }

    /* Mostar la gráfica */
    g.show();

    /* Ejecutar prim en las componentes conexas y mostrar los resultados */
    cout << "MultiPrim\n";
    g.showMultiPrim(g.multiprim());
}
