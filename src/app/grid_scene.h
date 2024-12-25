#pragma once

#include <QGraphicsScene>
#include <QGraphicsItem>

#define ROWS 30
#define COLS 30
#define CELL_SIZE 15
#define GAP 3

enum class CellState {
  Default,
  Gray,
  DarkGray,
  Black
};

class GridCell final : public QGraphicsRectItem {
  public:
    GridCell(qreal x, qreal y, qreal size, QGraphicsItem* parent = nullptr);

    CellState getState() const noexcept;

    void setState(CellState state) noexcept;

  // protected:
    // void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
    // void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
    // void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

  private:
    CellState state_;

    void updateColor();
};

class GridScene final : public QGraphicsScene {
  Q_OBJECT

public:
    GridScene(QObject* parent = nullptr);
    ~GridScene();

    void handleMouseClick(GridCell *cell);

    GridCell* cellAt(int row, int col);

    void refreshState();

protected:
void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
void keyPressEvent(QKeyEvent *keyEvent) override;

private:
    QVector<QVector<GridCell*>> m_cells_;

    GridCell* last_processed_cell_ = nullptr;
};
