#pragma once

#include <QGraphicsScene>
#include <QGraphicsItem>

#define ROWS 28
#define COLS 28
#define CELL_SIZE 14
#define GAP 2

enum class CellState {
  Default,
  LightGray,
  Gray,
  DarkGray,
  Black
};

class GridCell final : public QGraphicsRectItem {
  public:
    GridCell(qreal x, qreal y, qreal size, QGraphicsItem* parent = nullptr);

    CellState getState() const noexcept;

    void setState(CellState state) noexcept;

    double NormalizePixel() const noexcept;

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

    std::vector<double> GetSnapshot() const;

protected:
// void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
void keyPressEvent(QKeyEvent *keyEvent) override;

private:
    QVector<QVector<GridCell*>> m_cells_;

    GridCell* last_processed_cell_ = nullptr;
};
