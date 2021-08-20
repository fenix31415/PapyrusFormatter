#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "UtilsLight.h"

// auto/autoreadonly * type/name/init
int maxprops[2][3];

enum TrimOptions :unsigned char {
    LForceWS = 0x1,  // a _d_ +; a _d_ [
    LForceNL = 0x2,  // unused
    LKeep = 0x3,  // import a _d; import a _\n_d_
    LRemove = 0x4,  // _ scriptname
    LMask = 0x7,

    RForceWS = 0x8,  // same
    RForceNL = 0x10, // import a _d_ -> import a _d\n
    RKeep = 0x18, // _d\n scriptname
    RRemove = 0x20, // a _d_ ) -> a _d )
    RMask = 0x38,

    SRemove = 0x40,  // a _ [
    SForceWS = 0x80,  // int _ a; a _ +
    SForceNL = 0xC0,  // import a _
    SMask = 0xC0
};

class TrimOptionsPresets {
public:
    const static unsigned char oneline = TrimOptions::SForceWS | TrimOptions::LForceWS | TrimOptions::RForceWS;
    const static unsigned char newline = TrimOptions::SForceNL | TrimOptions::LKeep | TrimOptions::RForceNL;
    const static unsigned char newlone = TrimOptions::SForceNL | TrimOptions::LForceNL | TrimOptions::RKeep;
    const static unsigned char mb_line = TrimOptions::SRemove | TrimOptions::LForceWS | TrimOptions::RForceWS;
    const static unsigned char l__line = TrimOptions::SRemove | TrimOptions::LRemove | TrimOptions::RKeep;
    const static unsigned char l_enh = TrimOptions::SRemove | TrimOptions::LRemove | TrimOptions::RForceWS;
    const static unsigned char r_enh = TrimOptions::SRemove | TrimOptions::LForceWS | TrimOptions::RRemove;
    const static unsigned char remove = TrimOptions::SRemove | TrimOptions::LRemove | TrimOptions::RRemove;
};

class NodeWSS : public Node {
private:
    std::pair<std::string, unsigned char> GetTrimStrS(unsigned char ops) {
        switch (ops) {
        case TrimOptions::SRemove:  return { "", 0 };
        case TrimOptions::SForceWS: return { " ", 0 };
        case TrimOptions::SForceNL: return { "\n", WSData::NewLn };
        default:                    return { "?s?", 0 };
        }
    }
    // using with R: ops >>= 3
    std::pair<std::string, unsigned char> GetTrimStrLR(unsigned char ops, bool hasNewLn) {
        const std::pair<std::string, unsigned char> empty = { "", 0 };
        const std::pair<std::string, unsigned char> space = { " ", 0 };
        const std::pair<std::string, unsigned char> newln = { "\n", WSData::NewLn };
        const std::pair<std::string, unsigned char> dunno = { "?LR?", 0 };

        switch (ops) {
        case TrimOptions::LRemove:  return empty;
        case TrimOptions::LForceWS: return space;
        case TrimOptions::LForceNL: return newln;
        case TrimOptions::LKeep:    return hasNewLn ? newln : space;
        default:                    return dunno;
        }
    }
    bool handleBorders(NodeWS* bor, unsigned char ops, bool left) {
        unsigned char ops_ = left ? (ops & TrimOptions::LMask) : ((ops & TrimOptions::RMask) >> 3);
        if (!bor->isContainsData()) {
            auto to_add = GetTrimStrLR(ops_, bor->isContainsNewLn());
            if (to_add.first != "") {
                bor->value = to_add.first;
                bor->ops = to_add.second;
            }
            else {
                return true;
            }
        }
        else {
            auto to_add = GetTrimStrLR(ops_, false);
            if (left)
                ch.insert(ch.begin(), new NodeWS(to_add.first, to_add.second));
            else
                ch.push_back(new NodeWS(to_add.first, to_add.second));
        }
        return false;
    }
    void trim(unsigned char ops) {
        NodeWS* m = ch.size() ? (NodeWS*)ch[0] : 0;
        if (!ch.size() || ch.size() == 1 && !m->isContainsData()) {
            auto to_add = GetTrimStrS(ops & TrimOptions::SMask);
            if (to_add.first != "") {
                if (!ch.size())
                    ch.push_back(new NodeWS(to_add.first, to_add.second));
                else {
                    m->value = to_add.first;
                    m->ops = to_add.second;
                }
            }
            else {
                if (ch.size()) {
                    delete ch[0];
                    ch.pop_back();
                }
            }
        }
        else {
            if (handleBorders(m, ops, true)) {
                delete ch[0];
                ch.erase(ch.begin());
            }
            if (handleBorders((NodeWS*)ch[ch.size() - 1], ops, false)) {
                delete ch[ch.size() - 1];
                ch.pop_back();
            }
        }
    }

public:
    void appendl(Node* v) {
        NodeWS* n = (NodeWS*)v;
        if (!n->isContainsData() && ch.size() > 0) {
            if (auto m = (NodeWS*)ch[0]; !m->isContainsData()) {
                m->value = n->value + m->value;
                m->ops = m->ops | (n->ops & WSData::NewLn);
                return;
            }
        }
        ch.insert(ch.begin(), v);
    }
    void appendlNode(Node* v) {
        for (auto& i : v->ch) {
            appendl(i);
        }
    }
    void appendr(Node* v) {
        NodeWS* n = (NodeWS*)v;
        if (!n->isContainsData() && ch.size() > 0) {
            if (auto m = (NodeWS*)ch[ch.size() - 1]; !m->isContainsData()) {
                m->value += n->value;
                m->ops = m->ops | (n->ops & WSData::NewLn);
                return;
            }
        }
        ch.push_back(v);
    }

    std::string printWS(unsigned char ops) {
        trim(ops);

        std::string ans = "";
        for (int i = 0; i < ch.size(); ++i) {
            ans += ch[i]->print_();
        }
        return ans;
    }

    virtual std::string gettype() const {
        return "NodeWSS";
    }
    NodeWSS() : Node() { };
};

class NodeList : public Node {
private:
    bool withStart;
public:
    virtual std::string print_(const std::string& shift = "") {
        std::string ans, tmp;
        for (int i = 0; i < ch.size(); ++i) {
            tmp = ch[i]->print_();
            if (tmp == "")
                continue;
            if (withStart || i != 0)
                ans += shift;
            ans += tmp;
        }
        return ans;
    }

    virtual std::string gettype() const {
        return "NodeList";
    }

    NodeList(bool withStart = true) : Node(), withStart(withStart) { };
};

class NodeConstList : public Node {
private:
    bool withStart;
    const std::vector<std::string> args;
    const std::vector<std::string> enhs;
public:
    virtual std::string print_(const std::string& shift = "") {
        std::string ans = enhs.size() ? enhs[0] : "";
        for (int i = 0; i < ch.size(); ++i) {
            auto sh = i < args.size() ? args[i] : "";
            ans += ch[i]->print_(sh);
            if (i + 1 < enhs.size()) ans += enhs[i + 1];
        }
        return ans;
    }

    virtual std::string gettype() const {
        return "NodeConstList";
    }
    NodeConstList(std::vector<Node*> chs) : Node(), withStart(true), args(std::vector<std::string>()), enhs(std::vector<std::string>()) { ch = chs; };
    NodeConstList(std::vector<Node*> chs, std::vector<std::string> enhs) : Node(), withStart(true), args(std::vector<std::string>()), enhs(std::move(enhs)) { ch = chs; };
    NodeConstList(std::vector<Node*> chs, std::vector<std::string> enhs, std::vector<std::string> args, bool withStart = true) : Node(), withStart(withStart), args(std::move(args)), enhs(std::move(enhs)) { ch = chs; };
};

class NodeListEndl : public Node {
public:
    enum Options :unsigned char {
        AllEnable = 0x01,
        DisableEnd = 0x02,
        Disable = 0x03,
        LnMask = 0x03,

        AddShift = 0x04
    };
private:
    unsigned char ops;
    bool incShift;

public:
    virtual std::string print_(const std::string& shift = "") {
        return printLn(shift);
    }
    virtual std::string printLn(const std::string& shift = "") {
        std::string ans;

        for (int i = 0; i < ch.size(); ++i) {
            if (!ch[i]->isEndls()) {
                auto newline = ((ops & Options::LnMask) == Options::AllEnable) || (((ops & Options::LnMask) == Options::DisableEnd) && (i != ch.size() - 1));
                auto addshift = ops & Options::AddShift;
                std::string new_shift = shift;
                if (incShift && ch[i]->incShift()) new_shift += one_shift;
                ch[i]->appendPrint(ans, newline, addshift, new_shift);
            }
            else {
                NodeListEndl* tmp = (NodeListEndl*)ch[i]->ch[0];
                ans += tmp->printLn(shift);
            }
        }
        return ans;
    }
    virtual std::string text() const {
        std::string ans = "{ ";
        for (int i = (int)ch.size() - 1; i >= 0; --i) {
            if (i != ch.size() - 1) ans += ", ";
            ans += ch[i]->text();
        }
        return ans + "}";
    }

    virtual bool isEndls() const {
        return true;
    }

    virtual std::string gettype() const {
        return "NodeListEndl";
    }

    NodeListEndl(bool incshift = true, unsigned char ops = Options::AllEnable | Options::AddShift) : Node(), incShift(incshift), ops(ops) { };
};

class Node_mb : public Node {
public:
    virtual std::string print_(const std::string& shift = "") {
        if (ch.size()) {
            auto ans = ch[0]->print_(shift);
            return ans;
        }
        return "";
    }
    virtual bool isEndls() const {
        if (ch.size()) {
            return ch[0]->isEndls();
        }
        return false;
    }

    virtual bool incShift() const {
        if (ch.size())
            return ch[0]->incShift();
        return true;
    }
    virtual std::string gettype() const {
        if (ch.size())
            return "(Node_mb: " + ch[0]->gettype() + ")";
        return "Node_mb";
    }
    Node_mb(Node* val = nullptr) : Node() { if (val) { addch(val); } }
};

class Node_enhws : public Node {
private:
    std::string l, r;
public:
    virtual std::string print_(const std::string& shift = "") {
        unsigned char ops = ch[1]->ch.size() ? TrimOptionsPresets::l_enh : TrimOptionsPresets::remove;
        return l + ((NodeWSS*)ch[0])->printWS(ops) + ch[1]->print_(shift) + r + ((NodeWSS*)ch[2])->printWS(TrimOptionsPresets::r_enh);
    }

    virtual bool incShift() const {
        return ch[1]->incShift();
    }
    virtual std::string gettype() const {
        return "(Node_enhws: " + ch[1]->gettype() + ")";
    }
    Node_enhws(Node* val, const std::string l, Node* lws, const std::string r, Node* rws) : Node(), l(l), r(r) { addch({ lws, val, rws }); }
};

class Node_enh : public Node {
    // "[" mb_wss (Expression mb_wss) "]"
    // [a]; [;/c/; a]
private:
    std::string l, r;
public:
    virtual std::string print_(const std::string& shift = "") {
        unsigned char ops = ch[1]->ch.size() ? TrimOptionsPresets::l_enh : TrimOptionsPresets::remove;
        return l + ((NodeWSS*)ch[0])->printWS(ops) + ch[1]->print_(shift) + r;
    }

    virtual bool incShift() const {
        return ch[1]->incShift();
    }
    virtual std::string gettype() const {
        return "(Node_enh: " + ch[1]->gettype() + ")";
    }
    Node_enh(const std::string l, Node* lws, Node* val, const std::string r) : Node(), l(l), r(r) { addch({ lws, val }); }
};

class NodeBin : public Node {
private:
    std::string sp;
public:
    virtual std::string print_(const std::string& shift = "") {
        return ch[0]->print_() + sp + ch[1]->print_() + ch[2]->print_();
    }

    virtual std::string gettype() const {
        return "(NodeBin: " + ch[0]->gettype() + ", " + ch[2]->gettype() + ")";
    }
    NodeBin(Node* l, Node* s, Node* r, const std::string sp = " ") : Node(), sp(sp) { addch({ l, s, r }); };
};



class NodeNws : public Node {
private:
    unsigned char ops;
    bool right;
public:
    virtual std::string print_(const std::string& shift = "") {
        std::string ans = ch[0]->print_(shift);
        if (right) {
            if (ch[1])
                ans += ((NodeWSS*)ch[1])->printWS(ops);
        }
        else {
            ans = ((NodeWSS*)ch[1])->printWS(ops) + ans;
        }
        return ans;
    }
    virtual std::string gettype() const {
        return "(NodeNws: " + ch[0]->gettype() + ")";
    }
    virtual bool incShift() const {
        return ch[0]->incShift();
    }
    NodeNws(Node* n, unsigned char ops, Node* rws = nullptr, bool right = true) : Node(), ops(ops), right(right) { addch({ n, rws }); };
};

class NodeWSoneline : public NodeNws {
public:
    NodeWSoneline(Node* n, Node* rws = nullptr, bool right = true) : NodeNws(n, TrimOptionsPresets::oneline, rws, right) { };
};

class NodeWSmb_line : public NodeNws {
public:
    NodeWSmb_line(Node* n, Node* rws = nullptr, bool right = true) : NodeNws(n, TrimOptionsPresets::mb_line, rws, right) { };
};

class NodeWS_rem : public NodeNws {
public:
    NodeWS_rem(Node* n, Node* rws = nullptr, bool right = false) : NodeNws(n, TrimOptionsPresets::remove, rws, right) { };
};

class NodeWSl_enh : public NodeNws {
public:
    NodeWSl_enh(Node* n, Node* rws = nullptr, bool right = false) : NodeNws(n, TrimOptionsPresets::l_enh, rws, right) { };
};

class NodeWSr_enh : public NodeNws {
public:
    NodeWSr_enh(Node* n, Node* rws = nullptr, bool right = true) : NodeNws(n, TrimOptionsPresets::r_enh, rws, right) { };
};

class NodeRemoveLine : public Node {
    // for empty lines
public:
    virtual std::string print_(const std::string& shift = "") {
        std::string ans = ((NodeWSS*)ch[0])->printWS(TrimOptionsPresets::remove);
        return ans;
    }

    virtual std::string gettype() const {
        return "NodeRemoveLine";
    }
    NodeRemoveLine(Node* line) : Node() { addch(line); };
};



class NodeFunc : public Node {
public:
    virtual std::string print_(const std::string& shift = "") {
        std::string ans = ch[0]->print_() + "\n" + ch[1]->print_(shift) + shift + ch[2]->print_(shift);
        return ans;
    }
    virtual std::string gettype() const {
        return "NodeFunc";
    }
    NodeFunc(Node* header, Node* statements, Node* endfunction) : Node() { addch({ header, statements, endfunction }); };
};

class NodePropAuto : public Node {
private:
    std::string& presstabs(std::string& s, int a, int b) const {
        s += std::string(maxprops[a][b] - s.length(), ' ');
        return s;
    }
public:
    int proptype;
    virtual std::string print_(const std::string& shift = "") {
        std::string ans = ch[0]->print_() + ch[1]->print_(" ") + " " + ch[2]->print_() + ch[3]->print_();
        //ans += presstabs(type_, proptype-1, 0) + "property" + presstabs(name, proptype-1, 1);
        //ans += presstabs(init, proptype-1, 2) + " " + ch[2]->print_() + ch[3]->print_(" ");
        return ans;
    }

    std::string get_init() const {
        return "get_init";
    }

    std::string get_type() const {
        return ch[0]->ch[0]->print_();
    }

    std::string get_name() const {
        return ch[0]->ch[1]->text();
    }

    virtual std::string gettype() const {
        return "NodePropAuto";
    }
    NodePropAuto(Node* start, Node* init, Node* autoflag, Node* end) : Node(), proptype(autoflag->text() != "auto") { addch({ start, init, autoflag, end }); };
};

class NodeProgStatements : public Node {
private:
    void prepare_props() {
        for (int i = 0; i < 2; ++i) {
            for (auto& prop : ch[i]->ch) {
                NodePropAuto* p = (NodePropAuto*)prop;
                maxprops[i][0] = std::max(maxprops[i][0], (int)p->get_type().length());
                maxprops[i][1] = std::max(maxprops[i][1], (int)p->get_name().length());
                maxprops[i][2] = std::max(maxprops[i][2], (int)p->get_init().length());
            }
        }

        auto cmp = [](const auto& lhs, const auto& rhs)
        {
            NodePropAuto* l = (NodePropAuto*)lhs;
            NodePropAuto* r = (NodePropAuto*)rhs;
            std::string x = l->get_name();
            std::string y = r->get_name();
            std::for_each(x.begin(), x.end(), [](char& c) {
                c = ::toupper(c);
                });
            std::for_each(y.begin(), y.end(), [](char& c) {
                c = ::toupper(c);
                });
            return x < y;
        };
        std::sort(ch[0]->ch.begin(), ch[0]->ch.end(), cmp);
        std::sort(ch[1]->ch.begin(), ch[1]->ch.end(), cmp);
    }
public:
    virtual std::string print_(const std::string& shift = "") {
        prepare_props();
        std::string ans = ch[0]->print_() + "\n" + ch[1]->print_() + "\n" + ch[2]->print_() + "\n" + ch[3]->print_() + "\n" + ch[4]->print_();
        return ans;
    }

    virtual void addch(Node* c) {
        auto type = c->gettype();
        if (type == "NodePropAuto") {
            NodePropAuto* p = (NodePropAuto*)c;
            if (p->proptype == 1) {  // auto
                ch[0]->addch(c);
            }
            else if (p->proptype == 2)  // autoreadonly
                ch[1]->addch(c);
            else                    // big
                ch[2]->addch(c);
        }
        else if (type == "NodeGlobalDefin") {
            ch[3]->addch(c);
        }
        else {
            ch[4]->addch(c);
        }
    }

    virtual std::string gettype() const {
        return "NodeProgStatements";
    }

    // auto props, autoreadonly props, big props, global vars & others
    NodeProgStatements() : Node() {
        ch.push_back(new NodeListEndl(false));
        ch.push_back(new NodeListEndl(false));
        ch.push_back(new NodeListEndl(false));
        ch.push_back(new NodeListEndl(false));
        ch.push_back(new NodeList());
    };
};

class NodeParam : public Node {
public:
    virtual std::string print_(const std::string& shift = "") {
        std::string ans = ch[0]->print_();
        if (ch[1]->ch.size()) {
            ans += " " + ch[1]->ch[0]->print_();
            ans += ch[1]->ch[1]->print_();
        }
        return ans;
    }

    virtual std::string gettype() const {
        return "NodeParam";
    }
    NodeParam(Node* type, Node* mbinit) : Node() { addch({ type, mbinit }); };
};

class NodeIf : public Node {
public:
    virtual std::string print_(const std::string& shift) {
        std::string ans = "if" + ch[0]->print_() + "\n" + ch[1]->print_(shift);
        ans += shift + ((NodeWSS*)ch[2])->printWS(TrimOptionsPresets::l_enh);
        return ans + ch[3]->print_(shift) + ch[4]->print_(shift) + "endif";
    }
    NodeIf(Node* cond, Node* states, Node* ws, Node* some_elseifs, Node* mb_else) : Node() { addch({ cond, states, ws, some_elseifs, mb_else }); };
};

class NodeElse : public Node {
public:
    virtual std::string print_(const std::string& shift) {
        return ch[0]->print_() + "\n" + ch[1]->print_(shift) + "" + shift + ((NodeWSS*)ch[2])->printWS(TrimOptionsPresets::l_enh);
    }
    NodeElse(Node* else_, Node* states, Node* ws) : Node() { addch({ else_, states, ws }); };
};

class NodeElseIf : public Node {
public:
    virtual std::string print_(const std::string& shift) {
        return "elseif" + ch[0]->print_() + "\n" + ch[1]->print_(shift) + shift + ((NodeWSS*)ch[2])->printWS(TrimOptionsPresets::l_enh);
    }
    NodeElseIf(Node* cond, Node* states, Node* ws) : Node() { addch({ cond, states, ws }); };
};

class NodeWhile : public Node {
public:
    virtual std::string print_(const std::string& shift = "") {
        return "while" + ch[0]->print_() + "\n" + ch[1]->print_(shift) + shift + ch[2]->print_();
    }
    NodeWhile(Node* cond, Node* states, Node* endwhile) : Node() { addch({ cond, states, endwhile }); };
};

class NodeFuncHeader : public Node {
public:
    virtual std::string print_(const std::string& shift = "") {
        std::string ans = ch[0]->print_();
        if (ans != "") ans += " ";
        ans += ch[1]->print_() + ch[2]->print_() + ch[3]->print_(",") + ch[4]->print_();
        return ans;
    }
    virtual std::string gettype() const {
        return "NodeFuncHeader";
    }
    NodeFuncHeader(Node* type, Node* func, Node* name, Node* params, Node* flags) : Node() { addch({ type, func, name, params, flags }); };
};

