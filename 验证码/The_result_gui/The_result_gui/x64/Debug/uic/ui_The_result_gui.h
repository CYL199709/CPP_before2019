/********************************************************************************
** Form generated from reading UI file 'The_result_gui.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_THE_RESULT_GUI_H
#define UI_THE_RESULT_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_The_result_guiClass
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;

    void setupUi(QMainWindow *The_result_guiClass)
    {
        if (The_result_guiClass->objectName().isEmpty())
            The_result_guiClass->setObjectName(QString::fromUtf8("The_result_guiClass"));
        The_result_guiClass->resize(719, 647);
        The_result_guiClass->setWindowTitle(QString::fromUtf8("\350\277\220\350\241\214\347\252\227\345\217\243"));
        The_result_guiClass->setStyleSheet(QString::fromUtf8("background-image: url(:/new_image/C:/Users/14587/Desktop/\345\256\214\346\210\220/\350\203\214\346\231\2576.jpg);"));
        The_result_guiClass->setToolButtonStyle(Qt::ToolButtonIconOnly);
        The_result_guiClass->setAnimated(true);
        The_result_guiClass->setDockOptions(QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks);
        centralWidget = new QWidget(The_result_guiClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(110, 120, 291, 81));
        pushButton->setStyleSheet(QString::fromUtf8("color:black;\n"
"font: 9pt \"Agency FB\";\n"
"font: 75 18pt \"Agency FB\";"));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(110, 270, 291, 81));
        pushButton_2->setFocusPolicy(Qt::StrongFocus);
        pushButton_2->setContextMenuPolicy(Qt::CustomContextMenu);
        pushButton_2->setStyleSheet(QString::fromUtf8("color:black;\n"
"font: 75 18pt \"Agency FB\";"));
        pushButton_2->setCheckable(false);
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(110, 420, 291, 81));
        pushButton_3->setStyleSheet(QString::fromUtf8("color:black;\n"
"font: 75 18pt \"Agency FB\";"));
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(460, 340, 161, 81));
        pushButton_4->setStyleSheet(QString::fromUtf8("color:black;\n"
"font: 75 18pt \"Agency FB\";"));
        pushButton_4->setAutoDefault(false);
        The_result_guiClass->setCentralWidget(centralWidget);

        retranslateUi(The_result_guiClass);
        QObject::connect(pushButton, SIGNAL(clicked()), The_result_guiClass, SLOT(blur_image()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), The_result_guiClass, SLOT(find_image()));
        QObject::connect(pushButton_3, SIGNAL(clicked()), The_result_guiClass, SLOT(find_digital()));
        QObject::connect(pushButton_4, SIGNAL(clicked()), The_result_guiClass, SLOT(over()));

        QMetaObject::connectSlotsByName(The_result_guiClass);
    } // setupUi

    void retranslateUi(QMainWindow *The_result_guiClass)
    {
        pushButton->setText(QApplication::translate("The_result_guiClass", "Image Enhancement", nullptr));
        pushButton_2->setText(QApplication::translate("The_result_guiClass", "Image Recognition", nullptr));
        pushButton_3->setText(QApplication::translate("The_result_guiClass", "Verification Code Identification", nullptr));
        pushButton_4->setText(QApplication::translate("The_result_guiClass", "Close Windows", nullptr));
        Q_UNUSED(The_result_guiClass);
    } // retranslateUi

};

namespace Ui {
    class The_result_guiClass: public Ui_The_result_guiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THE_RESULT_GUI_H
