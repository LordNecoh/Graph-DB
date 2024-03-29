#include "movies.h"

using namespace movies;

//FUNZIONI

MovieDB movies::createEmpty(){
    MovieDB newDB = new DataBase;
    newDB->movies = nullptr;
    newDB->people = nullptr;
    newDB->numMovies = 0;
    newDB->numPeople = 0;
    newDB->numActed = 0;
    newDB->numDirected = 0;
    newDB->numProduced = 0;
    return newDB;
}

//Aggiunta Entità:

bool recursiveAddEntity(Nodo &currEntity, const Label l, const EntityType t){   //Gestisco Porzione ridotta DataBase

    if(!currEntity){    //Inserimento in coda
        currEntity = new Entity;
        currEntity->name = l;
        currEntity->type = t;
        currEntity->adjacentList = nullptr;
        currEntity->nextEntity = nullptr;
        currEntity->visited = false;
        return true;
    }
    if(currEntity->name == l) return false;
    return recursiveAddEntity(currEntity->nextEntity, l, t);
}


bool movies::addEntity(MovieDB &mdb, const Label l, const EntityType t){    //Gestisco struct DataBase
    
    if(t == MOVIE){
        bool success = recursiveAddEntity(mdb->movies, l, t);
        if(success) ++mdb->numMovies;
        return success;
    }    
    bool success = recursiveAddEntity(mdb->people, l, t);
    if(success) ++mdb->numPeople;
    return success;
}

//Aggiunta Relazione:

bool horizontalAddRelationship(Arco &currRelationship, const Label l, const RelationshipType t){   //Scorre orizzontalmente
    
    if(!currRelationship){  //Inserimento in coda
        currRelationship = new Relationship;
        currRelationship->type = t;
        currRelationship->next = nullptr;
        currRelationship->adjacentLabel = l;
        return true;
    }
    if(currRelationship->adjacentLabel== l && currRelationship->type == t) return false;
    return horizontalAddRelationship(currRelationship->next, l, t);
}

bool verticalAddRelationship(const Nodo &currEntity, const Label from, const Label to, const RelationshipType t){   //Scorre verticalmente
    
    if(!currEntity) return false;
    if(currEntity->name == from){
        return horizontalAddRelationship(currEntity->adjacentList, to, t);
    }
    return verticalAddRelationship(currEntity->nextEntity, from, to, t);
}

bool movies::addRelationship(MovieDB &mdb, const Label person, const Label movie, const RelationshipType t){    //Gestisce Struct DataBase
    
    bool success = verticalAddRelationship(mdb->movies, movie, person, t);
    success = ( verticalAddRelationship(mdb->people, person, movie, t) && success);
    if(success){
        switch(t){
            case ACTED: ++mdb->numActed;
            break;
            
            case DIRECTED: ++mdb->numDirected;
            break;
            
            case PRODUCED: ++mdb->numProduced;
            break;
        }
    }
    return success;
}

//Numeri Entità/Relazioni:
int movies::numEntitiesPerType(const MovieDB &mdb, EntityType t){
    switch(t){
        case PERSON: return mdb->numPeople;
        break;
            
        case MOVIE: return mdb->numMovies;
        break;
    }
    return -1;
}

int movies::numRelationshipsPerType(const MovieDB &mdb, RelationshipType t){
    switch(t){
        case ACTED: return mdb->numActed;
        break;
            
        case DIRECTED: return mdb->numDirected;
        break;
            
        case PRODUCED: return mdb->numProduced;
        break;
    }
    return -1;
}

//Stampe:
void relationshipsPrintDB(const Arco &edge){
    if(!edge) return;
    std::cout << edge->adjacentLabel << "(" << edge->type << ")" << ", ";
    return relationshipsPrintDB(edge->next);
}

void entitiesPrintDB(const Nodo &node){
    if(!node) return;
    std::cout << node->name << " : ";
    relationshipsPrintDB(node->adjacentList);
    std::cout << endl;
    return entitiesPrintDB(node->nextEntity);
}

void printDB(const movies::MovieDB &mdb){
    //Stampa informazioni generali
    int totEntities = mdb->numPeople + mdb->numMovies;
    int totRelationships = mdb->numActed + mdb->numDirected + mdb->numProduced;
    std::cout << "Informazioni Database:" << std::endl;
    std::cout << "Numero Entità (Totali = Film + Persone): " << totEntities << " = " << mdb->numMovies << " + " << mdb->numPeople << std::endl;
     std::cout << "Numero Relazioni (Totali = Acted + Directed + Produced): " << totRelationships  << " = " << mdb->numActed << " + " << mdb->numDirected << " + " << mdb->numProduced << std::endl;
     
     std::cout << std::endl;
     std::cout << "Elenco Film seguiti da relazioni: " << std::endl;
     entitiesPrintDB(mdb->movies);
     std::cout <<" - - - - - - - - - - - - - - - - - - - - - - - - - - - "  <<  std::endl;
     std::cout << "Elenco Persone seguite da relazioni: " << std::endl;
     entitiesPrintDB(mdb->people);
}



void auxQuery(const Arco &currR, const Label l, list::List &lst, bool &found, RelationshipType t){
    if(!currR) return;
    if(currR->adjacentLabel == l && currR->type == t){  //l potrebbe non essere del tipo richiesto
        found = true;
        auxQuery(currR->next, l, lst, found, t);
        return;   
    }
    //Se è un attore, lo aggiungo a prescindere...
    if(currR->type == ACTED)list::addFront(currR->adjacentLabel, lst);
    auxQuery(currR->next, l, lst, found, t);
    //... ma se l'attore x non ha recitato in questo film, cancello dalla lista tramite backtracking
    if(!found && currR->type == ACTED) list::pop(lst);
}

list::List movies::coActors(const MovieDB &mdb, Label l){
    if(mdb->numActed == 0) return list::emptyList;    //Per risparmiare tempo con DataBase senza attori
    list::List lst = list::createEmpty();
    Nodo cursor = mdb->movies;
    bool found = false;     //Indica se è stato trovato l nel film indagato
    while(cursor){
        auxQuery(cursor->adjacentList, l, lst, found, ACTED);
        found = false;
        cursor = cursor->nextEntity;
    }
    lst = list::removeDuplicates(lst);
    return lst;
}

list::List movies::actorsProducedBy(const MovieDB &mdb, Label l){
    if(mdb->numProduced == 0) return list::emptyList;    //Per risparmiare tempo con DataBase senza prod
    list::List lst = list::createEmpty();
    Nodo cursor = mdb->movies;
    bool found = false;     //Indica se è stato trovato l nel film indagato
    while(cursor){
        auxQuery(cursor->adjacentList, l, lst, found, PRODUCED);
        found = false;
        cursor = cursor->nextEntity;
    }
    lst = list::removeDuplicates(lst);
    return lst;
}

list::List movies::actorsDirectedBy(const MovieDB &mdb, Label l){
    if(mdb->numDirected == 0) return list::emptyList;    //Per risparmiare tempo con DataBase senza prod
    list::List lst = list::createEmpty();
    Nodo cursor = mdb->movies;
    bool found = false;     //Indica se è stato trovato l nel film indagato
    while(cursor){
        auxQuery(cursor->adjacentList, l, lst, found, DIRECTED);
        found = false;
        cursor = cursor->nextEntity;
    }
    lst = list::removeDuplicates(lst);
    return lst;
}

//DA FARE
int movies::peopleBaconNumber(const MovieDB& mdb, Label l, Arco currR, int kb) {
    int minNumber = NO_BACON_NUMBER;
    Nodo cursor = mdb->people;
    while (cursor) {
        if (cursor->name == currR->adjacentLabel && !cursor->visited) {
            cursor->visited = true;
            if (l == cursor->name) return kb;
            Arco edgeCursor = cursor->adjacentList;
            while (edgeCursor) {
                int curNumber = moviesBaconNumber(mdb, l, edgeCursor, kb + 1);
                if (curNumber < minNumber) minNumber = curNumber;
                edgeCursor = edgeCursor->next;
            }
            return minNumber;
        }
        cursor = cursor->nextEntity;
    }
    return minNumber;

}

int movies::moviesBaconNumber(const MovieDB& mdb, Label l, Arco currR, int kb) {
    int minNumber = NO_BACON_NUMBER;
    Nodo cursor = mdb->movies;
    while (cursor) {
        if (cursor->name == currR->adjacentLabel && !cursor->visited) {
            cursor->visited = true;
            Arco edgeCursor = cursor->adjacentList;
            while (edgeCursor) {
                int curNumber = peopleBaconNumber(mdb, l, edgeCursor, kb);
                if (curNumber < minNumber) minNumber = curNumber;
                edgeCursor = edgeCursor->next;
            }
            return minNumber;
        }
        cursor = cursor->nextEntity;
    }
    return minNumber;
}

int movies::BaconNumber(const MovieDB& mdb, Label l) {
    int minNumber = NO_BACON_NUMBER;
    /*
    int curNumber = peopleBaconNumber(mdb, l, mdb->people, 0);
    if (curNumber < minNumber) minNumber = curNumber;
    */

    Nodo cursor = mdb->people;
    while (cursor) {
        if (cursor->name == "Kevin Bacon") {
            cursor->visited = true;
            if (l == cursor->name) return 0;
            Arco edgeCursor = cursor->adjacentList;
            while (edgeCursor) {
                int curNumber = moviesBaconNumber(mdb, l, edgeCursor, 1);
                if (curNumber < minNumber) minNumber = curNumber;
                edgeCursor = edgeCursor->next;
            }
        }
        cursor = cursor->nextEntity;
    }


    //Reset visited
    cursor = mdb->people;
    while (cursor) {    //Persone
        cursor->visited = false;
        cursor = cursor->nextEntity;
    }
    cursor = mdb->movies;
    while (cursor) {    //Film
        cursor->visited = false;
        cursor = cursor->nextEntity;
    }
    return minNumber;
}





