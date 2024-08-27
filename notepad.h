#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>

#include <QVBoxLayout>
#include <QTextEdit>

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class NotePad; }
QT_END_NAMESPACE

class NotePad : public QMainWindow
{
    Q_OBJECT

public:
    NotePad(QWidget *parent = nullptr);
    ~NotePad();

private slots:

    void newFile();

    void open();

    void save();

    void saveAs();

    void print();

    void copy();

    void cut();

    void paste();

    void undo();

    void redo();

    void about();

    void exit();

    void onTextChanged();

private:

    void createActions();
    void updateActions();
    void updateCopy(bool yes);
    void updateRedo(bool available);
    void updateUndo(bool available);

    Ui::NotePad *ui;
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;

    QString currentFile = "";

    QAction *copyAct;
    QAction *redoAct;
    QAction *undoAct;
    QAction *pasteAct;
    QAction *cutAct;
};
#endif // NOTEPAD_H
