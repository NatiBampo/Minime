#include "startButtonDelegate.h"


StartButtonDelegate::StartButtonDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    _state =  QStyle::State_Enabled;
}


void StartButtonDelegate::paint(QPainter *painter,
                   const QStyleOptionViewItem &option,
                   const QModelIndex &index) const
{

    QString text = index.data(Qt::DisplayRole).toString();
    QRect rect = option.rect;

     //QRect textRect( rect);
     //textRect.setHeight( 30);
     //painter->drawText(textRect,"text");

     QRect buttonRect( rect);
     //buttonRect.setY(textRect.y()+ 35);
     //buttonRect.setHeight( 60);
     QStyleOptionButton button;
     button.rect = buttonRect;
     button.text = text;//QString("text %1 %2").arg(index.row()).arg(text);
     button.state = _state | QStyle::State_Enabled;

     QApplication::style()->drawControl
         (QStyle::CE_PushButton, &button, painter);
}

QSize StartButtonDelegate::sizeHint(const QStyleOptionViewItem &option,
                       const QModelIndex &index) const
{
    return QSize(800, 170);
}

bool StartButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index)
{

    QRect buttonRect( option.rect);
    buttonRect.setY(option.rect.y()+ 35);
    buttonRect.setHeight( 30);

    if( event->type() == QEvent::MouseButtonPress) {
        _state = QStyle::State_Sunken;
    } else if( event->type() == QEvent::MouseButtonRelease) {
        _state = QStyle::State_Raised;
        emit buttonClicked( index);
    }
    return true;
}
