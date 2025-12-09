#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QLineEdit>

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:
    QLineEdit *display;            // شاشة العرض

    double currentNumber = 0;      // الرقم الحالي (مش مستخدم كتير هنا)
    QString currentOperation = ""; // العملية الحالية (مش مستخدم كتير هنا)
    bool newInput = true;          // هل المستخدم بدأ إدخال رقم جديد

    void handleButton(const QString &text); // دالة التعامل مع كل زرار

    // لدعم parsing للتعبيرات
    int precedence(QChar op);              // ترتيب العمليات
    QString infixToPostfix(const QString &expr); // تحويل التعبير من infix لـ postfix
    double evalPostfix(const QString &postfix);  // تقييم التعبير بعد تحويله
};

#endif // CALCULATOR_H
