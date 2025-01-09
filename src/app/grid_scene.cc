#include "grid_scene.h"

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <qtransform.h>

#include <iostream>


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

// void GridCell::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent) {
//   if (auto scene = dynamic_cast<GridScene*>(this->scene())) {
//     scene->handleMouseClick(this);
//   }
//   // QGraphicsRectItem::mousePressEvent(mouseEvent);
// }

// void GridCell::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent) {
//   if (mouseEvent->button() == Qt::MouseButton::LeftButton) {
//     if (auto scene = dynamic_cast<GridScene*>(this->scene())) {
//       scene->handleMouseClick(this);
//     }
//   }
//   // QGraphicsRectItem::mouseMoveEvent(mouseEvent);
// }

// void GridCell::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
//   if (auto scene = dynamic_cast<GridScene*>(this->scene())) {
//     scene->handleMouseClick(this);
//   }
// }

void GridCell::updateColor() {
  if (state_ == CellState::Default) {
    setBrush(Qt::lightGray);
  } else if (state_ == CellState::Gray) {
    setBrush(Qt::gray);
  } else if (state_ == CellState::DarkGray) {
    setBrush(Qt::darkGray);
  } else if (state_ == CellState::Black) {
    setBrush(Qt::black);
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

void GridScene::refreshState() {
  for (int i = 0; i != ROWS; ++i) {
    for (int j = 0; j != COLS; ++j) {
      if (m_cells_[i][j]) {
        m_cells_[i][j]->setState(CellState::Default);
      }
    }
  }
}

// void GridScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
// }

void GridScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  last_processed_cell_ = nullptr;
}

void GridScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  if (mouseEvent->buttons() & Qt::LeftButton) {
    auto item = itemAt(mouseEvent->scenePos(), QTransform());
    if (GridCell* cell = dynamic_cast<GridCell*>(item)) {
      if (last_processed_cell_ != cell) {
        handleMouseClick(cell);
        last_processed_cell_ = cell;
      }
    }
  }
}

void GridScene::keyPressEvent(QKeyEvent *keyEvent) {
  if (keyEvent->key() == Qt::Key_D) {
    refreshState();
  }
}

GridScene::~GridScene() {
  for (int i = 0; i != ROWS; ++i) {
    for (int j = 0; j != COLS; ++j) {
      delete(m_cells_[i][j]);
    }
  }
}

static inline void updateCellState(GridCell* cell) {
  if (!cell) return;
    CellState current_state = cell->getState();
    if (current_state == CellState::Default) {
      cell->setState(CellState::Gray);
    } else if (current_state == CellState::Gray) {
      cell->setState(CellState::DarkGray);
    } else if (current_state == CellState::DarkGray) {
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
