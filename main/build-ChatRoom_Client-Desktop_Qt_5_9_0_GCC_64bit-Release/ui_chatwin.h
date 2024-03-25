/********************************************************************************
** Form generated from reading UI file 'chatwin.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATWIN_H
#define UI_CHATWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_chatwin
{
public:
    QTextBrowser *textBrowser;
    QTextEdit *textEdit;
    QPushButton *pushButton;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton_2;
    QProgressBar *progressBar;

    void setupUi(QDialog *chatwin)
    {
        if (chatwin->objectName().isEmpty())
            chatwin->setObjectName(QStringLiteral("chatwin"));
        chatwin->resize(471, 485);
        textBrowser = new QTextBrowser(chatwin);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(30, 60, 411, 271));
        textEdit = new QTextEdit(chatwin);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(30, 350, 411, 81));
        pushButton = new QPushButton(chatwin);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(350, 440, 89, 25));
        label = new QLabel(chatwin);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 10, 281, 17));
        label_2 = new QLabel(chatwin);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 30, 281, 17));
        pushButton_2 = new QPushButton(chatwin);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(250, 440, 89, 25));
        progressBar = new QProgressBar(chatwin);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(30, 440, 201, 23));
        progressBar->setValue(24);

        retranslateUi(chatwin);

        QMetaObject::connectSlotsByName(chatwin);
    } // setupUi

    void retranslateUi(QDialog *chatwin)
    {
        chatwin->setWindowTitle(QApplication::translate("chatwin", "Dialog", Q_NULLPTR));
        pushButton->setText(QApplication::translate("chatwin", "\345\217\221\351\200\201", Q_NULLPTR));
        label->setText(QApplication::translate("chatwin", "1", Q_NULLPTR));
        label_2->setText(QApplication::translate("chatwin", "2", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("chatwin", "\345\217\221\351\200\201\346\226\207\344\273\266", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class chatwin: public Ui_chatwin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATWIN_H
