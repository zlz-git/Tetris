#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

const int BLOCK_SIZE=25;
const int MARGIN=5;
const int AREA_ROW=20;
const int AREA_COL=12;

enum Direction{
    Up,
    Down,
    Left,
    Right,
    Space
};

struct Boarder{
    int upboard;
    int downboard;
    int leftboard;
    int rightboard;
};

struct block_point{
    int pos_x;
    int pos_y;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void InitGame();
    void StartGame();
    void GameOver();
     void paintEvent(QPaintEvent*event);
     void timerEvent(QTimerEvent*event);
    void CreateBlock(int block[4][4],int blockid);
    void ResetBlock();
    void GetBoarder(int block[4][4],Boarder &blockboarder);//

    void BlockMove(Direction dir);
    void ConvertStable(int x,int y);
    bool IsCollide(int x,int y, Direction dir);
    void BlockRotate(int block[4][4]);
    void keyPressEvent(QKeyEvent*event);



private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    int game_area[AREA_ROW][AREA_COL];
    block_point block_pos;

    int cur_block[4][4];
    Boarder cur_boarder;

    int next_block[4][4];
    Boarder next_boarder;

    int score;
    int game_timer;
    int paint_timer;
    int speed_ms;
    int refresh_ms;

    int falg;


};

#endif // MAINWINDOW_H
