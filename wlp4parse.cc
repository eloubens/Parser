#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include "wlp4data.h"
#include <memory>


class Tree {
    public: 
        std::vector<std::unique_ptr<Tree>> children;
        std::pair<std::string, std::string> data;

    Tree(std::string token, std::string value) : data{token, value} {}
    Tree() {} // implies a empty node (aka the bottom of the leaf)

    void addChildString(std::string d1, std::string d2) { 
        children.insert(children.begin(), std::make_unique<Tree>(d1, d2)); 
    }

    void addChildFront(std::unique_ptr<Tree> child) {
        children.insert(children.begin(), std::move(child));
    }

    void print() {
        if (data.first != "") {
            std::cout << data.first << " ";
        }
        for (unsigned int i = 0; i < children.size(); i++) {
            children[i]->print();
        }
    }

    void traverse() { 
        if (data.first == ".EMPTY" && data.second == ".EMPTY" ) {

        }else {
            if (data.first != "" && children.empty()) {
                std::cout << data.first << " X " << data.second << " " ;
            } else {
            std::cout << data.first << " " ;
            }
            for (unsigned int i = 0; i < children.size(); i++) {
                if (children[i]->data.first != "") {
                    std::cout << children[i]->data.first << " ";
                }
            }
            std::cout << std::endl;
        }
        for (unsigned int i = 0; i < children.size(); i++) {
            children[i]->traverse();
        }
    }
};


void print(const std::vector<std::pair<std::string, std::string>> &iseq, const std::vector<std::unique_ptr<Tree>> &redseq) {
    std::cout << std::endl;
    for (unsigned int i = 0; i < redseq.size(); i++) {
        redseq[i]->print();
    }

    
    
    std::cout << ". " << std::endl;
    for (auto [token, val]  : iseq) {
        std::cout << token << " ";// << val << " ";
    }
    std::cout << std::endl;
}

void printFirst(const std::vector<std::pair<std::string, std::string>> &iseq, const std::vector<std::unique_ptr<Tree>> &redseq) {
    std::cout << std::endl;
    for (unsigned int i = 0; i < redseq.size(); i++) {
        std::cout << redseq[i]->data.first << " W ";
    }

    
    
    std::cout << ". " << std::endl;
    for (auto [token, val]  : iseq) {
        std::cout << token << " ";// << val << " ";
    }
    std::cout << std::endl;
}

void print(const std::vector<std::unique_ptr<Tree>> &redseq) {
    for (unsigned int i = 0; i < redseq.size(); i++) {
        redseq[i]->print();
        std::cout << " X ";
    }
}

void printFirst(const std::vector<std::unique_ptr<Tree>> &redseq) {
    for (unsigned int i = 0; i < redseq.size(); i++) {
        std::cout << redseq[i]->data.first << " " ;
    }
}

int main() {
    std::string l; //line
    std::string w; //word
    std::istream &in = std::cin; 
    std::vector<std::pair<std::string, std::vector<std::string>>> cfg;
    std::vector<std::unique_ptr<Tree>> redseq;
    std::vector<std::pair<std::string, std::string>> iseq;
    std::map<std::pair<int, std::string>,int> trans;
    std::map<std::pair<int, std::string>,int> reduct;

    // initialize given CFG
    std::istringstream c{WLP4_CFG};
    getline(c, l); // get rid fo CFG header

    // .CFG section
    while(std::getline(c, l) && l[0] != '.' ) {
        std::istringstream iss{l};
        std::string lhs;
        std::vector<std::string> rhs;
        iss >> lhs; // read in the lhs
        while (iss >> w) {
            rhs.emplace_back(w);
        }
        cfg.emplace_back(std::make_pair(lhs, rhs));
    } 

    std::istringstream t{WLP4_TRANSITIONS};
    std::getline(t, l);
    //TRASITIONS section
    while(std::getline(t, l) && l[0] != '.') {
        std::istringstream iss{l};
        int from, to;
        std::string symbol;
        iss >> from >> symbol >> to;
        trans.insert(std::make_pair(std::make_pair(from, symbol), to));
    }
    
    std::istringstream r{WLP4_REDUCTIONS};
    std::getline(r, l);

    //REDUCTIONS section
    while(std::getline(r, l) && l[0] != '.') {
        std::istringstream iss{l};
        int state, rule;
        std::string tag;
        iss >> state >> rule >> tag;
        reduct.insert(std::make_pair(std::make_pair(state, tag), rule));
    }

    iseq.emplace_back(std::make_pair("BOF", "BOF"));

    //.INPUT section
    while(std::getline(in, l)) {
        std::istringstream iss{l};
        std::string first, second;
        iss >> first >> second;
        iseq.emplace_back(std::make_pair(first, second));
    }
    iseq.emplace_back(std::make_pair("EOF", "EOF"));

    int state = 0; 
    int k = 0;
    // print(iseq, redseq);

    while(iseq.size() != 0) {
        if (reduct.count(std::make_pair(state, iseq[0].first)) > 0) {
            int n = reduct[std::make_pair(state, iseq[0].first)]; // get the number of what production to reduce 
            std::unique_ptr<Tree> temp = std::make_unique<Tree>(cfg[n].first, cfg[n].first);
            if ( cfg[n].second[0] == ".EMPTY") {
                temp->addChildString(".EMPTY", ".EMPTY");
            } else {
                for (unsigned int i = 0; i < cfg[n].second.size(); i++) {
                    temp->addChildFront(std::move(redseq[redseq.size() - 1 - i]));
                }
                for (unsigned int i = 0; i < cfg[n].second.size(); i++) {
                    redseq.pop_back();
                }
            }

            redseq.emplace_back(std::move(temp)); // push LHS onto reduced sequence 
            state = 0;
            // find the current state using DFA of the reduced sequence
            for (unsigned int i = 0; i < redseq.size(); i++) {
                if (trans.count(std::make_pair(state, redseq[i]->data.first)) > 0) {
                    state = trans[std::make_pair(state, redseq[i]->data.first)];
                } else {
                    std::cerr << "ERROR at " << k;
                    return 1;
                }
            }
        }else if (trans.count(std::make_pair(state, iseq[0].first)) > 0) {
            state = trans[std::make_pair(state, iseq[0].first)]; // get next state
            redseq.emplace_back(std::make_unique<Tree>(iseq[0].first, iseq[0].second));
            iseq.erase(iseq.begin());
            k++;
        } else {
            std::cerr << "ERROR at " << k;
            return 1;
        }
    }

    if (reduct.count(std::make_pair(state, ".ACCEPT")) > 0) {
        int n = reduct[std::make_pair(state, ".ACCEPT")]; // get the number of what production to reduce 
        std::unique_ptr<Tree> temp = std::make_unique<Tree>(cfg[n].first, cfg[n].first);
        if ( cfg[n].second[0] == ".EMPTY") {
            temp->addChildString(".EMPTY", ".EMPTY");
        } else {
            for (unsigned int i = 0; i < cfg[n].second.size(); i++) {
                temp->addChildFront(std::move(redseq[redseq.size() - 1 - i]));
            }
            for (unsigned int i = 0; i < cfg[n].second.size(); i++) {
                redseq.pop_back();
            }
        }
        redseq.emplace_back(std::move(temp)); // push LHS onto reduced sequence !
    } else {
        std::cerr << "ERROR at " << k;
        return 1;
    }
     redseq[0]->traverse();
}
