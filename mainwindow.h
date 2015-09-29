#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QFileSystemModel>
#include <QProcess>
#include <QQueue>
#include <QSettings>
#include "storemodel.h"
#include "trayicon.h"
#include "process.h"
#include "config.h"
#if SINGLE_APP
#include "singleapplication.h"
#else
#define SingleApplication QApplication
#endif

namespace Ui {
class MainWindow;
}

struct execQueueItem {
    QString app;
    QString args;
    QString input;
};

struct UserInfo;

class MainWindow : public QMainWindow
{
    Q_OBJECT

enum actionType { GPG, GIT, EDIT, DELETE, GPG_INTERNAL, PWGEN };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool checkConfig();
    void setApp(SingleApplication* app);
    void setText(QString);
    QStringList getSecretKeys();
    void generateKeyPair(QString, QDialog *);
    void userDialog(QString = "");
    QString generatePassword();
    void config();
    void executePassGitInit();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_updateButton_clicked();
    void on_pushButton_clicked();
    void on_treeView_clicked(const QModelIndex &index);
    void on_configButton_clicked();
    void readyRead(bool finished);
    void processFinished(int, QProcess::ExitStatus);
    void processError(QProcess::ProcessError);
    void clearClipboard();
    void clearPanel();
    void on_lineEdit_textChanged(const QString &arg1);
    void on_lineEdit_returnPressed();
    void on_clearButton_clicked();
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_editButton_clicked();
    void on_usersButton_clicked();
    void messageAvailable(QString message);
    void on_profileBox_currentIndexChanged(QString);
    void showContextMenu(const QPoint& pos);
    void addFolder();
    void editPassword();
    void focusInput();

private:
    QApplication *QtPass;
    QScopedPointer<QSettings> settings;
    QScopedPointer<Ui::MainWindow> ui;
    QFileSystemModel model;
    StoreModel proxyModel;
    QScopedPointer<QItemSelectionModel> selectionModel;
    QProcess fusedav;
    QString clippedPass;
    actionType currentAction;
    QString lastDecrypt;
    bool wrapperRunning;
    QStringList env;
    QQueue<execQueueItem> *execQueue;
    bool freshStart;
    QDialog *keygen;
    QString currentDir;
    QHash<QString, QString> profiles;
    QString profile;
    bool startupPhase;
    trayIcon *tray;
    void updateText();
    void enableUiElements(bool);
    void selectFirstFile();
    QModelIndex firstFile(QModelIndex parentIndex);
    QString getDir(const QModelIndex &, bool);
    QString getFile(const QModelIndex &, bool);
    void setPassword(QString, bool, bool);
    QSettings &getSettings();
    QList<UserInfo> listKeys(QString keystring = "", bool secret = false);
    QStringList getRecipientList(QString for_file);
    QString getRecipientString(QString for_file, QString separator = " ", int *count = NULL);
    void mountWebDav();
    void updateEnv();
    void updateProfileBox();
    void initTrayIcon();
    void destroyTrayIcon();
    bool removeDir(const QString & dirName);
    void waitFor(int);
    Process *process;
    Config *cfg;
};

#endif // MAINWINDOW_H
