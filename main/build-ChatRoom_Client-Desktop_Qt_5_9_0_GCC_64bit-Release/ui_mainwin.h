/********************************************************************************
** Form generated from reading UI file 'mainwin.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIN_H
#define UI_MAINWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_mainwin
{
public:
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton;
    QLineEdit *lineEdit;
    QPushButton *pushButton_2;
    QListView *listView;
    QLabel *label_3;
    QPushButton *pushButton_3;
    QListView *listView_2;
    QPushButton *pushButton_4;
    QLineEdit *lineEdit_2;

    void setupUi(QDialog *mainwin)
    {
        if (mainwin->objectName().isEmpty())
            mainwin->setObjectName(QStringLiteral("mainwin"));
        mainwin->resize(640, 480);
        label = new QLabel(mainwin);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 50, 361, 17));
        label_2 = new QLabel(mainwin);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 70, 371, 17));
        pushButton = new QPushButton(mainwin);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(350, 20, 89, 25));
        lineEdit = new QLineEdit(mainwin);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(40, 20, 181, 25));
        pushButton_2 = new QPushButton(mainwin);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(240, 20, 89, 25));
        listView = new QListView(mainwin);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setGeometry(QRect(30, 120, 281, 331));
        label_3 = new QLabel(mainwin);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(40, 90, 371, 17));
        pushButton_3 = new QPushButton(mainwin);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(360, 120, 231, 21));
        listView_2 = new QListView(mainwin);
        listView_2->setObjectName(QStringLiteral("listView_2"));
        listView_2->setGeometry(QRect(360, 150, 231, 281));
        pushButton_4 = new QPushButton(mainwin);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(500, 440, 89, 25));
        lineEdit_2 = new QLineEdit(mainwin);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(360, 440, 131, 25));

        retranslateUi(mainwin);

        QMetaObject::connectSlotsByName(mainwin);
    } // setupUi

    void retranslateUi(QDialog *mainwin)
    {
        mainwin->setWindowTitle(QApplication::translate("mainwin", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("mainwin", "1", Q_NULLPTR));
        label_2->setText(QApplication::translate("mainwin", "2", Q_NULLPTR));
        pushButton->setText(QApplication::translate("mainwin", "\345\210\267\346\226\260", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("mainwin", "\347\231\273\351\231\206", Q_NULLPTR));
        label_3->setText(QApplication::translate("mainwin", "3", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("mainwin", "\347\276\244\350\201\212", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("mainwin", "\345\210\233\345\273\272\347\276\244\350\201\212", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class mainwin: public Ui_mainwin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIN_H
