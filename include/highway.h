#ifndef HIGHWAY_H
#define HIGHWAY_H

#include <string>
#include <vector>
#include <iostream>

class Highway {
public:
    enum class NodeType { Varco, Svincolo };

    struct Node {
        double km;        // distanza
        NodeType type;    // varco o svincolo
        int id;           // numero identificativo
    };

    Highway();

    bool loadFromFile(const std::string& filepath); // carica file
    bool validate(std::ostream& err) const;         // controllo dai dati
    void print(std::ostream& out) const;            // stampa nodi

    const std::vector<Node>& getNodes() const;      // lista nodi


    std::vector<double> getGatesKm() const;         // lista km dei varchi
    std::vector<double> getRampsKm() const;         // lista km degli svincoli

private:
    std::vector<Node> nodes_; // vettore nodi

    static bool parseLine(const std::string& line, double& km_out, NodeType& type_out);
    static std::string typeToString(NodeType t);

    void assignIdsSortedByKm();
    std::vector<Node> sortedByKm() const;

    int countType(NodeType t) const;
};

#endif

