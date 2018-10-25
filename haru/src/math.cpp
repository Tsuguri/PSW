#include "math.hpp"

MathFormula::MathFormula(QObject *parent) : QObject(parent),
    text{"0.0"}, valid{true}, t{0.0} {
    parser.DefineVar("t", &t);
    parser.SetExpr("0.0");
}

void MathFormula::setText(const QString &text) {
    this->text = text;
    emit textChanged();

    buildParser();
}

double MathFormula::evaluate(double x) {
    t = x;
    return parser.Eval();
}

void MathFormula::setValid(bool valid) {
    if(this->valid != valid) {
        this->valid = valid;
        emit validChanged();
    }
}

void MathFormula::buildParser() {
    using namespace mu;
    try {
        parser.SetExpr(text.toStdString());
        parser.Eval();
        setValid(true);
    } catch(Parser::exception_type &e) {
        setValid(false);
    }
}



