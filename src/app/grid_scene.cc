#include "grid_scene.h"

#include <QGraphicsSceneMouseEvent>
#include <qgraphicsitem.h>

#include <iostream>
#include <qnamespace.h>

GridCell::GridCell(qreal x, qreal y, qreal size, QGraphicsItem* parent)
  : QGraphicsRectItem(x, y, size, size, parent),
    state_{CellState::Default} {
    updateColor();
  }

CellState GridCell::getState() const noexcept { return state_; }

void GridCell::setState(CellState state) noexcept {
  state_ = state;
  updateColor();
}

void GridCell::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent) {
  if (auto scene = dynamic_cast<GridScene*>(this->scene())) {
    scene->handleMouseClick(this);
  }
  QGraphicsRectItem::mousePressEvent(mouseEvent);
}

void GridCell::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent) {
  if (mouseEvent->button() == Qt::MouseButton::LeftButton) {
    if (auto scene = dynamic_cast<GridScene*>(this->scene())) {
      scene->handleMouseClick(this);
    }
  }
  QGraphicsRectItem::mouseMoveEvent(mouseEvent);
}

void GridCell::updateColor() {
  if (state_ == CellState::Default) {
    setBrush(QColor(211, 211, 211));
  } else if (state_ == CellState::Gray) {
    setBrush(QColor(69, 69, 69));
  } else if (state_ == CellState::Black) {
    setBrush(Qt::black);
  } else {

  }
}


GridScene::GridScene(QObject* parent) : QGraphicsScene(parent) {
  m_cells_.resize(ROWS);
  for (int i = 0; i != ROWS; ++i) {
    m_cells_[i].resize(COLS);
    for (int j = 0; j != COLS; ++j) {
      qreal x = j * (CELL_SIZE + GAP);
      qreal y = i * (CELL_SIZE + GAP);
      auto cell = new GridCell(x, y, CELL_SIZE);
      addItem(cell);
      m_cells_[i][j] = cell;
    }
  }
}

GridCell* GridScene::cellAt(int row, int col) {
  if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
    return m_cells_[row][col];
  }
  return nullptr;
}

GridScene::~GridScene() {
  for (int i = 0; i != ROWS; ++i) {
    for (int j = 0; j != COLS; ++j) {
      delete(m_cells_[i][j]);
    }
  }
}

static void updateCellState(GridCell* cell) {
  if (!cell) return;
    CellState current_state = cell->getState();
    if (current_state == CellState::Default) {
      cell->setState(CellState::Gray);
    } else if (current_state == CellState::Gray) {
      cell->setState(CellState::Black);
    }
}

void GridScene::handleMouseClick(GridCell *cell) {
  int clickedRow = -1, clickedCol = -1;
  GridCell* curr = nullptr;
  for (int i = 0; i != ROWS; ++i) {
    for (int j = 0; j != COLS; ++j) {
      if (m_cells_[i][j] == cell) {
        clickedRow = i;
        clickedCol = j;
        break;
      }
    }
    if (clickedRow != -1) break;
  }

  if (clickedRow == -1 || clickedCol == -1) {
    return;
  }

  GridCell* left  = clickedCol > 0        ? m_cells_[clickedRow][clickedCol - 1] : nullptr;
  GridCell* right = clickedCol < COLS - 1 ? m_cells_[clickedRow][clickedCol + 1] : nullptr;
  GridCell* top   = clickedRow > 0        ? m_cells_[clickedRow - 1][clickedCol] : nullptr;
  GridCell* bot   = clickedRow < ROWS - 1 ? m_cells_[clickedRow + 1][clickedCol] : nullptr;

  updateCellState(left);
  updateCellState(right);
  updateCellState(top);
  updateCellState(bot);

  cell->setState(CellState::Black);
}
