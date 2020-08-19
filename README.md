# cppformlang

A C++ library to manipulate formal grammar. In general, it can be used to better understand algorithms in a formal way.

## Installation

## Sources

Most algorithms come from Introduction to *Automata Theory, Languages, and Computation*
(2nd edition) by John E. Hopcroft, Rajeev Motwani and Jeferey D. Ullman.

Indexed grammars come from the original paper *Index Grammars - An Extension of Context-free grammars* by Alfred V. Aho.

On the implementation of Hopcroft minimization algorithm: [Implementation of Hopcroft's Algorithm, Hang Zhou](https://www.irif.fr/~carton/Enseignement/Complexite/ENS/Redaction/2009-2010/hang.zhou.pdf)

[Intersection CFG/Regex](https://www.degruyter.com/downloadpdf/j/stuf.1961.14.issue-1-4/stuf.1961.14.14.143/stuf.1961.14.14.143.pdf) and [a better written version](http://www.cs.umd.edu/~gasarch/BLOGPAPERS/cfg.pdf)

## Usage

### Regular expression

#### Non Deterministic Automata

The representation of non deterministic automata, i.e. automata with possibly several next states at each stage but no epsilon transitions.

#### Epsilon Non Deterministic Automata

It represents a non deterministic automaton where epsilon transitions are allowed.


### Regex and Finite Automaton

As regex and finite automaton are equivalent, one can turn one into the other.


### Context-Free Grammar

We represent here context-free grammars. Like finite automata, one needs to use the classes *pyformlang.cfg.Variable* and *pyformlang.cfg.Terminal* to represent variables and terminals. The productions need to be represented as *pyformlang.cfg.Production*. In addition, epsilon terminals are members of *pyformlang.cfg.Epsilon*.

### CFG and PDA

As CFG and PDA are equivalent, one can turn one into the other, but needs to be careful about whether the PDA accepts on empty stack and final state. The conversions between CFG and PDA are done when the PDA is accepting by empty stack

### Indexed Grammars

Indexed grammars are grammars which have a stack which can be duplicated. In an indexed grammar, rules can take 4 forms (sigma is the stack):

* *EndRule*: This simply turns a Variable into a terminal, for example A[sigma]->a
* *ProductionRule*: We push something on the stack, for example A[sigma]->B[f sigma]
* *ConsumptionRule*: We consume something from the stack, for example A[f sigma] -> C[sigma]
* *DuplicationRule*: We duplicate the stack, for example A[sigma] -> B[sigma] C[sigma]
