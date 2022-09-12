#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <memory>
#include <list>
#include <vector>
#include <set>

struct Cell;
struct Block;
struct LineColumnBase;
struct Line;
struct Column;
struct Palace;

struct Cell { //一个小格子
    Cell(uint8_t val) : value(val), possibleValues({1, 2, 3, 4, 5, 6, 7, 8, 9}) { 
        if (value == 0) {
            possibleValues.clear();
        }
    }

    uint8_t value = 0; //当前格子的值，0表示值还没确定，1~9表示已经确定
    std::set<uint8_t> possibleValues; //可能的取值
    std::weak_ptr<Block> lineBlock; //当前格子所在九宫格中的行
    std::weak_ptr<Block> columnBlock; //当前格子所在九宫格中的列
};

struct Block { //九宫格中的一行或一列，含3个格子
    Block(bool horizontal) : isHorizontal(horizontal) { }

    void addCell(const std::shared_ptr<Cell>& cell) {
        assert(cell);
        assert(cells.size() < 3);
        cells.push_back(cell);
    }

    bool isHorizontal;
    std::vector<std::shared_ptr<Cell> > cells; //3个格子
    std::weak_ptr<LineColumnBase> lineOrColumn; //当前Block所在Line或Column
    std::weak_ptr<Palace> palace; //当前Block所在Palace
};

struct LineColumnBase { //数独的一整行
    void addBlock(const std::shared_ptr<Block>& block) {
        assert(block);
        assert(blocks.size() < 3);
        blocks.push_back(block);
    }

    virtual void print() = 0;

    std::vector<std::shared_ptr<Block> > blocks; //一整行、列中的3个block
};

struct Line : public LineColumnBase { //数独的一整行
    Line(int index) : lineIndex(index) { }

    void print() override {
        bool start = true;
        for (const auto& block : blocks) {
            for (const auto &cell : block->cells) {
                printf ("%s%d", start ? "" : ", ", cell->value);
                start = false;
            }
        }
        printf ("\n");
    }

    int lineIndex = -1;
};

struct Column : public LineColumnBase { //数独的一整列
    Column(int index) : columnIndex(index) { }

    void print() override {
        for (const auto& block : blocks) {
            for (const auto &cell : block->cells) {
                printf ("%d\n", cell->value);
            }
        }
    }

    int columnIndex = -1;
};

struct Palace { //数独的一个九宫格
    Palace(int horIdx, int verIdx) : horIndex(horIdx), verIndex(verIdx) { }

    void addBlock(const std::shared_ptr<Block>& block, bool horizontal) {
        assert(block);
        std::vector<std::shared_ptr<Block> > &blocks = horizontal ? horBlocks : verBlocks;
        assert(blocks.size() < 3);
        blocks.push_back(block);
    }

    void print() {
        for (const auto& block : horBlocks) {
            bool start = true;
            for (const auto &cell : block->cells) {
                printf ("%s%d", start ? "" : ", ", cell->value);
                start = false;
            }
            printf ("\n");
        }
    }

    int horIndex = -1;
    int verIndex = -1;
    std::vector<std::shared_ptr<Block> > horBlocks; //九宫格中的3个横向block
    std::vector<std::shared_ptr<Block> > verBlocks; //九宫格中的3个纵向block
};

static void setBlocks(std::shared_ptr<Cell>& cell, std::shared_ptr<Block>& lnBlk, std::shared_ptr<Block>& colBlk) {
    assert(cell);
    assert(lnBlk);
    assert(colBlk);
    cell->lineBlock = lnBlk;
    lnBlk->addCell(cell);
    cell->columnBlock = colBlk;
    colBlk->addCell(cell);
}

static void setLineColumnPalace(std::shared_ptr<Block>& block, std::shared_ptr<LineColumnBase>& lnOrCol, std::shared_ptr<Palace>& plc) {
    assert(block);
    assert(lnOrCol);
    assert(plc);
    block->lineOrColumn = lnOrCol;
    lnOrCol->addBlock(block);
    block->palace = plc;
    plc->addBlock(block, block->isHorizontal);
}

struct Sudoku {
    Sudoku() {
        for (int i = 0; i < 9; i++) {
            lines[i] = std::make_shared<Line>(i); //9行
            columns[i] = std::make_shared<Column>(i); //9列
            palaces[i / 3][i % 3] = std::make_shared<Palace>(i / 3, i % 3); //9个9宫格
        }
    }

    Sudoku(uint8_t matrix[81]) : Sudoku() {
        std::shared_ptr<Block> lineInBlocks[27];
        std::shared_ptr<Block> columnInBlocks[27];
        for (int i = 0; i < 27; i++) {
            lineInBlocks[i] = std::make_shared<Block>(true);
            //0, 1, 2号block属于0号line，3, 4, 5号block属于1号line
            //0, 3, 6号block属于0,0号palace，1, 4, 7号block属于0,1号palace，2, 5, 8号block属于0,2号palace
            //9, 12, 15号block属于1,0号palace，10, 13, 16号block属于1,1号palace，20, 23, 26号block属于2,2号palace
            setLineColumnPalace(lineInBlocks[i], lines[i / 3], palaces[i / 9][i % 3]);

            columnInBlocks[i] = std::make_shared<Block>(false);
            //0, 9, 18号block属于0号column，1, 10, 19号block属于1号column
            //0, 1, 2号block属于0,0号palace，3, 4, 5号block属于0,1号palace，6, 7, 8号block属于0,2号palace
            //9, 10, 11号block属于1,0号palace，12, 13, 14号block属于1,1号palace，24, 25, 26号block属于2,2号palace
            setLineColumnPalace(columnInBlocks[i], columns[i % 9], palaces[i / 9][(i % 9) / 3]);
        }
        std::shared_ptr<Cell> cells[81];
        for (int i = 0; i < 81; i++) {
            cells[i] = std::make_shared<Cell>(matrix[i]);
            //0, 1, 2号cell属于第0行第0个block，3, 4, 5号cell属于第0行第1个block
            //0, 9, 18号cell属于第0列第0个block，27, 36, 45号cell属于第0列第1个block
            //1, 10, 19号cell属于第1列第0个block，28, 37, 46号cell属于第1列第1个block
            setBlocks(cells[i], lines[i / 9]->blocks[(i % 9) / 3], columns[i % 9]->blocks[i / 27]);
        }
    }

    void print(int type = (1 | 2 | 4)) {
        if (type & 1) { //打印lines
            printf("按行打印：\n");
            for (int i = 0; i < 9; i++) {
                lines[i]->print();
            }
        }
        if (type & 2) { //打印columns
            printf("按列打印：\n");
            for (int i = 0; i < 9; i++) {
                columns[i]->print();
            }
        }
        if (type & 4) { //打印palaces
            printf("按九宫格打印：\n");
            for (int i = 0; i < 9; i++) {
                palaces[i / 3][i % 3]->print();
            }
        }
    }

    std::shared_ptr<LineColumnBase> lines[9]; //9行
    std::shared_ptr<LineColumnBase> columns[9]; //9列
    std::shared_ptr<Palace> palaces[3][3]; //9个九宫格
};

//先将每个cell的可能取值算出来（行、列、九宫格中已确定的数字都排除）
//

int main() {
    uint8_t matrix[] = {
        9, 8, 5,  1, 0, 0,  2, 0, 0,
        0, 2, 0,  5, 0, 6,  3, 8, 4,
        4, 3, 0,  2, 0, 7,  0, 0, 9,

        8, 0, 2,  0, 0, 0,  0, 4, 0,
        6, 9, 4,  3, 1, 0,  0, 0, 7,
        0, 7, 0,  0, 0, 4,  9, 0, 0,

        0, 0, 8,  4, 0, 0,  7, 9, 0,
        3, 0, 0,  0, 0, 0,  8, 0, 5,
        0, 0, 0,  8, 0, 0,  4, 3, 0
    };

    std::shared_ptr<Sudoku> sudoku = std::make_shared<Sudoku>(matrix);
    sudoku->print();

    return 0;
}
