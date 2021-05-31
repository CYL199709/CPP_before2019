#pragma once
//QT
#include <QtWidgets/QMainWindow>
#include "ui_The_result_gui.h"
#include<qlabel.h>
#include<qtextedit.h>
#include<QGraphicsScene>  
#include<QGraphicsView>  
#include<QProcess>
//Opencv
#include<opencv2/opencv.hpp>  
#include<iostream>  
#include<vector>  
#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>


class The_result_gui : public QMainWindow
{
	Q_OBJECT

public:
	The_result_gui(QWidget *parent = Q_NULLPTR);

private:
	Ui::The_result_guiClass ui;
private slots:    //ÉùÃ÷ÐÅºÅº¯Êý  
	void find_image();
	void find_digital();
	void over();
	void blur_image();

};
