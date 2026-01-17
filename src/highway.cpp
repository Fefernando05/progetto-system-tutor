//Alessandro Perin 2137431
#include "Highway.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>

// Evita errori a parseLine nel caso nel file di testo ci siano righe vuote
static bool isBlank(const std::string& line) {
    for (char c : line) {
        if (!std::isspace(static_cast<unsigned char>(c)))  //controlla se c non è uno spazio vuoto
            return false;
    }
    return true;
}

Highway::Highway() {}

const std::vector<Highway::Node>& Highway::getNodes() const {
    return nodes_;
}

std::string Highway::typeToString(NodeType t) {
    return (t == NodeType::Varco ? "V" : "S");
}

int Highway::countType(NodeType t) const {
    int cnt = 0;
    for (const Node& n : nodes_) {
        if (n.type == t)
            cnt++;
    }
    return cnt;
}

std::vector<Highway::Node> Highway::sortedByKm() const {
    std::vector<Node> tmp = nodes_;
    std::sort(tmp.begin(), tmp.end(), [](const Node& a, const Node& b) {  //funzione per mettere varchi prima di svincoli
        if (a.km != b.km) return a.km < b.km;                            // se stessa distanza
        return static_cast<int>(a.type) < static_cast<int>(b.type);
    });
    return tmp;
}

bool Highway::parseLine(const std::string& line, double& nodeKm, NodeType& nodeType) {
    std::istringstream iss(line); //trasformo la stringa in uno stream per leggerla piu facilmente
    double km_val;
    std::string type_lettera;

    if (!(iss >> km_val >> type_lettera))
        return false;

    if (type_lettera.size() != 1)
        return false;

    char t = type_lettera[0];
    if (t == 'V' || t == 'v')
        nodeType = NodeType::Varco;
    else if (t == 'S' || t == 's')
        nodeType = NodeType::Svincolo;
    else
        return false;

    nodeKm = km_val;
    return true;
}

void Highway::assignIdsSortedByKm() {
    std::vector<size_t> idx(nodes_.size());
    for (size_t i = 0; i < idx.size(); i++)
        idx[i] = i;

    std::sort(idx.begin(), idx.end(), [&](size_t a, size_t b) {
        if (nodes_[a].km != nodes_[b].km) return nodes_[a].km < nodes_[b].km;
        return static_cast<int>(nodes_[a].type) < static_cast<int>(nodes_[b].type);
    });

    int nextVarcoId = 1;
    int nextSvincoloId = 1;

    for (size_t k = 0; k < idx.size(); k++) {
        size_t i = idx[k];
        if (nodes_[i].type == NodeType::Varco)
            nodes_[i].id = nextVarcoId++;
        else
            nodes_[i].id = nextSvincoloId++;
    }
}

bool Highway::loadFromFile(const std::string& filepath) {
    nodes_.clear();

    std::ifstream fin(filepath);
    if (!fin)
        return false;

    std::string line;
    while (std::getline(fin, line)) {
        if (isBlank(line))
            continue;

        double km_val = 0.0;
        NodeType t;
        if (!parseLine(line, km_val, t))
            continue;

        Node n;
        n.km = km_val;
        n.type = t;
        n.id = 0;
        nodes_.push_back(n);
    }

    assignIdsSortedByKm();
    return true;
}

bool Highway::validate(std::ostream& err) const {
    bool ok = true;

    if (nodes_.empty()) {
        err << "Errore: Highway.txt non contiene righe valide.\n";
        return false;
    }

    int nV = countType(NodeType::Varco);
    int nS = countType(NodeType::Svincolo);

    if (nV < 2) {
        err << "Errore: devono essere presenti almeno due varchi (V). Trovati: " << nV << "\n";
        ok = false;
    }

    if (nS < 2) {
        err << "Errore: devono essere presenti almeno due svincoli (S).\n";
        ok = false;
    }

    std::vector<Node> srt = sortedByKm();

    double firstVarcoKm = 0.0;
    double lastVarcoKm = 0.0;
    bool hasVarco = false;

    for (const Node& n : srt) {
        if (n.type == NodeType::Varco) {
            if (!hasVarco) firstVarcoKm = n.km;
            lastVarcoKm = n.km;
            hasVarco = true;
        }
    }

    if (hasVarco) {
        bool hasSvincoloBefore = false;
        bool hasSvincoloAfter = false;

        for (const Node& n : srt) {
            if (n.type != NodeType::Svincolo) continue;
            if (n.km < firstVarcoKm) hasSvincoloBefore = true;
            if (n.km > lastVarcoKm) hasSvincoloAfter = true;
        }

        if (!hasSvincoloBefore) {
            err << "Errore: manca uno svincolo prima del primo varco.\n";
            ok = false;
        }
        if (!hasSvincoloAfter) {
            err << "Errore: manca uno svincolo dopo l'ultimo varco.\n";
            ok = false;
        }

        const double MIN_DIST = 1.0;
        for (const Node& v : srt) {
            if (v.type != NodeType::Varco) continue;

            for (const Node& s : srt) {
                if (s.type != NodeType::Svincolo) continue;

                double d = std::fabs(v.km - s.km);
                if (d < MIN_DIST) {
                    err << "Errore: distanza < 1 km tra varco km "
                        << v.km << " e svincolo km " << s.km << "\n";
                    ok = false;
                }
            }
        }
    }

    for (const Node& n : nodes_) {
        if (n.km < 0.0) {
            err << "Errore: distanza negativa non ammessa.\n";
            ok = false;
        }
    }

    return ok;
}

void Highway::print(std::ostream& out) const {
    std::vector<Node> srt = sortedByKm();

    out << "Autostrada (ordinata per km)\n";
    for (const Node& n : srt) {
        if (n.type == NodeType::Varco)
            out << "Varco " << n.id << "  km " << n.km << "\n";
        else
            out << "Svincolo " << n.id << "  km " << n.km << "\n";
    }
}


std::vector<double> Highway::getGatesKm() const {
    std::vector<double> out;
    for (const Node& n : nodes_) {
        if (n.type == NodeType::Varco)
            out.push_back(n.km);
    }
    std::sort(out.begin(), out.end());
    return out;
}

std::vector<double> Highway::getRampsKm() const {
    std::vector<double> out;
    for (const Node& n : nodes_) {
        if (n.type == NodeType::Svincolo)
            out.push_back(n.km);
    }
    std::sort(out.begin(), out.end());
    return out;
}
