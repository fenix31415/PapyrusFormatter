#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

const std::string one_shift = "  ";

class Node {
public:
    std::vector<Node*> ch;
    std::string value;

public:
    Node() : value(), ch() {
        //std::cout<<"creating parent Node"<<std::endl;
    };
    Node(std::string s) : value(s), ch() {
        //std::replace(s.begin(), s.end(), '\n', '\\');
        //std::replace(s.begin(), s.end(), '\r', '\\');
        //std::cout<<"creating Node '"<<s<<"'"<<std::endl;
    };

    virtual std::string text() const {
        return value;
    }

    virtual void addch(Node* c) {
        ch.push_back(c);
    }

    Node* addch(std::vector<Node*>&& args) {
        ch.insert(ch.end(), args.begin(), args.end());
        return this;
    }

    virtual std::string gettype() const {
        return "Node";
    }

    virtual std::string print_(const std::string& shift = "") {
        std::string ans = "(" + gettype() + ": " + text() + " ";
        for (int i = ch.size() - 1; i >= 0; --i) {
            ans += "[" + ch[i]->print_(shift) + "], ";
        }
        ans += ")";
        return ans;
    }

    void print(const char* name) {
        if (!name) {
            std::cout << print_();
        }
        else {
            std::ofstream outfile(name);
            outfile << print_();
            outfile.close();
        }
    }

    void appendText(std::string& ans, const Node* src, const std::string& enhL = "", const std::string& enhR = "") const {
        std::string tmp = src->text();
        if (tmp != "")
            ans += enhL + tmp + enhR;
    }

    void appendStr(std::string& ans, const std::string& src, const std::string& enhL = "", const std::string& enhR = "") const {
        if (src != "")
            ans += enhL + src + enhR;
    }

    void appendValues(std::string& ans, const Node* src, const std::string& enhL = "", const std::string& enhR = "") const {
        std::string tmp = "";
        for (int i = src->ch.size() - 1; i >= 0; --i) {
            tmp += enhL + src->ch[i]->text() + enhR;
        }
        if (tmp != "")
            ans += tmp;
    }

    void appendPrint(std::string& ans, bool newline = true, bool addShift = true, const std::string& shift = "") {
        std::string r, tmp;
        if (newline) r = "\n";
        tmp = print_(shift);
        if (tmp == "") return;
        if (addShift)
            ans += shift + tmp + r;
        else
            ans += tmp + r;
    }

    // for NodeListEndl
    virtual bool isEndls() const {
        return false;
    }

    // for docstring
    virtual bool incShift() const {
        return true;
    }

    virtual ~Node() {
        //std::cout<<"deleting '"<<value<<"'"<<std::endl;
        for (int i = 0; i < ch.size(); ++i) {
            //std::cout<<"    deleting child "<<i<<" '"<<ch[i]->value<<"'"<<std::endl;
            delete ch[i];
        }
    }
};

class NodeConst : public Node {
private:
    void ltrim(std::string& s) const {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
            }));
    }
    void rtrim(std::string& s) const {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
            }).base(), s.end());
    }
    void trim(std::string& s) const {
        ltrim(s);
        rtrim(s);
    }
public:
    virtual std::string print_(const std::string& shift = "") {
        return value;
    }

    virtual std::string gettype() const {
        return "NodeConst";
    }
    NodeConst(std::string s) : Node(s) {

    };
};

enum WSData {
    Data = 0x01,
    NewLn = 0x02
};

class NodeWS : public Node {
public:
    unsigned char ops;

    virtual std::string print_(const std::string& shift = "") {
        return text();
    }

    virtual std::string gettype() const {
        return "NodeWS";
    }

    bool isContainsData() {
        return ops & WSData::Data;
    }

    bool isContainsNewLn() {
        return ops & WSData::NewLn;
    }

    unsigned char getOps() {
        return ops;
    }

    NodeWS(std::string s, unsigned char flags) : Node(s), ops(flags) { };
};
