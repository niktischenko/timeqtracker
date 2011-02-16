#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	timer = new QTimer(this);
	timer->setSingleShot(false);
	timer->setInterval(1000);
	model = new QStandardItemModel(0, 2, this);
	clear();

	QString saveFileName = QDir::homePath()+QDir::separator()+".timeqtracker_save";
	saveFile.setFileName(saveFileName);
	if (saveFile.exists()) {
		restore();
	}

	connect(ui->newButton, SIGNAL(clicked()), this, SLOT(newTask()));
	connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stop()));
	connect(ui->hideButton, SIGNAL(clicked()), this, SLOT(hideTable()));
	connect(ui->showButton, SIGNAL(clicked()), this, SLOT(showTable()));
	connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(clear()));
	connect(ui->taskNameEdit, SIGNAL(textEdited(QString)), this, SLOT(renameCurrentTask(QString)));
}

MainWindow::~MainWindow()
{
	delete timer;
	delete ui;
}

void MainWindow::newTask()
{
	stop();
	connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
	currentTaskTime = QTime(0, 0);
	currentTaskName = "new task";
	ui->taskNameEdit->setText(currentTaskName);
	model->setRowCount(model->rowCount()+1);
	updateCurrentTask();
	timer->start();
}

void MainWindow::stop()
{
	disconnect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
	timer->stop();
}

void MainWindow::hideTable()
{
	ui->widget->hide();
	setMaximumHeight(37);
}

void MainWindow::showTable()
{
	ui->widget->show();
}

void MainWindow::clear()
{
	stop();
	model->clear();
	QStringList headers;
	headers << "Task" << "Time";
	model->setHorizontalHeaderLabels(headers);
	ui->tableView->setModel(model);
	ui->tableView->setColumnWidth(0, 450);
	ui->tableView->setColumnWidth(1, 110);
	if (saveFile.exists()) {
		saveFile.remove();
	}
}

void MainWindow::renameCurrentTask(QString name)
{
	currentTaskName = name;
	updateCurrentTask();
}

void MainWindow::updateTime()
{
	currentTaskTime = currentTaskTime.addSecs(1);
	ui->taskTime->setText(currentTaskTime.toString("HH:mm:ss"));
	updateCurrentTask();
	if (currentTaskTime.second() == 0) {
		save();
	}
	timer->start();
}

void MainWindow::updateCurrentTask()
{
	int id = model->rowCount() - 1;
	if (id < 0) {
		return;
	}
	model->setItem(id, 0, new QStandardItem(currentTaskName));
	model->setItem(id, 1, new QStandardItem(currentTaskTime.toString("HH:mm:ss")));
}

void MainWindow::restore()
{
	saveFile.open(QFile::ReadOnly);
	QTextStream stream(&saveFile);
	QString s_count;
	QString name;
	QString time;
	int count;
	s_count = stream.readLine();
	count = s_count.toInt();
	for (int i = 0; i < count; i++) {
		name = stream.readLine();
		time = stream.readLine();
		model->setRowCount(i+1);
		model->setItem(i, 0, new QStandardItem(name));
		model->setItem(i, 1, new QStandardItem(time));
	}
	currentTaskName = name;
	ui->taskNameEdit->setText(name);
	currentTaskTime = QTime::fromString(time, "HH:mm:ss");
	connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
	timer->start();
	saveFile.close();
}

void MainWindow::save()
{
	saveFile.remove();
	saveFile.open(QFile::WriteOnly);
	QTextStream stream(&saveFile);
	int count = model->rowCount();
	stream << count << "\n";
	for (int i = 0; i < count; i++) {
		stream << model->index(i, 0).data().toString() << "\n";
		stream << model->index(i, 1).data().toString() << "\n";
	}
	stream << "\n";
	saveFile.close();
}
