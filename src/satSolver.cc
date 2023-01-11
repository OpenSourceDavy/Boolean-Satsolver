#include <memory>
#include <vector>
#include <set>
#include <map>

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <unordered_map>
#include <queue>
;


struct result {
    std::vector<std::vector<int>> CNF;
    std::set<int> Assignment;
    bool flag{};
};

static int negate1(int literal) {
    return -literal;
}


static int var(int literal) {
    return abs(literal);
}

static bool sign(int literal) {
    return literal > 0;
}


static result bcp(std::vector<std::vector<int>>& formula, std::set<int>& assignment) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i =0;i<formula.size(); i++) {
            std::vector<int> clause = formula[i];
            int size = formula[i].size();
            if (size==0){
                result re;
                re.CNF = formula;
                re.Assignment = assignment ;
                return re;}
            else if (size == 1) {

                formula.erase(remove(formula.begin(), formula.end(), clause), formula.end());
                i--;
                if (!assignment.count(clause[0])){
                    assignment.insert(clause[0]);
                }
                for (auto &c: formula) {
                    c.erase(remove(c.begin(), c.end(), negate1(clause[0])), c.end());
                }
                changed = true;

            }
            else if (size>1){
                continue;
            }
        }
    }

    result re;
    re.CNF = formula;
    re.Assignment = assignment ;

    return re;
}

static bool hasUnitClause(std::vector<std::vector<int>> formula){
    for(auto clause:formula){
        if(clause.size()==1){
            return true;
        }
    }
    return false;
}



static result plp(std::vector<std::vector<int>>& formula, std::set<int>& assignment) {
    std::set<int> seenPositive;
    std::set<int> seenNegative;

    // Find pure literals (variables that appear only as a positive literal or
    // only as a negative literal).
    for (const auto& clause : formula) {
        for (int literal : clause) {
            if (sign(literal)) {
                seenPositive.insert(var(literal));
            } else {
                seenNegative.insert(var(literal));
            }
        }
    }

    std::set<int> pureLiterals;
    for (int var : seenPositive) {
        if (!seenNegative.count(var)) {
            pureLiterals.insert(var);
        }
    }
    for (int var : seenNegative) {
        if (!seenPositive.count(var)) {
            pureLiterals.insert(-var);
        }
    }


    while (!pureLiterals.empty()) {

        int literal = *pureLiterals.begin();

//        assignment.insert(literal);
        pureLiterals.erase(literal);
        formula.erase(
                remove_if(formula.begin(), formula.end(), [&]( auto& clause) {
                    return find(clause.begin(), clause.end(), literal) != clause.end();
                }),
                formula.end());
    }


    result re;
    re.CNF = formula;
    re.Assignment = assignment ;
    return re;
}


static std::vector<std::vector<int>> checkcnf(const std::vector<std::vector<int>>& formula, int literal) {
    std::vector<std::vector<int>> simplified_formula;
    for (const auto& clause : formula) {
        // check if the clause is already satisfied
        bool satisfied = false;
        for (const auto& l : clause) {
            if (l == literal) {
                satisfied = true;
                break;
            }
        }
        if (!satisfied) {
            // remove the literal from the clause if it is made false by the assignment
            std::vector<int> new_clause;
            for (const auto& l : clause) {
                if (l != -literal) {
                    new_clause.push_back(l);
                }
            }
            simplified_formula.push_back(new_clause);
        }
    }
    return simplified_formula;
}


static bool DPLL(std::vector<std::vector<int>>& formula1, std::set<int> assignment1) {
    result re;
    result result;

    re = bcp(formula1, assignment1);

    result = plp(re.CNF, re.Assignment);

    std::vector<std::vector<int>> formula = result.CNF;
    std::set<int> assignment = result.Assignment;

    if (formula.empty()) {
        // The formula is satisfied by the current assignment.
        return true;
    }
    for (const auto &clause: formula) {
        if (clause.empty()) {

            return false;
        }
    }

    // Choose a variable to split on.
    int literal = 0;
    for (const auto& clause : formula) {
        for (const auto& l : clause) {
            if (!assignment.count(l)&& !assignment.count(-l)) {
                literal = l;
                break;
            }
        }
        break;
    }

    // create a copy of the formula and the assignment
    std::vector<std::vector<int>> formula_copy = formula;
    std::set<int> assignment_copy = assignment;

    // try setting the literal to true
    formula_copy = checkcnf(formula_copy, literal);
    assignment_copy.insert(literal);
    if (DPLL(formula_copy, assignment_copy)) {
        return true;
    }

    // try setting the literal to false
    formula_copy = formula;
    assignment_copy = assignment;
    formula_copy = checkcnf(formula_copy, -literal);
    assignment_copy.insert(-literal);
    if (DPLL(formula_copy, assignment_copy)) {
        return true;
    }

    return false;
}
//    int literal = 0;
//    for (const auto &clause: formula) {
//        for (auto number: clause) {
//            if (!assignment.count(number)) {
//                literal = number;
//                break;
//            }
//        }
//        bool flag = true;
//        for (auto i: assignment) {
//            if (-literal == i) {
//                flag = false;
//            }
//        }
//        if (literal != 0 && flag) {
//            break;
//        }
//    }
//    for (auto &i: assignment) {
//        if (-literal == i)
//            return false;
//    }
//    assignment.insert(literal);
//    if (dpll(formula, assignment)) {
//        return true;
//    }
//    assignment.erase(literal);
//    assignment.insert(-literal);
//    if (dpll(formula, assignment)) {
//        return true;
//    }
//    assignment.erase(-literal);
//    return false;



//int main() {
//    // Example usage:
//    CNF formula = {{3},
//                   {-3}
//                   };
//    Assignment assignment(20);
//
//    if (dpll(formula, assignment)) {
//        cout << "Satisfiable" << endl;
////        for (int i = 0; i < assignment.size(); i++) {
////            cout << "x" << i + 1 << " = " << assignment[i] << endl;
////        }
//    } else {
//        cout << "Unsatisfiable" << endl;
//    }
//    return 0;
//}
