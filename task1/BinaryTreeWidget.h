#ifndef BINARYTREEWIDGET_H
#define BINARYTREEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include "bst.h"

constexpr int kVerticalStep = 70;
constexpr int kPenWidth = 2;
constexpr int kCircleDiameter = 40;
constexpr int kMargin = 20;

class BinaryTreeWidget : public QWidget {
    Q_OBJECT

public:
    explicit BinaryTreeWidget(QWidget* parent = nullptr)
        : QWidget(parent), tree(nullptr) {}

    void setTree(GoodRBTree<int, std::string>* newTree) {
        tree = newTree;
        update();
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        if (tree && tree->getRoot()) {
            drawTree(&painter, tree->getRoot(), width() / 2, kMargin + kCircleDiameter / 2, width() / 5.0);
        }
    }

private:
    GoodRBTree<int, std::string>* tree;

    void drawTree(QPainter* painter, TreeNode<int, std::string>* node, int x, int y, double dx) {
        if (!node) return;

        int childY = y + kVerticalStep;

        // Draw left edge (blue, thicker)
        if (node->left) {
            painter->setPen(QPen(Qt::blue, kPenWidth + 1));
            painter->drawLine(x, y, x - dx, childY);
            drawTree(painter, node->left, x - dx, childY, dx / 1.5);
        }

        // Draw right edge (red, thicker)
        if (node->right) {
            painter->setPen(QPen(Qt::red, kPenWidth + 1));
            painter->drawLine(x, y, x + dx, childY);
            drawTree(painter, node->right, x + dx, childY, dx / 1.5);
        }

        // Draw node circle
        QColor fillColor = (node->color == RED) ? Qt::red : Qt::black;
        painter->setBrush(QBrush(fillColor));
        painter->setPen(QPen(Qt::white, 2));
        painter->drawEllipse(x - kCircleDiameter / 2, y - kCircleDiameter / 2, kCircleDiameter, kCircleDiameter);

        // Draw key text
        painter->setPen(Qt::white);
        QFont font = painter->font();
        font.setBold(true);
        painter->setFont(font);
        QRect textRect(x - kCircleDiameter / 2, y - kCircleDiameter / 2, kCircleDiameter, kCircleDiameter);
        painter->drawText(textRect, Qt::AlignCenter, QString::number(node->key));
    }
};

#endif // BINARYTREEWIDGET_H
