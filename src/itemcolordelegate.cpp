#include "itemcolordelegate.h"

ItemColorDelegate::ItemColorDelegate()
{

}

void ItemColorDelegate::paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const {

    QString val;
    QStyleOptionViewItem opt(option);
    if (index.data().canConvert(QMetaType::QString)) {
        val = index.data().toString();
        if (val == "5") {
            //painter->fillRect(option.rect, option.palette.highlight());
            opt.palette.setColor(QPalette::Base, QColor(Qt::GlobalColor::green));
        } else if (val == "4") {
            opt.palette.setColor(QPalette::Base, QColor(Qt::GlobalColor::yellow));
        } else if (val == "3") {
            opt.palette.setColor(QPalette::Base, QColor(Qt::GlobalColor::cyan));
        } else {
            QItemDelegate::paint(painter, option, index);
        }
    }
}




ItemColorDelegate::~ItemColorDelegate()
{

}

