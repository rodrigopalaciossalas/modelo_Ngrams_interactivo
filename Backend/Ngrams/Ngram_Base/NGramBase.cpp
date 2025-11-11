#include "NGramBase.hpp"
#include <iostream>

NGramBase::NGramBase(int n_valor) {
    n = n_valor;
    cant = 0;
}

NGramBase::~NGramBase() {}

int NGramBase::obN() const {
    return n;
}

int NGramBase::obCant() const {
    return cant;
}