#include <fstream>
#include <iostream>
#include "movies.h"

using namespace std;

const string menu_text = R"(
Inserisci l'operazione scelta e di seguito gli operandi richiesti.
I nomi vengono sempre letti fino a fine linea.

a: leggi un database (2 operandi: nome_file_nodi nome_file_archi)
b: aggiungi un nodo corrispondente a una persona (1 operando: nome)
c: aggiungi un nodo corrispondente a un film (1 operando: nome)
d: stampa il contenuto del database
e: numero di persone
f: numero di film
g: numero di relazioni "ha recitato in"
h: numero di relazioni "ha diretto"
i: numero di relazioni "ha prodotto"
j: stampa gli attori che hanno recitato in almeno un film con un attore
   (1 operando: nome)
k: stampa gli attori che hanno recitato in film con un dato produttore
   (1 operando: nome)
l: stampa gli attori che hanno recitato in film con un dato regista
   (1 operando: nome)
m: numero di Bacon di un attore (1 operando: nome)
n: aggiungi un nodo corrispondente a una relazione
   (3 operandi separati da ':', persona:film:relazione, come nei file di input)
)";
const char last_choice = 'n';  // carattere che rappresenta l'ultima scelta
const string prompt = "\ndigita p per stampare il menu, q per terminare\n> ";

// variabile globale usata per decidere se stampare i prompt
bool quiet;

// ------ FUNZIONI AUSILIARIE -------

// Visualizza il menu con le scelte possibili e chiede
// all'utente di immetterne una. 
// Continua a ripresentare il menu finché l'utente non
// immette una scelta valida. Ritorna la scelta fatta ed esce se
// viene immesso 'q' o EOF.
char menu()
{
  while (true) {
    if (!quiet)
      cout << prompt;
    char ch;
    if (!(cin >> ch) || ch == 'q')
      exit(0);
    if (ch == 'p') {
      cout << menu_text;
      continue;
    }
    if (ch < 'a' || ch > last_choice) {
      cerr << "scelta non valida" << endl;
      continue;
    }
    return ch;
  }
}

// legge una stringa
string read_string()
{
    string res;
    cin >> res;
    if (!cin)
        throw runtime_error("fine file quando una stringa è attesa");
    return res;
}

// rimuove spazi all'inizio e alla fine della stringa
string strip(string s)
{
    int i = 0;
    while (isspace(s[i]))
        i++;
    int j = s.size() - 1;
    while (isspace(s[j]) && j > i)
        j--;
    return s.substr(i, j - i + 1);
}


// legge un'etichetta da standard input
string read_label()
{
    string label;
    getline(cin, label);
    if (!cin)
        throw runtime_error("fine file quando cerco di leggere un'etichetta");
    return strip(label);
}

// apre un file e solleva eccezione in caso di errore
ifstream open(string file_name)
{
    ifstream res = ifstream(file_name);
    if (!res)
        throw runtime_error("non posso aprire " + file_name);
    return res;
}

// dice se una stringa è fatta solo di spazi
bool all_blank(string s)
{
    for (unsigned int i=0; i < s.size(); i++)
        if (!isspace(s[i]))
            return false;
    return true;
}

// cerca la posizione del separatore ':'
int find_separator(string s, int pos)
{
    string::size_type sep = s.find(':', pos);
    if (sep == string::npos)
        throw runtime_error("separatore mancante");
    return sep;
}

// aggiunge nodi da un file con quel nome
void add_nodes(movies::MovieDB &mdb, string file_name)
{
    ifstream ifs = open(file_name);
    while (true) {
        string line;
        getline(ifs, line);
        if (!ifs)
            break;
        if (all_blank(line))
            continue;
        int sep = find_separator(line, 0);
        movies::Label l = strip(line.substr(0, sep));
        string type_s = strip(line.substr(sep + 1, string::npos));
        movies::EntityType t;
        if (type_s == "persona")
            t = movies::PERSON;
        else if (type_s == "film")
            t = movies::MOVIE;
        else
            throw runtime_error("tipo di nodo \"" + type_s +  "\" non valido");
        if (!movies::addEntity(mdb, l, t))
            throw runtime_error("nodo " + l + " già presente");
    }
    ifs.close();
}

void add_edge_from_line(movies::MovieDB &mdb, string line)
{
    int sep1 = line.find(":", 0);
    int sep2 = line.find(":", sep1 + 1);
    string type_s = strip(line.substr(sep2 + 1, string::npos));
    movies::RelationshipType t;
    if (type_s == "recitato_in")
        t = movies::ACTED;
    else if (type_s == "diretto")
        t = movies::DIRECTED;
    else if (type_s == "prodotto")
        t = movies::PRODUCED;
    else
        throw runtime_error("tipo di arco \"" + type_s + "\" non valido");
    movies::Label person = strip(line.substr(0, sep1));
    movies::Label movie = strip(line.substr(sep1 + 1, sep2 - sep1 - 1));
    if (!movies::addRelationship(mdb, person, movie, t))
        throw runtime_error("inserimento dell'arco " + strip(line) + " fallito");    
}


// aggiunge archi da un file con quel nome
void add_edges(movies::MovieDB &mdb, string file_name)
{
    ifstream ifs = open(file_name);
    while (true) {
        string line;
        getline(ifs, line);
        if (!ifs)
            break;
        if (all_blank(line))
            continue;
        add_edge_from_line(mdb, line);
    }
    ifs.close();
}

// ------ MAIN --------

int main(int argc, char *argv[]) {

  quiet = argc >= 2 && argv[1][0] == 'q';

  movies::MovieDB mdb = movies::createEmpty();

  if (!quiet)

    cout << menu_text;

  // ciclo sulla scelta
  while (true) {
    char ch = menu();
    
    string nodes_filename, edges_filename;
    string line;
    movies::Label label;

    // lettura parametri
    try  {
        switch (ch) {
            case 'a':
                nodes_filename = read_string();
                edges_filename = read_string();
                break;
            
            case 'b':
            case 'c':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
                label = read_label();
                break;
            
            case 'n':
                getline(cin, line);
                if (!cin)
                    throw runtime_error("fine del file quando una riga è necessaria");
                break;                
                
        }
    }
    catch (runtime_error &e) {
        cerr << "errore nella lettura dei parametri: " << e.what() << endl;
        continue;
    }

    bool success;
    list::List lst;

    // esecuzione del comando
    try  {
        int bacon_number;

        switch (ch) {
            case 'a':
                add_nodes(mdb, nodes_filename);
                add_edges(mdb, edges_filename);
                break;
            
            case 'b':
                success = movies::addEntity(mdb, label, movies::PERSON);
                break;
            
            case 'c':
                success = movies::addEntity(mdb, label, movies::MOVIE);
                break;
            
            case 'd':
                printDB(mdb);
                break;
            
            case 'e':
                cout << movies::numEntitiesPerType(mdb, movies::PERSON) << endl;
                break;
            
            case 'f':
                cout << movies::numEntitiesPerType(mdb, movies::MOVIE) << endl;
                break;

            case 'g':
                cout << movies::numRelationshipsPerType(mdb, movies::ACTED) << endl;
                break;
            
            case 'h':
                cout << movies::numRelationshipsPerType(mdb, movies::DIRECTED) << endl;
                break;
            
            case 'i':
                cout << movies::numRelationshipsPerType(mdb, movies::PRODUCED) << endl;
                break;
            
            case 'j':
                lst = movies::coActors(mdb, label);
                break;
            
            case 'k':
                lst = movies::actorsProducedBy(mdb, label);
                break;
            
            case 'l':
                lst = movies::actorsDirectedBy(mdb, label);
                break;
            
            case 'm':
                bacon_number = movies::BaconNumber(mdb, label);
                if (bacon_number == movies::NO_BACON_NUMBER)
                    cout << "no bacon number" << endl;
                else
                    cout << bacon_number << endl;
                break;
            
            case 'n':
                add_edge_from_line(mdb, line);
                break;
        }
    }
    catch (runtime_error &e) {
        cerr << "errore eseguendo il comando: " << e.what() << endl;
        continue;
    }

    // eventuale output
    switch (ch) {
        case 'b':
        case 'c':
            if (success)
                cout << "operazione riuscita" << endl;
            else
                cout << "operazione fallita" << endl;
            break;
        
        case 'j':
        case 'k':
        case 'l':
            printList(lst);
            list::clear(lst);
    }

  }  // while
}  // main
