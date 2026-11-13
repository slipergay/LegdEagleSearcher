#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringListModel>
#include <QJsonDocument>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_comboBox_activated(int index);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    // 双击搜索结果，用系统默认程序打开
    void on_listView_doubleClicked(const QModelIndex &index);

    void on_listView_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    QString dir;
    QStringListModel* m_listModel;
    QStringListModel* m_fglistModel;
    QList<QStringList> soy;      // 与 m_catKeys 一一对应
    QStringList m_catKeys;       // JSON 中的原始 key
    QJsonDocument cdoc;
    QString extx;
    QStringList m_allRelPaths;   // 初次扫描缓存（相对路径），供“新增分类”页复用，避免重复递归扫描
};

#endif // MAINWINDOW_H