#pragma once

#include <QGraphicsScene>
#include <QGraphicsItem>

#define ROWS 20
#define COLS 20
#define CELL_SIZE 20
#define GAP 3

enum class CellState {
  Default,
  Gray,
  Black
};

class GridCell final : public QGraphicsRectItem {
  public:
    GridCell(qreal x, qreal y, qreal size, QGraphicsItem* parent = nullptr);

    CellState getState() const noexcept;

    void setState(CellState state) noexcept;

  protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent) override;

  private:
    CellState state_;

    void updateColor();
};

class GridScene final : public QGraphicsScene {
  Q_OBJECT

public:
    GridScene(QObject* parent = nullptr);
    ~GridScene();

    void extracted();
    void handleMouseClick(GridCell *cell);

    GridCell* cellAt(int row, int col);

/* protected: */

private:
    QVector<QVector<GridCell*>> m_cells_;
};
