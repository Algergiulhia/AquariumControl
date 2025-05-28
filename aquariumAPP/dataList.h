#ifndef DATALIST_H
#define DATALIST_H

#include <QObject>

class dataList : public QObject {
    Q_OBJECT

public:
    dataList();
    ~dataList();
    int getId() const;

public /*slots*/:
    Q_INVOKABLE void setDataList(const QStringList&);
    Q_INVOKABLE QString getName() const;
    Q_INVOKABLE QStringList getCurrentData() const;
    Q_INVOKABLE int getCurrentIndex() const;
    Q_INVOKABLE int getChunkSize() const;
    Q_INVOKABLE void showNextData();
    Q_INVOKABLE void showPreviousData();
    Q_INVOKABLE void resetCurrentIndex();
    Q_INVOKABLE void setNameId(const QString, const int);

private:
    void updateCurrentData();

private:
    QStringList currentData;
    QList<QStringList> dataChunks;
    int currentIndex = 0;
    int chunkSize = 8;

    int id;
    QString name;
};
#endif
