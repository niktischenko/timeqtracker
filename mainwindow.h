#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QFile>
#include <QDir>
#include <QTextStream>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void newTask();
	void stop();
	void hideTable();
	void showTable();
	void clear();

private slots:
	void updateTime();
	void renameCurrentTask(QString name);

private:
	Ui::MainWindow *ui;
	QTime currentTaskTime;
	QTimer *timer;
	QString currentTaskName;
	QStandardItemModel *model;

	void updateCurrentTask();
	void restore();
	void save();
	QFile saveFile;
};

#endif // MAINWINDOW_H
