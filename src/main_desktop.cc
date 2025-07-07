#include "gui/desktop/brickgame/mainwindow.h"

 using namespace s21;

 // namespace s21 {

 int main(int argc, char *argv[]) {
   QApplication a(argc, argv);
   MainWindow w;
   QObject::connect(&w, &MainWindow::exit, &a, &QApplication::quit);
   w.show();
   return a.exec();
 };

 // } // namespace s21
