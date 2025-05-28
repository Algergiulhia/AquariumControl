#include "dataList.h"

dataList::dataList()
{
}

dataList::~dataList()
{
}

QStringList dataList::getCurrentData() const {
    return currentData;
}

int dataList::getCurrentIndex() const {
    return currentIndex;
}

int dataList::getChunkSize() const {
    return chunkSize;
}

int dataList::getId() const {
    return id;
}

QString dataList::getName() const {
    return name;
}

void dataList::setDataList(const QStringList& allDataList) {
    QList<QStringList> dataChunks2;
    for (int i = 0; i < allDataList.size(); i += chunkSize) {
        QStringList chunk = allDataList.mid(i, chunkSize);
        dataChunks2.append(chunk);
    }
    dataChunks = dataChunks2;
    updateCurrentData();
}

void dataList::showNextData() {
    ++currentIndex;
    updateCurrentData();
}

void dataList::showPreviousData() {
    --currentIndex;
    updateCurrentData();
}

void dataList::resetCurrentIndex() {
    currentIndex = 0;
}

void dataList::setNameId(const QString name, const int id) {
    this->name = name;
    this->id = id;
}

void dataList::updateCurrentData() {
    if (currentIndex >= 0 && currentIndex < dataChunks.size()) {
        currentData = dataChunks[currentIndex];
    } else {
        currentIndex = qBound(0, currentIndex, dataChunks.size() - 1);
    }
}
