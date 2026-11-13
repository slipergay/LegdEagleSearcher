#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QStringListModel>
#include <QDirIterator>
#include <QMessageBox>
#include <QInputDialog>
#include <QAbstractItemView>
#include <QDesktopServices>
#include <QUrl>
#include <QMenu>

// JSON 分类 key -> 界面显示中文名
static QString categoryDisplayName(const QString &key)
{
    static const QMap<QString, QString> zhNames = {
        {"dirs",        QStringLiteral("文件夹")},
        {"documents",   QStringLiteral("文档")},
        {"pictures",    QStringLiteral("图片")},
        {"audios",      QStringLiteral("音频")},
        {"videos",      QStringLiteral("视频")},
        {"programs",    QStringLiteral("程序")},
        {"cof",         QStringLiteral("压缩包")},
        {"others",      QStringLiteral("其他")}
    };
    return zhNames.value(key, key);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->stackedWidget->setCurrentIndex(0);
    m_listModel = new QStringListModel(this);
    ui->listView->setModel(m_listModel);
    m_fglistModel = new QStringListModel(this);
    ui->listView_2->setModel(m_fglistModel);

    //只初始化一次选择模式
    ui->listView_2->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void saveJsonToFile(const QString &filePath, const QJsonDocument &doc)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "保存JSON失败，无法打开文件：" << filePath << file.errorString();
        return;
    }
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}

void MainWindow::on_pushButton_clicked()
{
    dir = QFileDialog::getExistingDirectory(this, tr("选择目录"), "./");
    if(dir.isEmpty())
    {
        qDebug()<<"用户取消选择文件夹";
        return;
    }
    QJsonObject obj;
    QDir baseDir(dir);
    QDirIterator it2(dir,
                     QDir::AllEntries | QDir::NoDotAndDotDot,
                     QDirIterator::Subdirectories
                     );
    QJsonArray dirs,docu,pic,aud,vid,other,prog,cof;
    int scanCount = 0;
    m_allRelPaths.clear();
    while (it2.hasNext())
    {
        QString filePath = it2.next();
        QFileInfo info = it2.fileInfo();
        scanCount++;
        qDebug()<<"扫描到条目："<<filePath << " isDir="<<info.isDir();
        // 存相对路径：不同子目录下的同名文件可以区分，双击时也能定位到真实文件
        QString rel = baseDir.relativeFilePath(filePath);
        m_allRelPaths.append(rel);
        QString sf=info.suffix();
        sf=sf.toLower();
        bool isfige=false;
        if(info.isDir()){
            dirs.append(rel);
            isfige=true;
        }
        if(sf=="txt"||sf=="doc"||sf=="docx"||sf=="pdf"||sf=="xls"||sf=="xlsx"||sf=="ppt"||sf=="pptx"||sf=="odt"||sf=="rtf"||sf=="md"||sf=="html"||sf=="htm"){
            docu.append(rel);
            isfige=true;
        }
        if(sf=="jpg"||sf=="jpeg"||sf=="bmp"||sf=="png"||sf=="webp"||sf=="svg"||sf=="ico"||sf=="tif"||sf=="tiff"||sf=="psd"){
            pic.append(rel);
            isfige=true;
        }
        if(sf=="mp3"||sf=="m4a"||sf=="wav"||sf=="flac"||sf=="aac"||sf=="wma"||sf=="ogg"||sf=="amr"||sf=="ape"||sf=="aiff"){
            aud.append(rel);
            isfige=true;
        }
        if(sf=="mp4"||sf=="avi"||sf=="mov"||sf=="mkv"||sf=="flv"||sf=="wmv"||sf=="vob"){
            vid.append(rel);
            isfige=true;
        }
        if(sf=="zip"||sf=="7z"||sf=="rar"||sf=="tar"||sf=="gz"){
            cof.append(rel);
            isfige=true;
        }
        if(sf=="exe"||sf=="apk"||sf=="py"||sf=="js"||sf=="vbs"||sf=="bat"||sf=="sh"){
            prog.append(rel);
            isfige=true;
        }
        if(!isfige){
            other.append(rel);
        }
    }
    qDebug()<<"====扫描结束，一共扫描条目数量："<< scanCount;
    qDebug()<<"docu:"<<docu.size()<<" pic:"<<pic.size()<<" dirs:"<<dirs.size();
    obj["dirs"] = dirs;
    obj["documents"] = docu;
    obj["pictures"] = pic;
    obj["audios"] = aud;
    obj["videos"] = vid;
    obj["programs"] = prog;
    obj["cof"] = cof;
    obj["others"] = other;
    cdoc.setObject(obj);
    QString tmp=dir+"/nion.json";
    saveJsonToFile(tmp,cdoc);
    qDebug()<<"json写入路径："<< tmp;
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
    if(arg1==1){
        soy.clear();
        m_catKeys.clear();
        QString mul=dir+"/nion.json";
        QFile file(mul);
        if(file.open(QFile::ReadOnly)){
            QByteArray rawJson = file.readAll();
            QJsonParseError err;
            QJsonDocument doc = QJsonDocument::fromJson(rawJson, &err);
            file.close();
            if (err.error != QJsonParseError::NoError)
            {
                qDebug() << "解析失败：" << err.errorString();
                return;
            }
            if (!doc.isObject())
            {
                qDebug() << "根不是JSON对象";
                return;
            }
            QJsonObject root = doc.object();
            //同步更新内存cdoc，保证内存和磁盘一致
            cdoc = doc;

            QStringList allKeys = root.keys();
            ui->comboBox->clear();
            soy.clear();
            for(const QString &keyName : allKeys)
            {
                QJsonValue val = root[keyName];
                if(val.isArray())
                {
                    qDebug() << "===== 找到数组，key：" << keyName;
                    QJsonArray arr = val.toArray();
                    qDebug() << "数组元素数量：" << arr.size();
                    QStringList tmpst;
                    for(const QJsonValue &item : arr)
                    {
                        tmpst.append(item.toString());
                    }
                    // 显示中文名，原始key存进itemData，保证顺序与soy一致
                    ui->comboBox->addItem(categoryDisplayName(keyName), keyName);
                    m_catKeys.append(keyName);
                    soy.append(tmpst);
                }
            }
            if(!soy.isEmpty())
            {
                ui->comboBox->setCurrentIndex(0);
                m_listModel->setStringList(soy[0]);
            }
            else
            {
                m_listModel->setStringList({});
            }
        }else{
            qDebug() << "搜索页面加载失败:" << file.errorString() << mul;
        }
    }else if(arg1==2){
        if(dir.isEmpty())
        {
            qDebug()<<"尚未选择目录，跳过页面2扫描";
            return;
        }
        if(m_allRelPaths.isEmpty())
        {
            // 兜底：缓存为空时才重新扫描（正常流程在首次扫描时已缓存）
            QDirIterator its(dir,
                             QDir::AllEntries | QDir::NoDotAndDotDot,
                             QDirIterator::Subdirectories
                             );
            QStringList lst;
            QDir baseDir(dir);
            while (its.hasNext())
            {
                QString filePath = its.next();
                lst.append(baseDir.relativeFilePath(filePath));
            }
            m_allRelPaths = lst;
        }
        // 复用缓存，不再每次切页都全量递归扫描
        m_fglistModel->setStringList(m_allRelPaths);
    }
}

void MainWindow::on_comboBox_activated(int index)
{
    // 通过 itemData 找回原始key，再定位到soy中的下标，避免依赖显示顺序
    QString key = ui->comboBox->itemData(index).toString();
    int i = m_catKeys.indexOf(key);
    if(i < 0 || i >= soy.size())
        return;
    m_listModel->setStringList(soy[i]);
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    if(dir.isEmpty())
        return;
    QString rel = index.data(Qt::DisplayRole).toString();
    if(rel.isEmpty())
        return;
    QString fullPath = QDir(dir).filePath(rel);
    if(!QFileInfo::exists(fullPath))
    {
        QMessageBox::warning(this, tr("找不到条目"),
                             tr("路径不存在（可能是旧版nion.json只保存了文件名）：\n%1").arg(fullPath));
        return;
    }
    // 用系统默认程序打开；文件夹则打开资源管理器
    QDesktopServices::openUrl(QUrl::fromLocalFile(fullPath));
}

void MainWindow::on_pushButton_3_clicked()
{
    extx=QInputDialog::getText(this,"新分类名称","请输入新分类名称",QLineEdit::Normal);
    extx = extx.trimmed(); //去除首尾空格
    if(extx.isEmpty()){
        QMessageBox::information(this,"取消","你取消了对话框，操作不进行");
        return;
    }
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pushButton_4_clicked()
{
    if(dir.isEmpty() || extx.trimmed().isEmpty())
    {
        QMessageBox::warning(this,"错误","尚未设置分类名称或者目录为空");
        return;
    }

    QItemSelectionModel *selModel = ui->listView_2->selectionModel();
    QModelIndexList list = selModel->selectedIndexes();
    qDebug() << "选中条目数量：" << list.size();

    if(list.isEmpty())
    {
        QMessageBox::warning(this,"提示","页面2没有选中任何条目，无法创建分类");
        return;
    }

    QString filePath = dir+"/nion.json";
    QFile file(filePath);
    if(!file.open(QFile::ReadOnly))
    {
        QMessageBox::warning(this,"错误","读取nion.json失败");
        return;
    }
    QByteArray raw = file.readAll();
    file.close();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(raw,&err);
    if(err.error != QJsonParseError::NoError || !doc.isObject())
    {
        QMessageBox::warning(this,"错误","JSON解析失败");
        return;
    }

    QJsonObject obj = doc.object();
    QJsonArray jsonArr;
    for (const QModelIndex& idx : list)
    {
        QVariant val = idx.data(Qt::DisplayRole);
        jsonArr.append(val.toString());
    }
    obj[extx] = jsonArr;
    QJsonDocument newDoc(obj);
    saveJsonToFile(filePath, newDoc);

    cdoc = newDoc;
    ui->stackedWidget->setCurrentIndex(1);
}
void openContainingFolder(const QString &filePath)
{
    QFileInfo fi(filePath);
    if (!fi.exists())
        return;

    // 传文件 → 打开父目录；传目录 → 打开该目录
    QString dirPath = fi.isDir() ? fi.absoluteFilePath()
                                 : fi.absoluteDir().absolutePath();

    QDesktopServices::openUrl(QUrl::fromLocalFile(dirPath));
}
void MainWindow::on_listView_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);

    QAction *actOpen = menu.addAction("打开");
    QAction *actRename = menu.addAction("重命名");
    QAction *actDelete = menu.addAction("删掉");
    QAction *actPath = menu.addAction("文件路径");

    QAction *ret = menu.exec(mapToGlobal(pos));
    if (!ret)
        return;

    if (ret == actOpen) {
        QString fullPath;
        QModelIndex cur = ui->listView->currentIndex();
        if (cur.isValid()) {
            fullPath = QDir(dir).filePath(cur.data(Qt::DisplayRole).toString());
        }
        if(!QFileInfo::exists(fullPath))
        {
            QMessageBox::warning(this, tr("找不到条目"),
                                 tr("路径不存在（可能是demo版只保存了文件名）：\n%1").arg(fullPath));
            return;
        }
        QDesktopServices::openUrl(QUrl::fromLocalFile(fullPath));
    } else if (ret == actRename) {
        QString fullPath;
        QModelIndex cur = ui->listView->currentIndex();
        if (cur.isValid()) {
            fullPath = QDir(dir).filePath(cur.data(Qt::DisplayRole).toString());
        }
        QFile file(fullPath);
        QFileInfo fo(fullPath);
        if(file.exists()){
            QString ren=QInputDialog::getText(this,"输入","输入新名字");
            if (ren.isEmpty()) return;
            file.rename(fo.absolutePath() + "/" +ren+'.'+fo.suffix());
        }

    } else if (ret == actDelete) {
        QString fullPath;
        QModelIndex cur = ui->listView->currentIndex();
        if (cur.isValid()) {
            fullPath = QDir(dir).filePath(cur.data(Qt::DisplayRole).toString());
        }
        QFile file(fullPath);
        QFileInfo fo(fullPath);
        if(file.exists()){
            if (QMessageBox::question(this, "确认", "删吗？") == QMessageBox::Yes) {
                if (!file.remove()) {
                    QMessageBox::warning(this, "删除失败", file.errorString());
                    return;
                }
                int row = cur.row();
                m_listModel->removeRow(row);
                int i = m_catKeys.indexOf(ui->comboBox->currentData().toString());
                if (i >= 0 && i < soy.size() && row < soy[i].size())
                    soy[i].removeAt(row);
            }
        }
    } else if (ret == actPath) {
        QString fullPath;
        QModelIndex cur = ui->listView->currentIndex();
        if (cur.isValid()) {
            fullPath = QDir(dir).filePath(cur.data(Qt::DisplayRole).toString());
        }
        openContainingFolder(fullPath);
    }
}

