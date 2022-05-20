#include "mainwindow.h"
#include <QPushButton>




unsigned long long MainWindow::getDirSize(char *dir_name, struct Node* before){                     //Function for traversal and DataStructure Building


    unsigned long long dir_size=0;


    DIR *current_dir;

    if ((current_dir =opendir(dir_name)) != NULL){

    struct dirent *new_dir;


    while ((new_dir = readdir(current_dir)) !=NULL){

        char path[10000];


        strcat(strcat(strcpy(path, dir_name), strcmp(dir_name, "/")!= 0 ?"/":""), new_dir->d_name);


        struct Node *cur_node= new Node;

        cur_node->size=0;

        struct stat status;


        if (new_dir->d_type != DT_DIR){


            if (strcmp(new_dir->d_name,".")!=0 && strcmp(new_dir->d_name,"..")!=0){


                lstat(path,&status);

                if(!S_ISLNK(status.st_mode)&& S_ISREG(status.st_mode))                  //Checks whether the current node is a File and not a symoblic link, then we start
                                                                                        //Allocating the nodes in the DataStructure

                {

                    cur_node->size = status.st_size;


                   dir_size+=cur_node->size;
                   cur_node->type = 'F';

                    strcpy(cur_node->key,path);

                    before->next.push_back(cur_node);

                }


    }

}

else if(new_dir->d_type == DT_DIR)                                                      //Checks whether the current node is a Directory, we keep on recursing till
                                                                                        //till we reach the file level, while adding the sizes of directories
{

if(strcmp(new_dir->d_name,".") != 0 && strcmp(new_dir->d_name,"..") != 0 && strcmp(new_dir->d_name,"proc")!=0)  //Proc condition is to ignore the Proc file

{

    lstat(path,&status);

    if (!S_ISLNK(status.st_mode)&& S_ISDIR(status.st_mode)){    //Checks whether the current node is a Directory, we keep on recursing till
                                                                 //till we reach the file level, while adding the sizes of directories

        cur_node->size+=getDirSize(path,cur_node);

        dir_size+=cur_node->size;
        cur_node->type = 'D';
        strcpy(cur_node->key,path);

        before->next.push_back(cur_node);

                      }

              }
          }

        }

closedir(current_dir);

}

return dir_size;


}

vector<Node*> MainWindow::Traversal(Node * node)

{

return node->next ;                                                             //returns the vector containing the next level of the current node


}
bool compareInterval(Node * i1, Node * i2)
{
    return (i1->size > i2->size);                                               //Used for vector sorting
}

void MainWindow::DrawPieChart(Node *current)

{
    counter = 0 ;
    series = new QPieSeries();
    temp = Traversal(current);
    sort(temp.begin() , temp.end() , compareInterval);


     for (int i=0;i<temp.size(); i++){                                          //Creating PieChart Slices

       temp[i]->prev = current;                                                 //Saving the current directory as a prev for the next directories as in case of backbutton usages
       double n = temp[i]->size / float(current->size);

       series->append((temp[i])->key, n) ;
       counter++;

    }


     for (int i =0; i <counter ; i++)

       {

         QPieSlice *slice2 = series->slices().at(i);                            //Editing Slices of the PieChart

           slice2->setLabelVisible(false);
           QString tep ="";
           tep+=temp[i]->key;
           tep+=":  ";
           float g = temp[i]->size / float(current->size) * 100;
           if (g > 1)
           {
           tep +=QString::number(g, 'f', 1);
           }
           else if (g<1)
           {
           tep += "less than 1";
           }
           else if (g==0)
           {
           tep += QString::number(0);
           }
           tep += "%";
           slice2->setLabel(tep);

       }


     QChart *chart = new QChart();


     chart->addSeries(series);                                              //Displaying the ChartWindow
     chart->setTheme(QChart::ChartThemeLight);
     chart->setAnimationOptions(QChart::AllAnimations);
     chart->legend()->setVisible(true);
     chart->legend()->setAlignment(Qt::AlignRight);

     QString z ="";
     z+="Current Directory: ";
     z+= current->key;
     chart->setTitle(z);

    QChartView *chartView = new QChartView(chart);
    ui->horizontalLayout->addWidget(chartView,0,0);
    chartView->setRenderHint(QPainter::Antialiasing);


   for (int i =0 ; i < temp.size() ; i++){
       QListWidgetItem* item = new QListWidgetItem(QIcon(":/rec/F.png") , temp[i]->key);    //Creating the list for the directories
       ui->listWidget->addItem(item);

     }


  }



void MainWindow::on_pushButton_clicked()                //Scan button handling

{
  Node * n = new Node;
  QString t =  ui->listWidget->currentItem()->text();   //retreiving the clicked directory
{
      for (int i = 0 ; i < temp.size() ; i++ )          //Scanning the current level of directories
      {
          if(t == temp[i]->key && temp[i]->type == 'D') //if a match is a found, is of directory type, and is not empty, we process the node to print and use
          {                                             //the next level of directories, while pushing the prev in the stack in order for using the back button scenarios
              if(temp[i]->next.size() !=0)
              {                                         //We Deleted the current PieChart, empty the list, draw a new one, and list all the upcoming directories
              strcpy(n->key , temp[i]->key);
              n->size = temp[i]->size;
              n->next = temp[i]->next;
              Back.push(temp[i]->prev);
              ui->listWidget->clear();
              qDeleteAll(ui->horizontalLayout->children());
              QLayoutItem* child;
              child = ui->horizontalLayout->takeAt(0);
              while(child !=0)
              {
                  ui->horizontalLayout->removeWidget(child->widget());
                  delete child;
                  child = ui->horizontalLayout->takeAt(0);



              }
            DrawPieChart(n);
              }
              else                                  //If the folder is empty, we notify the user that the folder is empty
                  QMessageBox::information(this, "Message Box" , "Folder is Empty");
          }
          else if (t == temp[i]->key && temp[i]->type == 'F')   //If the current node is a file, we notify the user that this is the end of the directory
          {
            QMessageBox::information(this, "Message Box" , "End of Directory");
            break;
          }

       }

  }

}


MainWindow::MainWindow(QWidget *parent) :

    QMainWindow(parent),

    ui(new Ui::MainWindow)

{


    ui->setupUi(this);

    Node *root = new Node;

   char temp1[] ="/";

   unsigned long long off=getDirSize(temp1, root);

   root->size=off;
   strcpy(root->key , temp1);
   root->type = 'D';
   root->prev=NULL;
    DrawPieChart(root);                                     //We call the DrawPieChart Function for the first time with the root node ('/')

}


MainWindow::~MainWindow()

{

    delete ui;

}





void MainWindow::on_pushButton_2_clicked()          //Once the back button is Clicked, the stacks pops the most recently visited directory, and is eligible of going back till the stack
{                                                   //Is Empty, or in the other words, when the user goes back to the "/" directory
    Node *n = new Node;
    if(!Back.empty())
    {
      n = Back.top();
      Back.pop();
      qDeleteAll(ui->horizontalLayout->children());
      QLayoutItem* child;
      child = ui->horizontalLayout->takeAt(0);
      while(child !=0)
      {
          ui->horizontalLayout->removeWidget(child->widget());
          delete child;
          child = ui->horizontalLayout->takeAt(0);
      }
      ui->listWidget->clear();
      DrawPieChart(n);
    }
}
