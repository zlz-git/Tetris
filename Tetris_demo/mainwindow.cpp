#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"time.h"
#include<QDebug>
#include<QPainter>
#include<QMessageBox>
#include<QKeyEvent>
int item1[4][4]=
{
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
};
//右L
int item2[4][4]=
{
    {0,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
};
//左L
int item3[4][4]=
{
    {0,0,1,0},
    {0,0,1,0},
    {0,1,1,0},
    {0,0,0,0}
};
//右S
int item4[4][4]=
{
    {0,1,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,0,0}
};
//左S
int item5[4][4]=
{
    {0,0,1,0},
    {0,1,1,0},
    {0,1,0,0},
    {0,0,0,0}
};
//山形
int item6[4][4]=
{
    {0,0,0,0},
    {0,0,1,0},
    {0,1,1,1},
    {0,0,0,0}
};
int item7[4][4]=
{
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0}
};

inline void block_copy(int block1[4][4],int block2[4][4]){
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            block1[i][j]=block2[i][j];
        }
    }

}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent,Qt::WindowTitleHint|Qt::CustomizeWindowHint),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   this->setFocusPolicy(Qt::StrongFocus);
    falg=0;
    QImage bg=QImage(":/image/timg.jpg").scaled(this->size());
    QPalette palette=this->palette();
    palette.setBrush(QPalette::Background,QBrush(bg));
    this->setPalette(palette);
    InitGame();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::InitGame(){
    for(int i=0;i<AREA_ROW;++i){
        for(int j=0;j<AREA_COL;++j){
            game_area[i][j]=0;
        }
    }
    srand(time(0));
    speed_ms=800;
    refresh_ms=30;
    score=0;

}

void MainWindow::on_pushButton_clicked()
{
    if(falg==0){
        StartGame();
        falg=1;
        ui->pushButton->setText("暂停游戏");
       ui->pushButton->clearFocus();
    }
    else if(falg==1){
        killTimer(game_timer);
        QMessageBox::information(this,"提示","暂停中");
        ui->pushButton->setText("继续游戏");
        falg=2;
        ui->pushButton->clearFocus();
    }
    else if(falg==2){
        game_timer=startTimer(speed_ms);
        falg=1;
        ui->pushButton->setText("暂停游戏");
         ui->pushButton->clearFocus();
    }

}
void MainWindow::StartGame(){
  game_timer=startTimer(speed_ms);
  paint_timer=startTimer(refresh_ms);

  int blockid=rand()%7;
  CreateBlock(next_block,blockid);
  ResetBlock();
}
void MainWindow::GameOver(){
    killTimer(game_timer);
    //killTimer(paint_timer);
QMessageBox::information(this,"提示","游戏结束！");
    for(int i=0;i<AREA_ROW;i++){
        for(int j=0;j<AREA_COL;++j){
            game_area[i][j]=0;
        }
    }

    ui->pushButton->setText("开始游戏");
    falg=0;
}

void MainWindow::ResetBlock(){
    block_copy(cur_block,next_block);
    GetBoarder(cur_block,cur_boarder);

    int blockid=rand()%7;
    CreateBlock(next_block,blockid);

    block_point start_point;
    start_point.pos_x=AREA_COL/2-2;
    start_point.pos_y=0;
    block_pos=start_point;
}

void MainWindow::CreateBlock(int block[4][4], int blockid){
    switch (blockid) {
    case 0:
        block_copy(block,item1);
        break;
    case 1:
        block_copy(block,item2);
        break;
    case 2:
        block_copy(block,item3);
        break;
    case 3:
        block_copy(block,item4);
        break;
    case 4:
        block_copy(block,item5);
        break;
    case 5:
        block_copy(block,item6);
        break;
    case 6:
        block_copy(block,item7);
        break;
    default:
        break;
    }
}
void MainWindow::GetBoarder(int block[4][4], Boarder &blockboarder){
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            if(block[i][j]==1){
                blockboarder.downboard=i;
                break;
            }
        }
    }
    for(int i=3;i>=0;--i){
        for(int j=0;j<4;++j){
            if(block[i][j]==1){
                blockboarder.upboard=i;
                break;
            }
        }
    }
    for(int j=0;j<4;++j){
        for(int i=0;i<4;++i){
            if(block[i][j]==1){
                blockboarder.rightboard=j;
                break;
            }
        }
    }
    for(int j=3;j>=0;--j){
        for(int i=0;i<4;++i){
            if(block[i][j]==1){
                blockboarder.leftboard=j;
                break;
            }
        }
    }
}
void MainWindow::timerEvent(QTimerEvent *event){
    if(event->timerId()==game_timer){
        BlockMove(Down);

    }
    if(event->timerId()==paint_timer){
        update();
    }
    //qDebug()<<"TimerId:"<<event->timerId();
}
void MainWindow::paintEvent(QPaintEvent*event){
    QPainter painter(this);
    //游戏场景边框
    painter.setBrush(QBrush(Qt::white,Qt::SolidPattern));
    painter.drawRect(MARGIN,MARGIN,AREA_COL*BLOCK_SIZE,AREA_ROW*BLOCK_SIZE);
    //图案预告
    painter.setBrush(QBrush(Qt::white,Qt::SolidPattern));
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            if(next_block[i][j]==1){
                painter.drawRect(MARGIN*3+AREA_COL*BLOCK_SIZE+j*BLOCK_SIZE,MARGIN*5+i*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
            }

        }
    }
    //得分绘制
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial",20));
    painter.drawText(QRect(MARGIN*3+AREA_COL*BLOCK_SIZE,MARGIN*12+4*BLOCK_SIZE,BLOCK_SIZE*10,BLOCK_SIZE*3),
                     Qt::AlignLeft,"得分: "+QString::number(score));
    //移动方块和稳定方块绘制
    painter.setPen(Qt::black);
    for(int i=0;i<AREA_ROW;++i){
        for(int j=0;j<AREA_COL;++j){
            //移动方块
            if(game_area[i][j]==1){
                painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));
                painter.drawRect(j*BLOCK_SIZE+MARGIN,i*BLOCK_SIZE+MARGIN,BLOCK_SIZE,BLOCK_SIZE);
            }
            //稳定方块
            else if(game_area[i][j]==2){
                painter.setBrush(QBrush(Qt::blue,Qt::SolidPattern));
                painter.drawRect(j*BLOCK_SIZE+MARGIN,i*BLOCK_SIZE+MARGIN,BLOCK_SIZE,BLOCK_SIZE);
            }
        }
    }
}
void MainWindow::ConvertStable(int x, int y){
    for(int i=cur_boarder.upboard;i<=cur_boarder.downboard;++i){
        for(int j=cur_boarder.leftboard;j<=cur_boarder.rightboard;++j){
            if(cur_block[i][j]==1){
                game_area[y+i][x+j]=2;
            }
        }
    }
}
void MainWindow::BlockRotate(int block[4][4]){
    int temp[4][4];
    for(int i=0;i<4;i++)
        for(int j=0;j<4;++j){
            temp[3-j][i]=block[i][j];
        }
    for(int i=0;i<4;++i){
        for(int j=0;j<4;++j){
            block[i][j]=temp[i][j];
        }
    }
}
bool MainWindow::IsCollide(int x, int y, Direction dir){
    int temp[4][4];
    block_copy(temp,cur_block);
    Boarder temp_boarder;
    GetBoarder(temp,temp_boarder);
    switch (dir) {
    case Up:
        BlockRotate(temp);
        GetBoarder(temp,temp_boarder);
        break;
    case Down:
        y+=1;
        break;
    case Left:
        x-=1;
        break;
    case Right:
        x+=1;
        break;
    default:
        break;
    }
    for(int i=temp_boarder.upboard;i<=temp_boarder.downboard;++i){
        for(int j=temp_boarder.leftboard;j<=temp_boarder.rightboard;++j){
            if(game_area[y+i][x+j]==2&&temp[i][j]==1||x+temp_boarder.leftboard<0||x+temp_boarder.rightboard>AREA_COL-1){
                return true;
            }
        }
    }
    return false;
}
void MainWindow::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_Up:
        BlockMove(Up);
        break;
    case Qt::Key_Down:
        BlockMove(Down);
        break;
    case Qt::Key_Left:
        BlockMove(Left);
        break;
    case Qt::Key_Right:
        BlockMove(Right);
        break;
    case Qt::Key_Space:
        BlockMove(Space);
        break;
    default:
        break;
    }
}
void MainWindow::BlockMove(Direction dir){
    switch (dir) {
    case Down:
        //方块到达边界则不移动
        if(block_pos.pos_y+cur_boarder.downboard==AREA_ROW-1){
            ConvertStable(block_pos.pos_x,block_pos.pos_y);
            ResetBlock();
            break;
        }
        //碰撞检测
        if(IsCollide(block_pos.pos_x,block_pos.pos_y,Down)){
            ConvertStable(block_pos.pos_x,block_pos.pos_y);
            ResetBlock();
            break;
        }
        //恢复方块上方，清除移动过程的方块痕迹
        for(int j=cur_boarder.leftboard;j<=cur_boarder.rightboard;++j){
            game_area[block_pos.pos_y][block_pos.pos_x+j]=0;
        }
        //此处因为图形形状边界问题，上面清除的时候没有加边界，所以下面需要i->0-4全部重新拷贝一遍
        //如果上面恢复的时候 加了边界，则下面拷贝i可以从upborder->downborder
        block_pos.pos_y+=1;
        for(int i=0;i<4;++i){
            for(int j=cur_boarder.leftboard;j<=cur_boarder.rightboard;++j){
                //注意场景数组不越界 且不是稳定方块
                if(block_pos.pos_y+i<=AREA_ROW-1&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2){
                game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];}
            }
        }
        break;
    case Left:
        if(block_pos.pos_x+cur_boarder.leftboard==0||IsCollide(block_pos.pos_x,block_pos.pos_y,Left)){
            break;
        }
        for(int i=cur_boarder.upboard;i<=cur_boarder.downboard;++i){
            game_area[block_pos.pos_y+i][block_pos.pos_x+3]=0;
        }
        block_pos.pos_x-=1;
        for(int i=cur_boarder.upboard;i<=cur_boarder.downboard;++i){
            for(int j=0;j<4;++j){
                if(block_pos.pos_x+j>=0&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2){
                game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
                }
            }
        }
        break;
    case Right:
        if(block_pos.pos_x+cur_boarder.rightboard==AREA_COL-1&&IsCollide(block_pos.pos_x,block_pos.pos_y,Right)){
            break;
        }
        for(int i=cur_boarder.upboard;i<=cur_boarder.downboard;++i){
            game_area[block_pos.pos_y+i][block_pos.pos_x]=0;
        }
        block_pos.pos_x+=1;
        for(int i=cur_boarder.upboard;i<=cur_boarder.downboard;++i){
            for(int j=0;j<4;++j){
                if(block_pos.pos_x+j<=AREA_COL-1&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2){
                    game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
                }
            }
        }
        break;
    case Up:
        if(IsCollide(block_pos.pos_x,block_pos.pos_y,Up)){
            break;
        }
        //逆时针旋转90度
        BlockRotate(cur_block);
        //GetBoarder(cur_block,cur_boarder);
        //旋转后重新拷贝图案
        for(int i=0;i<4;++i){
            for(int j=0;j<4;++j){
                game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
            }
        }
        GetBoarder(cur_block,cur_boarder);
        break;
     case Space://一次到底
        while(block_pos.pos_y+cur_boarder.downboard<AREA_ROW-1&&!IsCollide(block_pos.pos_x,block_pos.pos_y,Down)){
            for(int i=cur_boarder.leftboard;i<=cur_boarder.rightboard;++i){
                game_area[block_pos.pos_y][block_pos.pos_x+i]=0;
            }
            block_pos.pos_y+=1;
            for(int i=0;i<4;++i){
                for(int j=cur_boarder.leftboard;j<=cur_boarder.rightboard;++j){
                    if(block_pos.pos_y+i<=AREA_ROW-1&&game_area[block_pos.pos_y+i][block_pos.pos_x+j]!=2){
                    game_area[block_pos.pos_y+i][block_pos.pos_x+j]=cur_block[i][j];
                    }
                }
            }
        }
        ConvertStable(block_pos.pos_x,block_pos.pos_y);
        ResetBlock();
        break;
    default:
        break;
    }
    //消行
    int i=AREA_ROW-1;
    int linecnt=0;
    while(i>=1){
        bool linefull=true;
        for(int j=0;j<AREA_COL;++j){
            if(game_area[i][j]==0){
                linefull=false;
                i--;
                break;
            }
        }
        if(linefull){
            for(int k=i;k>=1;--k){
                for(int j=0;j<AREA_COL;++j){
                    game_area[k][j]=game_area[k-1][j];
                }
            }
            linecnt++;
        }
    }
    score+=linecnt*10;
    //判断游戏结束
    for(int j=0;j<AREA_COL;++j){
        if(game_area[0][j]==2){GameOver();}
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    this->destroy();
}
