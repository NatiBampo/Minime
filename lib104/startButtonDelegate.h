#ifndef STARTBUTTONDELEGATE_H
#define STARTBUTTONDELEGATE_H

#include <QItemDelegate>
#include <QStandardItemModel>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>


class StartButtonDelegate:  public QItemDelegate
{
    Q_OBJECT
public:
    StartButtonDelegate(QObject *parent = 0);
    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const ;

    virtual QSize sizeHint(const QStyleOptionViewItem &option,
                           const QModelIndex &index) const ;

    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index);

signals:
    void buttonClicked(const QModelIndex &index);

private:
    QStyle::State  _state;
};

#endif // STARTBUTTONDELEGATE_H
