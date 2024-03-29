#include "list.h"

namespace movies {
    typedef std::string Label;

    enum EntityType {PERSON, MOVIE};
    enum RelationshipType {ACTED, DIRECTED, PRODUCED};
    
    struct Relationship{    //(Edge)
        RelationshipType type;
        Relationship* next;
        Label adjacentLabel;
    };
    typedef Relationship *Arco;
    
    struct Entity{  //(Vertex / Node)
        string name;
        EntityType type;
        Arco adjacentList;  //Puntatore al primo degli archi
        Entity* nextEntity;
        bool visited;
    };
    typedef Entity *Nodo;
    
    struct DataBase{    
        Nodo movies;    //Puntatore a tutti i film del DB
        Nodo people;    //Puntatore a tutte le persone del DB
        int numMovies;
        int numPeople;
        int numActed;
        int numDirected;
        int numProduced;
    };
    typedef DataBase *MovieDB;
    
    const MovieDB emptyDB = nullptr;

    const int NO_BACON_NUMBER = 999999999;

    // crea un database vuoto
    MovieDB createEmpty();

    // aggiunge un nodo di tipo PERSON o MOVIE con nome l, fallisce se un nodo con quell'etichetta è già presente
    // restituisce true se l'inserimento ha avuto successo
    bool addEntity(MovieDB &mdb, const Label l, const EntityType t);

    // aggiunge un arco tra `person` e `movie` con tipo t (ACTED, DIRECTED o PRODUCED)
    // fallisce se i nodi non hanno tipo appropriato o se un arco identico (stessa persona, film e tipo) esiste già
    bool addRelationship(MovieDB &mdb, const Label person, const Label movie, const RelationshipType t);

    // numero di nodi con tipo t
    int numEntitiesPerType(const MovieDB &mdb, EntityType t);

    // numero di archi con tipo t
    int numRelationshipsPerType(const MovieDB &mdb, RelationshipType t);

    // lista, senza ripetizioni, di attori che hanno interpretato un film con l'attore con etichetta l
    // restituisce una lista vuota se l non è una persona o non appartiene al grafo
    list::List coActors(const MovieDB &mdb, Label l);

    // lista, senza ripetizioni, di attori che hanno interpretato un film prodotto da l
    // restituisce una lista vuota se l non è una persona o non appartiene al grafo
    list::List actorsProducedBy(const MovieDB &mdb, Label l);

    // lista, senza ripetizioni, di attori che hanno interpretato un film diretto da l
    // restituisce una lista vuota se l non è una persona o non appartiene al grafo
    list::List actorsDirectedBy(const MovieDB &mdb, Label l);

    // numero di Bacon di l (vedi testo); restituisce NO_BACON_NUMBER se l non è una persona o non ha un numero di Bacon
    int BaconNumber(const MovieDB &mdb, Label l);    

    //Ausiliarie mie:

    int peopleBaconNumber(const MovieDB& mdb, Label l, Arco currR, int kb); //Per il bacon number, si muove tra le persone
    int moviesBaconNumber(const MovieDB& mdb, Label l, Arco currR, int kb); //Per il bacon number, si muove tra i film
}

// stampa il contenuto del database; siete liberi di scegliere il formato, ma pensatelo in modo che sia comprensibile per un umano
void printDB(const movies::MovieDB &mdb);
