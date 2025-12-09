#include "claculator.h"
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QStack>
#include <cmath>
#include <QMessageBox>

/*-------------------- Constructor --------------------*/
Calculator::Calculator(QWidget *parent)
    : QWidget(parent)
{
    display = new QLineEdit;
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(display);

    QGridLayout *grid = new QGridLayout;

    QString buttons[6][4] = {
        {"C", "←", "%", "/"},
        {"7", "8", "9", "*"},
        {"4", "5", "6", "-"},
        {"1", "2", "3", "+"},
        {"0", ".", "+/-", "="},
        {"(", ")", "√", "^"}
    };

    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            QPushButton *btn = new QPushButton(buttons[row][col]);
            grid->addWidget(btn, row, col);

            connect(btn, &QPushButton::clicked, [=]() {
                handleButton(btn->text());
            });
        }
    }

    mainLayout->addLayout(grid);
}

/*-------------------- Operator Precedence --------------------*/
int Calculator::precedence(QChar op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

/*-------------------- Convert Infix → Postfix --------------------*/
QString Calculator::infixToPostfix(const QString &expr)
{
    QString output;
    QStack<QChar> stack;

    for (int i = 0; i < expr.length(); ++i) {
        QChar c = expr[i];

        // رقم أو نقطة
        if (c.isDigit() || c == '.') {
            output += c;
        }
        // قوس مفتوح
        else if (c == '(') {
            stack.push(c);
        }
        // قوس مغلق
        else if (c == ')') {
            output += " ";
            while (!stack.isEmpty() && stack.top() != '(') {
                output += stack.pop();
                output += " ";
            }
            if (!stack.isEmpty()) stack.pop(); // إزالة '('
        }
        // العمليات
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            output += " ";
            while (!stack.isEmpty() && precedence(stack.top()) >= precedence(c)) {
                output += stack.pop();
                output += " ";
            }
            stack.push(c);
        }
    }

    output += " ";
    while (!stack.isEmpty()) {
        output += stack.pop();
        output += " ";
    }

    return output;
}

/*-------------------- Evaluate Postfix --------------------*/
double Calculator::evalPostfix(const QString &postfix)
{
    QStack<double> st;
    QString num;

    for (int i = 0; i < postfix.length(); ++i) {
        QChar c = postfix[i];

        // بناء الرقم
        if (c.isDigit() || c == '.' || c == '-') {
            num += c;
        }
        // نهاية الرقم
        else if (c == ' ') {
            if (!num.isEmpty()) {
                st.push(num.toDouble());
                num.clear();
            }
        }
        // العمليات
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            if (st.size() < 2) {
                QMessageBox::warning(this, "Error", "Invalid Expression");
                return 0;
            }
            double b = st.pop();
            double a = st.pop();

            if (c == '+') st.push(a + b);
            else if (c == '-') st.push(a - b);
            else if (c == '*') st.push(a * b);
            else if (c == '/') st.push(a / b);
            else if (c == '^') st.push(pow(a, b));
        }
    }

    if (!num.isEmpty()) st.push(num.toDouble());
    if (st.size() != 1) {
        QMessageBox::warning(this, "Error", "Invalid Expression");
        return 0;
    }

    return st.pop();
}

/*-------------------- Handle Button Press --------------------*/
void Calculator::handleButton(const QString &text)
{
    if (text == "=") {
        QString expr = display->text();
        QString postfix = infixToPostfix(expr);
        double result = evalPostfix(postfix);
        display->setText(QString::number(result));
        newInput = true;
    }
    else if (text == "C") {
        display->clear();
        newInput = true;
    }
    else if (text == "←") {
        QString t = display->text();
        if (!t.isEmpty()) t.chop(1);
        display->setText(t);
    }
    else if (text == "+/-") {
        double v = display->text().toDouble();
        display->setText(QString::number(-v));
    }
    else if (text == "%") {
        double v = display->text().toDouble() / 100.0;
        display->setText(QString::number(v));
    }
    else if (text == "√") {
        double v = display->text().toDouble();
        display->setText(QString::number(sqrt(v)));
    }
    else {
        if (newInput) {
            display->setText(text);
            newInput = false;
        } else {
            display->setText(display->text() + text);
        }
    }
}

/*-------------------- Destructor --------------------*/
Calculator::~Calculator()
{
}
