#ifndef MATH_HPP
#define MATH_HPP

#include <QObject>
#include <QString>

#include <muParser.h>

class MathFormula : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
    Q_PROPERTY(bool valid READ getValid NOTIFY validChanged)
public:
    explicit MathFormula(QObject *parent = nullptr);

    QString getText() const { return text; }

    bool getValid() const { return valid; }

    void setText(const QString &text);

    double evaluate(double x);

signals:
    void textChanged();
    void validChanged();

private:
    QString text;
    bool valid;

    mu::Parser parser;
    double t;

    void setValid(bool valid);

    void buildParser();

};


#endif //MATH_HPP
