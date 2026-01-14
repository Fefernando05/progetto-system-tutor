#ifndef HIGHWAY_H
#define HIGHWAY_H

#include <string>
#include <vector>
#include <iostream>

class Highway {
public:
    enum class NodeType { Varco, Svincolo };

    struct Node {
        double km;  //distanza
        NodeType type;  // varco o svincolo
        int id; //numero identificativo
    };

    Highway();

    bool loadFromFile(const std::string& filepath); //carica file
    bool validate(std::ostream& err) const; //controllo dai dati
    void print(std::ostream& out) const; //stampa nodi

    const std::vector<Node>& getNodes() const; //da la lista dei nodi

private:
    std::vector<Node> nodes_; //vettori con i nodi

    static bool parseLine(const std::string& line, double& km_out, NodeType& type_out); //prende km e tipo di nodo
    static std::string typeToString(NodeType t); //converto v o s in stringa

    void assignIdsSortedByKm(); //da il nimero ai nodi dopo averli ordianati per distanza
    std::vector<Node> sortedByKm() const; //da una copia dei nodi ordinati

    int countType(NodeType t) const; //conta quanti nodi di un tipo ci sono
};

#endif