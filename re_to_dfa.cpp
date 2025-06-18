#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

using namespace std;

class DFAState {
public:
    bool isFinal; // final state test
    int id;  // ID for each state
    map<char, DFAState*> transitions;
    DFAState(bool finalState = false, int stateID = 0) : isFinal(finalState), id(stateID) {}
};

class DFA {
private:
    DFAState* startState;
    DFAState* rejectState; // reject states
    int nextStateID;  // id
    vector<DFAState*> states;  // keep track

public:
    DFA() : startState(new DFAState()), nextStateID(1) {
        rejectState = new DFAState(false, -1); // loop for rejects
        rejectState->transitions['0'] = rejectState;
        rejectState->transitions['1'] = rejectState;
        states.push_back(startState);
    }

    ~DFA() {
        for (auto state : states) {
            delete state;
        }
        delete rejectState; // delete after each run
    }

    void reset() { // resetting
        for (auto state : states) {
            if (state != rejectState) { // avoid double delete
                delete state;
            }
        }
        states.clear();
        startState = new DFAState();
        states.push_back(startState);
        nextStateID = 1;
    }

    void addPattern(const string& re) {
        parseExpression(startState, re, 0, re.length() - 1);
        finalizeTransitions(); // Adding rejects 
    }

    void finalizeTransitions() { 
        for (auto state : states) {
            if (state->transitions.find('0') == state->transitions.end()) {
                state->transitions['0'] = rejectState; // if no 0, its a reject
            }
            if (state->transitions.find('1') == state->transitions.end()) {
                state->transitions['1'] = rejectState; // if no 1, its a reject
            }
        }
    }

int parseExpression(DFAState* currentState, const string& re, int start, int end) {
    for (int i = start; i <= end; ++i) {
        if (re[i] == '(') {
            int closeIndex = findClosingParenthesis(re, i);
            if (closeIndex == -1) {
                cerr << "Error: Unmatched parentheses." << endl; // for incorrect ( )
                return -1;
            }
            DFAState* subPatternState = new DFAState(false, nextStateID++);
            currentState->transitions['0'] = subPatternState;
            currentState->transitions['1'] = subPatternState;
            states.push_back(subPatternState);

            if (closeIndex < end && re[closeIndex + 1] == '*') {
                subPatternState->isFinal = true;
                subPatternState->transitions['0'] = subPatternState;
                subPatternState->transitions['1'] = subPatternState;
                i = closeIndex + 1;
            } else {
                currentState = subPatternState;
            }
            i = closeIndex;
        } else if (re[i] != '*') { // no add * for input
            DFAState* nextState = new DFAState(false, nextStateID++);
            currentState->transitions[re[i]] = nextState;
            states.push_back(nextState);

            if (i < end && re[i + 1] == '*') {
                nextState->isFinal = true;
                nextState->transitions[re[i]] = nextState;
                i++;
            } else {
                currentState = nextState;
            }
        }
    }
    currentState->isFinal = true;
    return 0;
}

    int findClosingParenthesis(const string& re, int openIndex) { // logic with parenthesis
        int balance = 1;
        for (int i = openIndex + 1; i < re.length(); i++) {
            if (re[i] == '(') balance++;
            if (re[i] == ')') balance--;
            if (balance == 0) return i;
        }
        return -1;
    }

    void printTransitions() {
        for (auto& state : states) {
            string stateIdFormat = (state == startState) ? "`q" : "q";
            if (state == rejectState) {
                stateIdFormat = "reject"; // Change ID format for rejects
            } else {
                stateIdFormat += to_string(state->id);
            }
            if (state->isFinal) {
                stateIdFormat = "[" + stateIdFormat + "]"; // Mark final states
            }
            for (auto& transition : state->transitions) {
                string nextStateIdFormat;
                if (transition.second == rejectState) {
                    nextStateIdFormat = "reject"; // rejects
                } else {
                    nextStateIdFormat = "q" + to_string(transition.second->id);
                }
                if (transition.second->isFinal && nextStateIdFormat != "reject") {
                    nextStateIdFormat = "[" + nextStateIdFormat + "]"; // Mark final states
                }
                cout << "(" << stateIdFormat << ", " << transition.first << ") -> " << nextStateIdFormat << endl;
            }
        }
    }
};

int main() {
    DFA dfa;
    string re;
    cout << "Welcome to RE to DFA machine. Use U for intersection, * for Kleene Star. ` means its a start state and [ ] means its an accept state" << endl;
    while (true) {
        cout << "Enter a RE or type STOP to end: ";
        getline(cin, re);
        if (re == "STOP") break;

        dfa.reset(); // Reset DFA for new pattern
        dfa.addPattern(re); // to add pattern
        dfa.printTransitions(); // prints it
    }
    return 0;
}
