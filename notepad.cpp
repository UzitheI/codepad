#include "notepad.h"
#include "ui_notepad.h"

NotePad::NotePad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NotePad)
    , verticalLayout(new QVBoxLayout)
    , textEdit(new QTextEdit)

{
    ui->setupUi(this);
    setCentralWidget(textEdit);
    setWindowTitle("Untitled - NotePad");
    createActions();
    connect(textEdit,&QTextEdit::textChanged,this,&NotePad::onTextChanged);
    connect(textEdit,&QTextEdit::copyAvailable,this,&NotePad::updateCopy);
    connect(textEdit,&QTextEdit::redoAvailable,this,&NotePad::updateRedo);
    connect(textEdit,&QTextEdit::undoAvailable,this,&NotePad::updateUndo);
}

NotePad::~NotePad()
{
    delete ui;
}

void NotePad::updateCopy(bool yes)
{
    copyAct->setEnabled(yes);
    cutAct->setEnabled(yes);
}

void NotePad::updateRedo(bool available)
{
    redoAct->setEnabled(available);
}

void NotePad::updateUndo(bool available)
{
    undoAct->setEnabled(available);
}

void NotePad::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *newAct = fileMenu->addAction(tr("&New"), this, &NotePad::newFile);
    newAct->setShortcut(QKeySequence::New);

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &NotePad::open);
    openAct->setShortcut(QKeySequence::Open);

    QAction *saveAct = fileMenu->addAction(tr("&Save"), this, &NotePad::save);
    saveAct->setShortcut(QKeySequence::Save);

    QAction *saveAsAct = fileMenu->addAction(tr("&Save As..."), this, &NotePad::saveAs);
    saveAsAct->setShortcut(QKeySequence::SaveAs);

    QAction *printAct = fileMenu->addAction(tr("&Print..."), this, &NotePad::print);
    printAct->setShortcut(QKeySequence::Print);

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("&Exit"), this, &QWidget::close);
    exitAct->setShortcut(QKeySequence::Quit);

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    undoAct = editMenu->addAction(tr("&Undo"), this, &NotePad::undo);
    undoAct->setShortcut(QKeySequence::Undo);
    undoAct->setEnabled(false);

    redoAct = editMenu->addAction(tr("&Redo"), this, &NotePad::redo);
    redoAct->setShortcut(QKeySequence::Redo);
    redoAct->setEnabled(false);

    editMenu->addSeparator();

    copyAct = editMenu->addAction(tr("&Copy"), this, &NotePad::copy);
    copyAct->setShortcut(QKeySequence::Copy);
    copyAct->setEnabled(false);

    cutAct = editMenu->addAction(tr("&Cut"), this, &NotePad::cut);
    cutAct->setShortcut(QKeySequence::Cut);
    cutAct->setEnabled(false);

    pasteAct = editMenu->addAction(tr("&Paste"), this, &NotePad::paste);
    pasteAct->setShortcut(QKeySequence::Paste);
    pasteAct->setEnabled(true);


    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction(tr("&About"), this, &NotePad::about);
    helpMenu->addAction(tr("About &Qt"), this, &QApplication::aboutQt);
}


void NotePad::updateActions()
{
   // saveAsAct->setEnabled(!textEdit->toPlainText().isEmpty());
}

void NotePad::newFile()
{
    currentFile.clear();
    setWindowTitle("NotePad - Untitled");
    textEdit->setText(QString());
}

void NotePad::open()
{

    QString filename = QFileDialog::getOpenFileName(this,"Open the file");
    QFile file(filename);
    currentFile = filename;
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,"Warning","Cannot open file : "+file.errorString());
        return;
    }
    setWindowTitle(filename);
    QTextStream in(&file);
    QString text = in.readAll();
    textEdit->setText(text);
    file.close();
}

void NotePad::save()
{
    if(currentFile.isEmpty())
    {
        saveAs();
    }
    else
{
    QFile file(currentFile);
    if(!file.open(QFile::WriteOnly |QFile::Text))
    {
        QMessageBox::warning(this,"Warning","Cannot save file : "+file.errorString());
        return;
    }
    setWindowTitle(currentFile+" - NotePad");
    QTextStream out(&file);
    QString text = textEdit->toPlainText();
    out << text;
    file.close();
    }
}

void NotePad::saveAs()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(filename);
    if(!file.open(QFile::WriteOnly |QFile::Text))
    {
        QMessageBox::warning(this,"Warning","Cannot save file : "+file.errorString());
        return;
    }
    currentFile = filename;
    setWindowTitle(filename+" - NotePad");
    QTextStream out(&file);
    QString text = textEdit->toPlainText();
    out << text;
    file.close();
}

void NotePad::print()
{
    QPrinter printer;
    QPrintDialog pDialog(&printer,this);
    if(pDialog.exec() == QDialog::Rejected)
    {
        QMessageBox::warning(this,"Warning","Cannot access printer");
        return;
    }
    textEdit->print(&printer);
}

void NotePad::copy()
{
    textEdit->copy();
    pasteAct->setEnabled(true);
}

void NotePad::cut()
{
    textEdit->cut();
}

void NotePad::paste()
{
    textEdit->paste();
}

void NotePad::undo()
{
 textEdit->undo();
}

void NotePad::redo()
{
    textEdit->redo();
}

void NotePad::about()
{
    QMessageBox::about(this,"About","<p align='center'>Credits<br>https://www.youtube.com/watch?v=I96uPDifZ1w</p>");
}

void NotePad::exit()
{
    QApplication::quit();
}

void NotePad::onTextChanged()
{
    setWindowTitle("*"+(currentFile.isEmpty()? "Untitled" : currentFile) +" - NotePad");
}
