#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string> 

using namespace std; 
 
namespace list{

    // TIPI:
    typedef string Elem;      // tipo base, compatibile con il tipo Label dell'grafo 

    const Elem emptyElem = "$#$#$";
    

    struct list{
	    Elem elem;
	    list* next;
    };
    typedef list * List;
    
    const List emptyList = nullptr;
    
    // FUNZIONI:
    
    void clear(List&);    // Smantella la lista
    // void set(int, Elem, List&); /* modifica l'elemento in posizione pos */
    // void add(int, Elem, List&); /* inserisce l'elemento in posizione pos, shiftando a destra gli altri elementi */
    // void addBack(Elem, List&);  /* inserisce l'elemento alla fine della lista */
    void addFront(Elem, List&); // inserisce l'elemento all'inizio della lista
    Elem pop( List&); // cancella e restituisce l'elemento in fronte
    // Elem get(int, const List&);	/* restituisce l'elemento in posizione pos */
    List createEmpty();
    bool isEmpty(const List&);
    // int size(const List&);
    void printList(const List&);
    bool isPresent(const List& l, Elem e);
    List removeDuplicates(List lst);
}


