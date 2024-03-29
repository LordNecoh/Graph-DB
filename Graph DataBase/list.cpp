#include "list.h"

using namespace list;


/****************************************************************/

void clearAux(List& l){
	if(isEmpty(l)) return;
	clearAux(l->next);
	delete l;  
}
void list::clear(List& l){
    if(isEmpty(l)) return;
	clearAux(l->next);
	delete l;
	l = nullptr; 
}


/****************************************************************/
void list::addFront(Elem e, List& l) // aggiunge e in testa
{
   List aux = new list;
   aux->elem = e;
   aux->next = l;
   l = aux;
}

/****************************************************************/

List list::createEmpty()
{
   List l = nullptr;
   return l;
}


/****************************************************************/
bool list::isEmpty(const List& l)
{
   return (l==emptyList);
}



/****************************************************************/
void list::printList(const List& l)
{
	if(isEmpty(l)) return;
	cout<<l->elem<<endl;
	return printList(l->next);
}
/****************************************************************/
list::Elem list::pop(List& l)
{
   if(l){
      List temp=l->next;
      delete l;
      l=temp;
   }
   return emptyElem;
}

/****************************************************************/
bool list::isPresent(const List& l, Elem e){
    List auxL = l;
    while(auxL){
        if(auxL->elem == e) return true;
        auxL = auxL->next;
    }
    return false;
}

/****************************************************************/

list::List list::removeDuplicates(List lst){
    List newList = createEmpty();
    List cursor = lst;
    while(!isEmpty(cursor)){
        if(!isPresent(newList, cursor->elem)) addFront(cursor->elem, newList);
        cursor = cursor->next;
    }
    clear(lst);
    return newList;
}


