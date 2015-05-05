#ifndef ITEMCOLORDELEGATE_H
#define ITEMCOLORDELEGATE_H

#include <QItemDelegate>


class ItemColorDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ItemColorDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    ~ItemColorDelegate();
};

#endif // ITEMCOLORDELEGATE_H

