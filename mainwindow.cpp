#include "mainwindow.h"
#include "ui_mainwindow.h"



#include <QImage>
#include <QDebug>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    FSupdateButtonState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateDatabaseInfo()
{
    ui->FScomboBox->clear();
    for(unsigned int i=1; i<=database.getNoFeatures(); ++i)
        ui->FScomboBox->addItem(QString::number(i));

    ui->FStextBrowserDatabaseInfo->setText("noClass: " +  QString::number(database.getNoClass()));
    ui->FStextBrowserDatabaseInfo->append("noObjects: "  +  QString::number(database.getNoObjects()));
    ui->FStextBrowserDatabaseInfo->append("noFeatures: "  +  QString::number(database.getNoFeatures()));

}

void MainWindow::FSupdateButtonState(void)
{
    if(database.getNoObjects()==0)
    {
        FSsetButtonState(false);
    }
    else
        FSsetButtonState(true);

}


void MainWindow::FSsetButtonState(bool state)
{
   ui->FScomboBox->setEnabled(state);
   ui->FSpushButtonCompute->setEnabled(state);
   ui->FSpushButtonSaveFile->setEnabled(state);
   ui->FSradioButtonFisher->setEnabled(state);
   ui->FSradioButtonSFS->setEnabled(state);
}

void MainWindow::on_FSpushButtonOpenFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open TextFile"), "", tr("Texts Files (*.txt)"));

    if ( !database.load(fileName.toStdString()) )
        QMessageBox::warning(this, "Warning", "File corrupted !!!");
    else
        QMessageBox::information(this, fileName, "File loaded !!!");

    FSupdateButtonState();
    updateDatabaseInfo();
}

void MainWindow::on_FSpushButtonCompute_clicked()
{
    int dimension = ui->FScomboBox->currentText().toInt();


    if( ui->FSradioButtonFisher ->isChecked())
    {
        std::vector<std::vector<int>> combianationsToCheck = Fisher::getAllCombinations(database.getNoFeatures(), dimension);
        for(auto combination : combianationsToCheck) {
            //First we compute avg matrix
            boost::numeric::ublas::matrix<float> avg_mat = boost::numeric::ublas::matrix<float>(dimension, database.getNoObjects(), 0);
            for(int combination_iter = 0; combination_iter < dimension; combination_iter++) {
                int featureNo = combination[combination_iter];
                for(int objectNo = 0; objectNo < database.getNoObjects(); objectNo++) {
                    avg_mat(combination_iter, 1) = avg_mat(combination_iter, 1) + database.getObjects()[objectNo].getFeatures()[featureNo];
                }
                avg_mat(combination_iter, 1) = avg_mat(combination_iter, 1) / database.getNoObjects();
                //Extending dimension because no matlab repeat fak
                for(int i = 0; i < dimension; i++) {
                    for(int j = 1; j < database.getNoObjects(); j++) {
                        avg_mat(i, j) = avg_mat(i, 1);
                    }
                }
            }
            //At this moment avg_mat is defined as follows: avg_mat(order_of_feature_in_combination, 1)
            //So you can use it as follows avg_mat(order_of_feature_in_combination, 1) == average of fature with number defined in combination[order_of_feature_in_combination]
            //Lets compute covariance matrix
            //First lets assemble BFM - Big Freakin Matrix
            boost::numeric::ublas::matrix<float> feature_value_mat = boost::numeric::ublas::matrix<float>(dimension, database.getNoObjects());
            for(int combination_iter = 0; combination_iter < dimension; combination_iter++) {
                for(int objectNo = 0; objectNo < database.getNoObjects(); objectNo++) {
                    int featureNo = combination[combination_iter];
                    feature_value_mat(combination_iter, objectNo) = database.getObjects()[objectNo].getFeatures()[featureNo];
                }
                boost::numeric::ublas::matrix<float> deta_mat = feature_value_mat - avg_mat;

            }

        }
    }
}



void MainWindow::on_FSpushButtonSaveFile_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
    tr("Open TextFile"), "D:\\Users\\Krzysiu\\Documents\\Visual Studio 2015\\Projects\\SMPD\\SMPD\\Debug\\", tr("Texts Files (*.txt)"));

        QMessageBox::information(this, "My File", fileName);
        database.save(fileName.toStdString());
}

void MainWindow::on_PpushButtonSelectFolder_clicked()
{
}

void MainWindow::on_CpushButtonOpenFile_clicked()
{

}

void MainWindow::on_CpushButtonSaveFile_clicked()
{

}

void MainWindow::on_CpushButtonTrain_clicked()
{

}

void MainWindow::on_CpushButtonExecute_clicked()
{

}
