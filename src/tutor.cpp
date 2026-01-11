/* * Main del sistema tutor autostradale. 
* Permette all'utente di avanzare nel tempo (set_time), resettare (reset) 
* e visualizzare statistiche (stats). * * Autore: <Beatrice Scanagatta> 
*/ 
#include <iostream> 
#include <string> 
#include <stdexcept> 
#include <cctype> 
#include "../include/TutorSystem.h" 
using namespace std; 
/* 
* Converte l'argomento di set_time in secondi. 
* Formati ammessi: 
* - "20" -> 20 secondi 
* - "20m" -> 20 minuti 
*/ 
static double parseDeltaSeconds(const string& token) 
{ 
    if(token.empty()) 
      throw invalid_argument("[ERRORE] Argomento mancante per set_time"); 

    // Caso minuti: termina con 'm' 
    if(token.back() == 'm') 
    { 
      if(token.size() == 1) 
        throw invalid_argument("[ERRORE] Argomento non valido per set_time"); 
      
      string numberPart = token.substr(0, token.size() - 1); 
      
      // controllo caratteri (solo cifre o punto decimale) 
      for(char c : numberPart) 
      { 
        if(!isdigit(static_cast<unsigned char>(c)) && c != '.') 
          throw invalid_argument("[ERRORE] Argomento non valido per set_time"); 
      } 
      double minutes = stod(numberPart); 
      if(minutes <= 0.0) 
        throw invalid_argument("[ERRORE] L'avanzamento temporale deve essere positivo"); 
      
      return minutes * 60.0; 
    } 
    // Caso secondi: numero puro 
    for(char c : token) 
    { 
      if(!isdigit(static_cast<unsigned char>(c)) && c != '.') 
        throw invalid_argument("[ERRORE] Argomento non valido per set_time"); 
    } 
    double seconds = stod(token); 
    if(seconds <= 0.0) 
      throw invalid_argument("[ERRORE] L'avanzamento temporale deve essere positivo"); 
  
    return seconds; 
} 
int main() 
{ 
    try 
    { 
      // File di input richiesti dal testo: Data/Highway.txt e Data/Passages.txt 
      TutorSystem tutor("Data/Highway.txt", "Data/Passages.txt"); 
      cout << "Sistema Tutor avviato. Comandi disponibili: \n- set_time <istante> (es. set_time 20 oppure set_time 20m) \n- reset \n - stats\n"; 
      string line; 
      while(true)  
      { 
        cout << ">> "; 
        
        // esce su EOF/errore 
        if(!getline(cin, line)) 
          break; 

        // ignora righe vuote 
        if(line.empty()) 
          continue; 
        
        // parsing comando: prima parola + resto (se presente) 
        string cmd, arg; 
        size_t spacePos = line.find(' '); 
        if(spacePos == string::npos) 
        { 
          cmd = line; 
        } 
        else 
        { 
          cmd = line.substr(0, spacePos); 
          // rimuove spazi multipli prima dell'argomento 
          size_t start = line.find_first_not_of(' ', spacePos); 
          if(start != string::npos) 
            arg = line.substr(start); 
        } 
        try 
        { 
          if(cmd == "set_time") 
          { 
            double deltaSeconds = parseDeltaSeconds(arg); 
            // Porta il sistema a un nuovo istante (successivo al corrente), 
            // analizzando i transiti nel tempo intercorso. 
            tutor.set_time(deltaSeconds); 
            // Stampa elenco veicoli da sanzionare per questo avanzamento. 
            // (Interfaccia pensata per evitare di duplicare logica nel main) 
            tutor.print_last_sanctions(cout);
          } 
          else if(cmd == "reset") 
          { 
            tutor.reset(); 
            cout << "Sistema resettato.\n"; 
          } 
          else if(cmd == "stats") 
          { 
            tutor.print_stats(cout); 
          } 
          else 
          { 
            cout << "[ERRORE] Comando non valido\n"; 
          }
        } 
        catch(exception& e) 
        {
          cerr << "" << e.what() << endl; 
        } 
      } 
      return 0; 
    } 
    catch(exception& e) 
    { 
      cerr << "" << e.what() << endl; 
      return 1; 
    } 
}
