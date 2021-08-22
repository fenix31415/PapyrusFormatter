#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "UtilsLight.h"
#include <cassert>
#include <tuple>
#include <functional>
#include <array>

// auto/autoreadonly * type/name/init
static int maxprops[2][3];
static const std::string sep = "\n";

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
    const static unsigned char comment = TrimOptions::SRemove | TrimOptions::LRemove | TrimOptions::RForceNL;
};

class NodeWSS : public Node {
private:
    static std::pair<std::string, unsigned char> GetTrimStrS(unsigned char ops) {
        switch (ops) {
        case TrimOptions::SRemove:  return { "", 0 };
        case TrimOptions::SForceWS: return { " ", 0 };
        case TrimOptions::SForceNL: return { "\n", WSData::NewLn };
        default:                    return { "?s?", 0 };
        }
    }
    // using with R: ops >>= 3
    static std::pair<std::string, unsigned char> GetTrimStrLR(unsigned char ops, bool hasNewLn) {
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
    NodeWSS() = default;

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

    void printWS(std::ostream& out, unsigned char ops) {
        trim(ops);

        for (int i = 0; i < ch.size(); ++i) {
            ch[i]->print_(out);
        }
    }

    bool isContainsData(unsigned char ops) {
        trim(ops);
        bool ans = false;
        for (int i = 0; i < ch.size(); ++i) {
            ans = ans || ((NodeWS*)ch[i])->isContainsData();
        }
        return ans;
    }

    std::string gettype() const override { return "NodeWSS"; }
};

class NodeNws : public Node {
private:
    bool right;
public:
    unsigned char ops;
    void print_(std::ostream& out, const std::string& shift = "") override {
        if (!right) {
            ((NodeWSS*)ch[1])->printWS(out, ops);
            if (ch[0]) ch[0]->print_(out, shift);
        }
        else {
            if (ch[0]) ch[0]->print_(out, shift);
            if (ch[1]) ((NodeWSS*)ch[1])->printWS(out, ops);
        }
    }
    virtual bool isEmpty() const { return true; }
    std::string gettype() const override { return ch[0]->gettype(); }
    bool incShift() const override { return ch[0] ? ch[0]->incShift() : true; }
    NodeNws(Node* n, unsigned char ops, Node* rws = nullptr, bool right = true) : Node(), ops(ops), right(right) { addch({ n, rws }); };
};

class NodeList : public Node {
private:
    bool withStart;
public:
    void print_(std::ostream& out, const std::string& shift = "") override {
        for (int i = 0; i < ch.size(); ++i) {
            if (withStart || i != 0)
                out << shift;
            ch[i]->print_(out);
        }
    }

    std::string gettype() const override { return "NodeList"; }

    NodeList(bool withStart = true) : Node(), withStart(withStart) { };
};

class NodeConstList : public Node {
    bool withStart;
    const std::vector<std::string> args;
    const std::vector<std::string> enhs;
public:
    void print_(std::ostream& out, const std::string& shift = "") override {
        if (enhs.size()) out << enhs[0];
        for (int i = 0; i < ch.size(); ++i) {
            ch[i]->print_(out, i < args.size() ? args[i] : "");
            if (i + 1 < (int)enhs.size()) out << enhs[i + 1];
        }
    }

    std::string gettype() const override { return "NodeConstList"; }

    NodeConstList(std::vector<Node*> chs) : Node(), withStart(true), args(std::vector<std::string>()), enhs(std::vector<std::string>()) { ch = chs; };
    NodeConstList(std::vector<Node*> chs, std::vector<std::string> enhs) : Node(), withStart(true), args(std::vector<std::string>()), enhs(std::move(enhs)) { ch = chs; };
    NodeConstList(std::vector<Node*> chs, std::vector<std::string> enhs, std::vector<std::string> args, bool withStart = true) : Node(), withStart(withStart), args(std::move(args)), enhs(std::move(enhs)) { ch = chs; };
};

class NodeConstListT : public NodeConstList {
    const std::string type;
public:
    std::string gettype() const override { return type; }
    NodeConstListT(std::string type, std::vector<Node*> chs) : NodeConstList(chs), type(type) { };
    NodeConstListT(std::string type, std::vector<Node*> chs, std::vector<std::string> enhs) : NodeConstList(chs, enhs), type(type) { };
    NodeConstListT(std::string type, std::vector<Node*> chs, std::vector<std::string> enhs, std::vector<std::string> args, bool withStart = true) : NodeConstList(chs, enhs, args), type(type) { };
};

class NodeWScomment : public NodeNws {
public:
    bool isEmpty() const override { return false; }
    NodeWScomment(Node* rws) : NodeNws(nullptr, TrimOptionsPresets::comment, rws, false) { };
};

class NodeListEndlBase : public Node {
public:
    enum Options :unsigned char {
        AllEnable = 0x01,
        DisableStart = 0x02,
        Disable = 0x03,  // only for %empty some_elseIfBlock
        LnMask = 0x03,

        AddShift = 0x04
    };
    enum NodeType
    {
        Item, Docstring, Comment, Empty, EndlList, EmptyItem
    };
    using nodes_t = std::vector<Node*>;

    unsigned char ops;
    NodeListEndlBase(unsigned char ops) : ops(ops) { };
    bool isEndls() const override { return true; }
    std::string gettype() const override { return "NodeListEndl"; }

    // opts: lastendl, firstendl, addshift, hasendl
    void appendPrint(Node* ch_i, std::ostream& out, const std::string& shift, unsigned char opts, bool* hasendl) const {
        if (opts & 2 && (!hasendl || !(*hasendl))) {
            out << "\n";
        }
        if (opts & 0x4) out << shift;
        ch_i->print_(out, shift);
        if ((opts & 1) || (opts & 0x8)) {
            out << "\n";
            if (hasendl && !(opts & 8)) *hasendl = true;
            if (hasendl && (opts & 8)) *hasendl = false;
        }
        else if (!dynamic_cast<NodeWScomment*>(ch_i) && !(opts & 1)) {
            if (hasendl) *hasendl = false;
        }
    }

    NodeType get_type(Node* v_i) const {
        if (v_i->isEndls()) return EndlList;

        if (NodeNws* d = dynamic_cast<NodeNws*>(v_i)) {
            if (d->isEmpty()) return v_i->gettype() == "NodeDocstring" ? Docstring : EmptyItem;
            else return ((NodeWSS*)d->ch[1])->isContainsData(d->ops) ? Comment : Empty;
        }
        else return Item;
    }
    void generic_print_one(std::ostream& out, Node* v, const std::string& shift, bool compressed, bool* hasendl, unsigned char ops) const;
    void generic_print(std::ostream& out, const nodes_t& v, const std::string& shift, bool compressed, unsigned char ops, bool* shouldsep = nullptr) const {
        if (shouldsep && *shouldsep && v.size()) out << sep;
        if (shouldsep) *shouldsep = *shouldsep || v.size() > 0;
        bool hasendl = true;
        for (int i = 0; i < v.size(); ++i) {
            generic_print_one(out, v[i], shift, compressed, &hasendl, ops);
        }
        if (shouldsep && v.size() > 0) *shouldsep = !hasendl;
    }
};

class NodeListEndl : public NodeListEndlBase {
public:
    void print_(std::ostream& out, const std::string& shift = "") override { return generic_print(out, ch, shift, true, ops); }
    NodeListEndl(unsigned char ops) : NodeListEndlBase(ops) { };
};

void NodeListEndlBase::generic_print_one(std::ostream& out, Node* v, const std::string& shift, bool compressed, bool* hasendl, unsigned char ops) const {
    std::string new_shift = shift;
    if (v->incShift() && (ops & Options::AddShift)) new_shift += one_shift;
    auto type = get_type(v);
    unsigned char addshift = ops & Options::AddShift ? 0x4 : 0x0;

    if (type == EndlList)  return ((NodeListEndl*)v->ch[0])->print_(out, shift);
    if (type == Docstring) return appendPrint(v, out, new_shift, 0x1 | addshift, hasendl);

    unsigned char opts = (ops & Options::LnMask) == Options::AllEnable ? 0x8 : 0x0;

    if (type == EmptyItem) return appendPrint(v, out, new_shift, ((ops & 0x80) ? (compressed ? 0x0 : 0x2) : opts) | addshift, hasendl);
    if (type == Item)      return appendPrint(v, out, new_shift, opts | addshift, hasendl);
    if (type == Comment) { appendPrint(v, out, new_shift, 0x2 | addshift, hasendl); *hasendl = true; return; }
    if ((ops & 0x80) && type == Empty) return appendPrint(v, out, new_shift, 0x0 | addshift, hasendl);
}

class Node_mb : public Node {
public:
    void print_(std::ostream& out, const std::string& shift = "") override { if (ch.size()) ch[0]->print_(out, shift); }
    bool isEndls() const override { return ch.size() ? ch[0]->isEndls() : false; }
    bool incShift() const override { return ch.size() ? ch[0]->incShift() : true; }
    std::string gettype() const override { return ch.size() ? "(Node_mb: " + ch[0]->gettype() + ")" : "Node_mb"; }
    Node_mb(Node* val = nullptr) : Node() { if (val) { addch(val); } }
};

class Node_enhws : public Node {
private:
    std::string l, r;
public:
    void print_(std::ostream& out, const std::string& shift = "") override {
        unsigned char ops = ch[1]->ch.size() ? TrimOptionsPresets::l_enh : TrimOptionsPresets::remove;
        out << l;
        ((NodeWSS*)ch[0])->printWS(out, ops);
        ch[1]->print_(out, shift);
        out << r;
        ((NodeWSS*)ch[2])->printWS(out, TrimOptionsPresets::r_enh);
    }
    bool incShift() const override { return ch[1]->incShift(); }
    std::string gettype() const override { return "(Node_enhws: " + ch[1]->gettype() + ")"; }
    Node_enhws(Node* val, const std::string l, Node* lws, const std::string r, Node* rws) : Node(), l(l), r(r) { addch({ lws, val, rws }); }
};

class Node_enh : public Node {
    // "[" mb_wss (Expression mb_wss) "]"
    // [a]; [;/c/; a]
private:
    std::string l, r;
public:
    void print_(std::ostream& out, const std::string& shift = "") override {
        unsigned char ops = ch[1]->ch.size() ? TrimOptionsPresets::l_enh : TrimOptionsPresets::remove;
        out << l;
        ((NodeWSS*)ch[0])->printWS(out, ops);
        ch[1]->print_(out, shift);
        out << r;
    }

    bool incShift() const override { return ch[1]->incShift(); }
    std::string gettype() const override { return "(Node_enh: " + ch[1]->gettype() + ")"; }
    Node_enh(const std::string l, Node* lws, Node* val, const std::string r) : Node(), l(l), r(r) { addch({ lws, val }); }
};

class NodeBin : public Node {
private:
    std::string sp;
public:
    void print_(std::ostream& out, const std::string& shift = "") {
        ch[0]->print_(out);
        out << sp;
        ch[1]->print_(out);
        ch[2]->print_(out);
    }

    std::string gettype() const override { return "(NodeBin: " + ch[0]->gettype() + ", " + ch[2]->gettype() + ")"; }
    NodeBin(Node* l, Node* s, Node* r, const std::string sp = " ") : Node(), sp(sp) { addch({ l, s, r }); };
};



class NodeWSnewline : public NodeNws {
public:
    NodeWSnewline(Node* n, Node* rws = nullptr, bool right = false) : NodeNws(n, TrimOptionsPresets::newline, rws, right) { };
};

class NodeWSlastline : public NodeNws {
public:
    NodeWSlastline(Node* n, Node* rws = nullptr, bool right = true) : NodeNws(n, TrimOptionsPresets::comment, rws, right) { };
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



class NodeFunc : public Node {
    bool isevent;
public:
    void print_(std::ostream& out, const std::string& shift = "") override {
        ch[0]->print_(out);
        out << "\n";
        ch[1]->print_(out, shift);
        out << shift;
        ch[2]->print_(out, shift);
    }
    std::string gettype() const override { return isevent ? "NodeEvent" : "NodeFunc"; }
    NodeFunc(Node* header, Node* statements, Node* endfunction, bool isevent = false) : Node(), isevent(isevent) { addch({ header, statements, endfunction }); };
};

// for state, script, big_property
class NodeProgStatements : public NodeListEndlBase {
    using NodeListEndlBase::generic_print;
    using item_t = std::tuple<Node*, nodes_t, Node*>;
    using nodes_vec = std::vector<item_t>;  // item, comms, docstr
    nodes_t tech[2]; // start, end
    static constexpr size_t ITEMS_SIZE = 10;
    nodes_vec items[ITEMS_SIZE];  // props_auto, props_read, props_big, vars, functions, states, imports, functions_native, events

    static size_t get_item_ind(std::string nodetype) {
        if (nodetype == "NodePropAuto") return 0;
        if (nodetype == "NodePropRead") return 1;
        if (nodetype == "NodePropBig") return 2;
        if (nodetype == "GlobalDefin") return 3;
        if (nodetype == "NodeFunc") return 4;
        if (nodetype == "State") return 5;
        if (nodetype == "Import") return 6;
        if (nodetype == "NodeFunc_n") return 7;
        if (nodetype == "NodeEvent") return 8;
        if (nodetype == "NodeEvent_n") return 9;
        std::cerr << "[WARN] unknown type " << nodetype << std::endl;
        return 5;
    }
    void prepare_props() {
        /*for (int i = 0; i < 2; ++i) {
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
        std::sort(ch[1]->ch.begin(), ch[1]->ch.end(), cmp);*/
        tech[0] = nodes_t();
        tech[0] = nodes_t();
        for (int i = 0; i < 8; i++)
        {
            items[i] = nodes_vec();
        }

        int ind = 0;
        int oldind = -1;
        nodes_vec* old_to_add = nullptr;
        for (; ind < ch.size(); ++ind) {
            auto type = get_type(ch[ind]);
            if (type == Comment) {
                if (oldind == -1) oldind = ind;
            }
            else if (type == Item || type == EndlList || type == EmptyItem) {
                nodes_vec& to_add = items[get_item_ind(ch[ind]->gettype())];
                old_to_add = &to_add;
                auto curind = to_add.size();
                to_add.push_back(std::make_tuple(ch[ind], nodes_t(), nullptr));
                if (oldind > -1) {
                    while (oldind < ind) {
                        std::get<1>(to_add[curind]).push_back(ch[oldind]);
                        ++oldind;
                    }
                    oldind = -1;
                }
            }
            else if (type == Docstring) {
                if (old_to_add) std::get<2>((*old_to_add)[old_to_add->size()-1]) = ch[ind];
                else tech[0].push_back(ch[ind]);
            }
        }
        while (oldind > -1 && oldind < ind) {
            tech[1].push_back(ch[oldind]);
            ++oldind;
        }
    }
    void generic_print(std::ostream& out, const nodes_vec& v, const std::string& shift, bool compressed, unsigned char ops, bool* shouldsep) {
        if (*shouldsep && v.size()) out << sep;
        *shouldsep = *shouldsep || v.size() > 0;
        bool hasendl = true;
        for (int i = 0; i < v.size(); ++i) {
            auto comms = std::get<1>(v[i]);
            for (auto& com : comms)
            {
                generic_print_one(out, com, shift, compressed, &hasendl, ops);
            }
            generic_print_one(out, std::get<0>(v[i]), shift, compressed, &hasendl, ops);
            auto docstr = std::get<2>(v[i]);
            if (docstr)
                generic_print_one(out, docstr, shift, compressed, &hasendl, ops);
        }
        if (v.size() > 0) *shouldsep = !hasendl;
    }

    inline const static std::array<std::function<const std::string* (Node*)>, ITEMS_SIZE> keyers = {
        [](Node* a) { return &a->ch[0]->ch[0]->ch[0]->ch[1]->ch[0]->value; },
        [](Node* a) { return &a->ch[0]->ch[0]->ch[0]->ch[1]->ch[0]->value; },
        [](Node* a) { return &a->ch[0]->ch[0]->ch[1]->ch[0]->value; },
        [](Node* a) { return &a->ch[0]->ch[1]->ch[0]->value; },
        [](Node* a) { return &a->ch[0]->ch[0]->ch[2]->ch[0]->value; },
        [](Node* a) { return &a->ch[0]->ch[0]->ch[1]->ch[0]->value; },
        [](Node* a) { return &a->ch[0]->ch[1]->ch[0]->value; },
        [](Node* a) { return &a->ch[0]->ch[0]->ch[2]->ch[0]->value; },
        [](Node* a) { return &a->ch[0]->ch[0]->ch[2]->ch[0]->value; },
        [](Node* a) { return &a->ch[0]->ch[0]->ch[2]->ch[0]->value; }
    };

    template<size_t IND>
    void sort() {
        const auto key = keyers[IND];
        auto& cur = items[IND];
        std::sort(cur.begin(), cur.end(), [&key](const item_t& _a, const item_t& _b) {
            auto a = std::get<0>(_a);
            auto b = std::get<0>(_b);
            auto a_name = key(a);
            auto b_name = key(b);
            return a_name->compare(*b_name) < 0;
            });
    }

    template<size_t IND>
    void sortprint(std::ostream& out, const std::string& shift, bool compressed, bool* shouldsep) {
        sort<IND>();
        generic_print(out, items[IND], shift, compressed, ops | 0x80, shouldsep);
    }
    /* mb later align
    void prepare_props_auto_(nodes_vec::const_iterator begin, nodes_vec::const_iterator end) {
        struct Lengths {
            size_t type = 0;
            size_t name = 0;
        } lengths;
        for (; begin != end; ++begin)
        {
            lengths.name = std::max(lengths.name, std::get<0>(*begin)->ch[0]->ch[0]->ch[0]->ch[1]->ch[0]->value.size());
            std::stringstream ss;
            std::get<0>(*begin)->ch[0]->ch[0]->ch[0]->ch[0]->print_(ss);
            lengths.type = std::max(lengths.type, ss.str().size());
        }
    }

    void prepare_props_auto(const nodes_vec& v, const std::string& shift, unsigned char ops) {
        auto l = v.begin(), r = l;
        while (r != v.end()) {
            while (r != v.end() && std::get<1>(*r).size() == 0) ++r;

            prepare_props_auto_(l, r);
            l = r;
        }
    }
    */
public:
    void print_(std::ostream& out, const std::string& shift = "") override {
        prepare_props();

        bool _shouldsep = (ops & 0x10) && tech[0].size() == 0;
        auto shouldsep = &_shouldsep;
        generic_print(out, tech[0], shift, true, ops | 0x80, shouldsep);  // scriptname docstr & something other

        sortprint<6>(out, shift, true, shouldsep);   // imports
        sortprint<0>(out, shift, true, shouldsep);   // auto props
        sortprint<1>(out, shift, true, shouldsep);   // autoreadonly props
        sortprint<3>(out, shift, true, shouldsep);   // vars
        sortprint<2>(out, shift, false, shouldsep);  // big props
        sortprint<8>(out, shift, false, shouldsep);  // events
        sortprint<4>(out, shift, false, shouldsep);  // functions
        sortprint<5>(out, shift, false, shouldsep);  // states
        sortprint<9>(out, shift, true, shouldsep);   // native events
        sortprint<7>(out, shift, true, shouldsep);   // native functions
        
        generic_print(out, tech[1], shift, true, ops | 0x80, shouldsep);  // last comments
    }

    NodeProgStatements(bool inc = true, bool isglobal = false) : NodeListEndlBase((inc ? NodeListEndlBase::Options::AddShift | NodeListEndlBase::Options::AddShift : 0) | (isglobal ? 0x10 : 0x0)) { };
};

class NodeParam : public Node {
public:
    void print_(std::ostream& out, const std::string& shift = "") override {
        ch[0]->print_(out);
        if (ch[1]->ch.size()) {
            out << " ";
            ch[1]->ch[0]->print_(out);
            ch[1]->ch[1]->print_(out);
        }
    }

    std::string gettype() const override { return "NodeParam"; }
    NodeParam(Node* type, Node* mbinit) : Node() { addch({ type, mbinit }); };
};

class NodeIf : public Node {
public:
    void print_(std::ostream& out, const std::string& shift) override {
        out << "if";
        ch[0]->print_(out);
        out << "\n";
        ch[1]->print_(out, shift);
        out << shift;
        ((NodeWSS*)ch[2])->printWS(out, TrimOptionsPresets::l_enh);
        ch[3]->print_(out, shift);
        ch[4]->print_(out, shift);
        out << "endif";
    }
    NodeIf(Node* cond, Node* states, Node* ws, Node* some_elseifs, Node* mb_else) : Node() { addch({ cond, states, ws, some_elseifs, mb_else }); };
};

class NodeElse : public Node {
public:
    void print_(std::ostream& out, const std::string& shift) override {
        ch[0]->print_(out);
        out << "\n";
        ch[1]->print_(out, shift);
        out << shift;
        ((NodeWSS*)ch[2])->printWS(out, TrimOptionsPresets::l_enh);
    }
    NodeElse(Node* else_, Node* states, Node* ws) : Node() { addch({ else_, states, ws }); };
};

class NodeElseIf : public Node {
public:
    void print_(std::ostream& out, const std::string& shift) override {
        out << "elseif";
        ch[0]->print_(out);
        out << "\n";
        ch[1]->print_(out, shift);
        out << shift;
        ((NodeWSS*)ch[2])->printWS(out, TrimOptionsPresets::l_enh);
    }
    NodeElseIf(Node* cond, Node* states, Node* ws) : Node() { addch({ cond, states, ws }); };
};

class NodeWhile : public Node {
public:
    void print_(std::ostream& out, const std::string& shift = "") override {
        out << "while";
        ch[0]->print_(out);
        out << "\n";
        ch[1]->print_(out, shift);
        out << shift;
        ch[2]->print_(out);
    }
    NodeWhile(Node* cond, Node* states, Node* endwhile) : Node() { addch({ cond, states, endwhile }); };
};

class NodeFuncHeader : public Node {
public:
    void print_(std::ostream& out, const std::string& shift = "") override {
        ch[0]->print_(out);
        if (ch[0]->ch.size()) out << " ";
        ch[1]->print_(out);
        ch[2]->print_(out);
        ch[3]->print_(out, ",");
        ch[4]->print_(out);
    }
    std::string gettype() const override { return "NodeFuncHeader"; }
    NodeFuncHeader(Node* type, Node* func, Node* name, Node* params, Node* flags) : Node() { addch({ type, func, name, params, flags }); };
};

