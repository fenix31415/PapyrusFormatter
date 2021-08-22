#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>

const std::string one_shift = "  ";

class Node {
public:
    std::vector<Node*> ch;
    std::string value;

public:
    Node() = default;
    Node(std::string s) : value(s), ch() { };
    Node(const Node& other) = delete;
    Node& operator=(const Node& other) = delete;

    virtual ~Node() {
        for (int i = 0; i < ch.size(); ++i) {
            delete ch[i];
        }
    }

    virtual std::string text() const { return value; }

    virtual void addch(Node* c) { ch.push_back(c); }

    Node* addch(const std::vector<Node*>& args) {
        ch.insert(ch.end(), args.begin(), args.end());
        return this;
    }

    virtual void print_(std::ostream& out, const std::string& shift = "") {
        out << "(" << gettype() << ": " << text() << " ";
        for (int i = (int)ch.size() - 1; i >= 0; --i) {
            out << "[";
            ch[i]->print_(out, shift);
            out << "], ";
        }
        out << ")";
    }

    void print__(std::ostream& out) const {
        out << value;
    }

    void print(const char* name) {
        if (!name) {
            std::stringstream ss;
            print_(ss);
            std::cout << ss.str();
        }
        else {
            std::ofstream outfile(name);
            print_(outfile);
            outfile.close();
        }
    }

    void appendText(std::ostream& out, const Node* src, const std::string& enhL = "", const std::string& enhR = "") const {
        std::string tmp = src->text();
        if (tmp != "")
            out << enhL << tmp << enhR;
    }

    void appendValues(std::ostream& out, const Node* src, const std::string& enhL = "", const std::string& enhR = "") const {
        for (int i = (int)src->ch.size() - 1; i >= 0; --i) {
            out << enhL << src->ch[i]->text() << enhR;
        }
    }

    virtual std::string gettype() const { return "Node"; }

    // for NodeListEndl
    virtual bool isEndls() const { return false; }

    // for docstring
    virtual bool incShift() const { return true; }
};

class NodeConst : public Node {
public:
    void print_(std::ostream& out, const std::string& shift = "") override { print__(out); }

    std::string gettype() const override { return "NodeConst"; }
    NodeConst(std::string s) : Node(s) { };
};

class NodeDocstring : public Node {
public:
    void print_(std::ostream& out, const std::string& shift = "") override { print__(out); }

    std::string gettype() const override { return "NodeDocstring"; }
    bool incShift() const override { return false; }
    NodeDocstring(std::string s) : Node(s) { };
};

enum WSData {
    Data = 0x01,
    NewLn = 0x02
};

class NodeWS : public Node {
public:
    unsigned char ops;

    void print_(std::ostream& out, const std::string& shift = "") { return print__(out); }

    std::string gettype() const override { return "NodeWS"; }

    bool isContainsData() const { return ops & WSData::Data; }

    bool isContainsNewLn() const { return ops & WSData::NewLn; }

    NodeWS(std::string s, unsigned char flags) : Node(s), ops(flags) { };
};
