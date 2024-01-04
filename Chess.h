#ifndef CLIONPROJECTS_CHESS_H
#define CLIONPROJECTS_CHESS_H
#include <vector>
#include <iostream>
#include <iomanip>
#include <raylib.h>

using namespace std;
#define BOARD_SIZE 14 //棋盘大小
#define TILE_SIZE 80 //单位格子大小
#define BOARD_OFFSET 100 //棋盘与屏幕的间距
#define R 38 //棋子大小
#define HUOWU 100000 //活五
#define HUOSI 10000; //活四
#define HUOSAN 1000;
#define HUOER 100;
Color boardColor = {222, 184, 135, 255}; //棋盘颜色
Color starColor = BLACK; //星元颜色
const int screenWidth = BOARD_OFFSET * 2 + BOARD_SIZE * TILE_SIZE;  //屏幕宽度
const int screenHeight = BOARD_OFFSET * 2 + BOARD_SIZE * TILE_SIZE; //屏幕高度

struct piece{  //单位棋子
    int step; //用来显示第几步的，没啥用
    int x; //坐标
    int y;
    int direction_value[9]; //八个方向各有多少颗棋子 0-4
    int value;
};
struct Global{
    int member; //0代表空 1代表黑棋 -1代表白棋
    piece* self;
    int state; //表示状态
};
class Chess{
private:
    int count = 0, win = 0, num_white = 0, num_black = 0; bool AI = true;
    piece white [50]{};   piece black [50]{};  //白棋 黑棋
    Global global [17][17] = {0, nullptr};  //1-15行 1-15列

    vector<piece> k_black{};
    vector<piece> k_white{};
    vector<piece> a_white{};
    vector<piece> a_black{};
    bool kill_black = false;
    bool kill_white = false;
    bool attack_black = false;
    bool attack_white = false;
    bool First_cut = true;
    piece initial{};
public:
    void Chief ();

    //绘图
    static void DRAW_Black (int x, int y, int i);
    static void DRAW_White (int x, int y, int i);
    void DRAW_Basis () const ;
    void DRAW_Piece ();


    //主体逻辑
    void press ();
    void progress(int x, int y);
    void detect (piece &object);
    bool judge_line (piece &piece, int direction);
    int judge (piece &piece, int direction);

    //悔棋功能主体
    void Recall ();
    void Recall(piece &piece, int direction);
    void Recall(piece &object);

    //一些工具
    Global TOOL_Connect_1 (int x, int y, int direction);
    Global TOOL_Connect_2(int x, int y, int direction);
    Global TOOL_Connect_3(int x, int y, int direction);
    Global TOOL_Connect_4(int x, int y, int direction);
    bool TOOL_Legal(int &x, int &y, int mouseX,int mouseY);
    void TOOL_Debug();
    void TOOL_Estimate(piece &p);


    //AI部分主体
    vector<piece> possible();
    piece dfs(int n,piece p);
    piece first_cut(int n);
    bool second_cut(int n, piece &p);
};
Chess chess;
void Chess::Chief(){
    DRAW_Basis();
    if(!win){
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) Recall();
        press();
    }
    DRAW_Piece();
}

void Chess::DRAW_Basis()const {
    if(win == 1)    DrawText(TextFormat("BLACK WIN !"),screenWidth-BOARD_OFFSET-200,screenHeight-BOARD_OFFSET+10,40,BLACK);
    if(win == -1)    DrawText(TextFormat("WHITE WIN !"),screenWidth-BOARD_OFFSET-200,screenHeight-BOARD_OFFSET+10,40,WHITE);
    DrawText(TextFormat("recall: click right button"),screenWidth-900,BOARD_OFFSET-80,40,BLACK);
    for (int i = 0; i <= BOARD_SIZE; i++) {
        if(i<9) DrawText(TextFormat("%d",i+1),BOARD_OFFSET + i * TILE_SIZE-5,BOARD_OFFSET-20,20,BLACK);
        if(i>8) DrawText(TextFormat("%d",i+1),BOARD_OFFSET + i * TILE_SIZE-10,BOARD_OFFSET-20,20,BLACK);
        DrawLine(BOARD_OFFSET + i * TILE_SIZE, BOARD_OFFSET, BOARD_OFFSET + i * TILE_SIZE, BOARD_OFFSET + BOARD_SIZE * TILE_SIZE, BLACK);
    }
    for (int j = 0; j <= BOARD_SIZE; j++) {
        if(j<9) DrawText(TextFormat("%d",j+1),BOARD_OFFSET-15,BOARD_OFFSET + j * TILE_SIZE-10,20,BLACK);
        if(j>8) DrawText(TextFormat("%d",j+1),BOARD_OFFSET-20,BOARD_OFFSET + j * TILE_SIZE-10,20,BLACK);
        DrawLine(BOARD_OFFSET, BOARD_OFFSET + j * TILE_SIZE, BOARD_OFFSET + BOARD_SIZE * TILE_SIZE, BOARD_OFFSET + j * TILE_SIZE, BLACK);
    }
    DrawCircle(BOARD_OFFSET + 3*TILE_SIZE , BOARD_OFFSET + 3*TILE_SIZE , 5, starColor);
    DrawCircle(BOARD_OFFSET + 3*TILE_SIZE , BOARD_OFFSET + 11*TILE_SIZE , 5, starColor);
    DrawCircle(BOARD_OFFSET + 11*TILE_SIZE , BOARD_OFFSET + 3*TILE_SIZE , 5, starColor);
    DrawCircle(BOARD_OFFSET + 11*TILE_SIZE , BOARD_OFFSET + 11*TILE_SIZE , 5, starColor);
    DrawCircle(BOARD_OFFSET + 7*TILE_SIZE , BOARD_OFFSET + 7*TILE_SIZE , 5, starColor);
}
void Chess::DRAW_Piece(){
    for(int i=0;i<num_white;i++){
        DRAW_White(white[i].x,white[i].y,white[i].step);
    }
    for(int i=0;i<num_black;i++){
        DRAW_Black(black[i].x,black[i].y,black[i].step);
    }
}
void Chess::DRAW_Black(int x, int y, int i){
    DrawCircle(BOARD_OFFSET+(x-1)*TILE_SIZE,BOARD_OFFSET+(y-1)*TILE_SIZE,R,BLACK);
    if(i<10) DrawText(TextFormat("%d",i),BOARD_OFFSET+(x-1)*TILE_SIZE-4,BOARD_OFFSET+(y-1)*TILE_SIZE-10,30,WHITE);
    if(i>9) DrawText(TextFormat("%d",i),BOARD_OFFSET+(x-1)*TILE_SIZE-10,BOARD_OFFSET+(y-1)*TILE_SIZE-15,30,WHITE);
}
void Chess::DRAW_White(int x, int y,int i){
    DrawCircle(BOARD_OFFSET+(x-1)*TILE_SIZE,BOARD_OFFSET+(y-1)*TILE_SIZE,R,WHITE);
    if(i<10) DrawText(TextFormat("%d",i),BOARD_OFFSET+(x-1)*TILE_SIZE-4,BOARD_OFFSET+(y-1)*TILE_SIZE-10,30,BLACK);
    if(i>9) DrawText(TextFormat("%d",i),BOARD_OFFSET+(x-1)*TILE_SIZE-10,BOARD_OFFSET+(y-1)*TILE_SIZE-15,30,BLACK);
}


void Chess::Recall(){
    if(count==0) return;
    piece new_piece = {count,0,0,0,0,0};
    Global g{0, nullptr};
    if(count%2==1){
        count --;
        num_black --;
        Recall(black[num_black]);
        global[black[num_black].y][black[num_black].x] = g;
        black[num_black] = new_piece;
        return;
    }
    if(count%2==0){
        count --;
        num_white --;
        Recall(white[num_white]);
        global[white[num_white].y][white[num_white].x] = g;
        white[num_white] = new_piece;
        return;
    }
};
void Chess::Recall(piece &piece, int direction){
    int addition = 0;
    int member = global[piece.y][piece.x].member;
    struct piece* P = global[piece.y][piece.x].self;

    if(member==TOOL_Connect_1(piece.x,piece.y,direction).member){
        if(member==TOOL_Connect_1(piece.x,piece.y,8-direction).member) addition = piece.direction_value[8-direction];
        for(int i=1;i<=4;i++){
            if(global[P->y][P->x].member!=TOOL_Connect_1(P->x,P->y,direction).member) break;
            P = TOOL_Connect_1(P->x,P->y,direction).self;
            piece.direction_value[direction] ++; //数到一次加一次
            P ->direction_value[8-direction] -= 1+addition;
        }
        P = global[piece.y][piece.x].self;
        if(addition!=0){
            for(int i=1;i<=addition;i++){
                P = TOOL_Connect_1(P->x,P->y,8-direction).self;
                P ->direction_value[direction] -= piece.direction_value[direction];
            }
        }
    }
    if(member==TOOL_Connect_1(piece.x,piece.y,direction).member*(-1)){
        piece.direction_value[direction] = 0;
        TOOL_Connect_1(piece.x,piece.y,direction).self->direction_value[8-direction] = 0;
    }
}
void Chess::Recall(piece &object){
    piece new_piece = {count,0,0,0,0,0};
    new_piece.x = object.x;  //如果都合法则在棋子类中实现落子 并且在黑棋或白棋类中存储坐标
    new_piece.y = object.y;

    for(int k=0;k<4;k++){
        Recall(new_piece,k);
        Recall(new_piece,8-k);
    }
}


bool Chess::TOOL_Legal(int &x, int &y, int mouseX,int mouseY){
    if(mouseX<BOARD_OFFSET || mouseY<BOARD_OFFSET) return false;
    if(screenWidth-mouseX<BOARD_OFFSET || screenHeight-mouseY<BOARD_OFFSET) return false;
    if((mouseX-BOARD_OFFSET)%TILE_SIZE <= 20) x = (mouseX-BOARD_OFFSET) / TILE_SIZE +1;
    if((mouseX-BOARD_OFFSET)%TILE_SIZE >= TILE_SIZE-20) x = ((mouseX-BOARD_OFFSET) / TILE_SIZE) + 2 ;
    if((mouseY-BOARD_OFFSET)%TILE_SIZE <= 20) y = (mouseY-BOARD_OFFSET) / TILE_SIZE +1;
    if((mouseY-BOARD_OFFSET)%TILE_SIZE >= TILE_SIZE-20) y = ((mouseY-BOARD_OFFSET) / TILE_SIZE) + 2 ;
    if(x==(-1) || y==(-1)) return  false;
    if(global[y][x].member) return false;
    return true;
}
void Chess::TOOL_Debug(){
    system("cls");
    cout.setf(ios::right);
    for(int i=0;i<num_white;i++){
        cout<<setw(2)<<i*2+1<<": ";
        for(int j=0;j<=8;j++){
            if(j==4) continue;
            if(black[i].direction_value[j]<0){
                cout.setf(ios::left);
                cout<<setw(2)<<black[i].direction_value[j];
                cout.setf(ios::right);
                cout<<" ";
                continue;
            }
            cout<<setw(2)<<black[i].direction_value[j];
            cout<<" ";
        }
        cout<<setw(6)<<black[i].value<<'\t';
        cout<<setw(2)<<i*2+2<<": ";
        for(int j=0;j<=8;j++){
            if(j==4) continue;
            if(white[i].direction_value[j]<0){
                cout.setf(ios::left);
                cout<<setw(2)<<white[i].direction_value[j];
                cout.setf(ios::right);
                cout<<" ";
                continue;
            }
            cout<<setw(2)<<white[i].direction_value[j];
            cout<<" ";
        }
        cout<<setw(6)<<white[i].value<<endl;
    }
}
void Chess::TOOL_Estimate(piece &p){
    piece temp{};
    p.value = 1;
    int a, b;
    for(int i=0;i<=8;i++){ //检验场上已有的情况 死四活四都是kill（再下一步就死） 活三是attack 双活二是attack
        if(i==4) continue;
        a = p.direction_value[i];
        b = p.direction_value[8-i];
        if(a==4 || b==4){
            p.value += HUOWU;
        }
        if(a==0&&b==3){
            temp.x =  p.x+(i%3)-1;
            temp.y =  p.y+(i/3)-1;
            if(global[p.y][p.x].member==1){
                kill_black = true;
                k_black.push_back(temp);
            }
            if(global[p.y][p.x].member==-1){
                kill_white = true;
                k_white.push_back(temp);
            }
            p.value += HUOSI;
        }
        if(a==0&&b==2){
            temp.x =  p.x+3*(((8-i)%3)-1);
            temp.y =  p.y+3*(((8-i)/3)-1);
            piece p0 = *TOOL_Connect_2(p.x,p.y,8-i).self;
            Global g = TOOL_Connect_4(p.x,p.y,8-i);
            if(p0.direction_value[8-i]==0){
                p.value += HUOSAN;
                if(g.member==global[p.y][p.x].member){
                    if(global[p.y][p.x].member==1){
                        kill_black = true; //跳四
                        k_black.push_back(temp);
                    }
                    if(global[p.y][p.x].member==-1){
                        kill_white = true;
                        k_white.push_back(temp);
                    }
                    p.value += HUOSI;
                    continue;//不再标记活三
                }
                if(global[p.y][p.x].member==1){
                    attack_black = true;
                    a_black.push_back(temp); //活三
                }
                if(global[p.y][p.x].member==-1) {
                    attack_white = true;
                    a_white.push_back(temp); //活三
                }
            }
        } //可能死三，可能活三，可能跳四
        if(a==0&&b==1){
            temp.x =  p.x+2*(i%3)-1;
            temp.y =  p.y+2*(i/3)-1;
            Global g2 = TOOL_Connect_2(p.x,p.y,8-i);
            Global g3 = TOOL_Connect_3(p.x,p.y,8-i);
            Global g4 = TOOL_Connect_4(p.x,p.y,8-i);
            if(g2.member==0){
                p.value += HUOER;
                if(g3.member==global[p.y][p.x].member && g4.member==global[p.y][p.x].member){
                    if(global[p.y][p.x].member==1){
                        kill_black = true;
                        k_black.push_back(temp);
                    }
                    if(global[p.y][p.x].member==-1){
                        kill_white = true;
                        k_white.push_back(temp);
                    }
                    p.value += HUOSI;
                }
            }
        } //可能活二， 可能跳四
    }
}
Global Chess::TOOL_Connect_1(int x, int y, int direction){
    x += (direction%3)-1;
    y += (direction/3)-1;
    return global[y][x];
}
Global Chess::TOOL_Connect_2(int x, int y, int direction){
    x += 2*((direction%3)-1);
    y += 2*((direction/3)-1);
    return global[y][x];
}
Global Chess::TOOL_Connect_3(int x, int y, int direction){
    x += 3*((direction%3)-1);
    y += 3*((direction/3)-1);
    return global[y][x];
}
Global Chess::TOOL_Connect_4(int x, int y, int direction){
    x += 4*((direction%3)-1);
    y += 4*((direction/3)-1);
    return global[y][x];
}



void Chess::press(){
    int x=-1, y=-1;
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        if(TOOL_Legal(x, y, mouseX, mouseY)){ //如果合法则初始化
            AI = true;
            progress(x,y);
            piece p = dfs(1,*global[y][x].self);
            AI = true;
            progress(p.x,p.y);
        };

    }
}
void Chess::progress(int x, int y){
    count ++;
    piece new_piece = {count,0,0,0,0,0};
    new_piece.x = x;  //如果都合法则在棋子类中实现落子 并且在黑棋或白棋类中存储坐标
    new_piece.y = y;


    if(count%2==1){
        global[y][x].member = 1;
        black[num_black] = new_piece;
        global[y][x].self = &black[num_black];
        detect(black[num_black]);
        num_black += 1;
    }

    if(count%2==0){
        global[y][x].member = -1;
        white[num_white] = new_piece;
        global[y][x].self = &white[num_white]; //注意第一个棋子存到向量组的第零个
        detect (white[num_white]);
        num_white ++;
    }

    for(int i=0;i<num_black;i++){
        TOOL_Estimate(black[i]);
        TOOL_Estimate(white[i]);
    };
    TOOL_Debug();
} //初始化这颗棋子
void Chess::detect(piece &object){
    for(int k=0;k<4;k++){
        if(judge_line(object,k)){
            if(count%2==1 && AI) win = 1;
            if(count%2==0 && AI) win = -1;
        }
    }
}
int Chess::judge(piece &piece, int direction){
    int key = 0;
    int addition = 0;
    int member = global[piece.y][piece.x].member;
    struct piece* P = global[piece.y][piece.x].self;

    if(member==TOOL_Connect_1(piece.x,piece.y,direction).member){
        if(member==TOOL_Connect_1(piece.x,piece.y,8-direction).member) addition = piece.direction_value[8-direction];
        for(int i=1;i<=4;i++){
            if(global[P->y][P->x].member!=TOOL_Connect_1(P->x,P->y,direction).member) break;
            key ++;
            P = TOOL_Connect_1(P->x,P->y,direction).self;
            piece.direction_value[direction] ++; //数到一次加一次
            P ->direction_value[8-direction] += 1+addition;
        }
        P = global[piece.y][piece.x].self;
        if(addition!=0){
            for(int i=1;i<=addition;i++){
                P = TOOL_Connect_1(P->x,P->y,8-direction).self;
                P ->direction_value[direction] += piece.direction_value[direction];
            }
        }
    }
    if(member==TOOL_Connect_1(piece.x,piece.y,direction).member*(-1)){
        piece.direction_value[direction] = -1;
        TOOL_Connect_1(piece.x,piece.y,direction).self->direction_value[8-direction] = -1;
    }
    return key;
}
bool Chess::judge_line(piece &piece, int direction){
    if(judge(piece,direction)+judge(piece,8-direction)>=4){
        return true;
    }
    return false;
}



vector<piece> Chess::possible() {
    vector<piece> P;
    piece po{};
    Global g[18][18]{};
    for (piece element: black) {
        g[element.x][element.y].member = 1;
        if(global[element.y][element.x].member==0) break;
        for (int direction = 0; direction < 9; direction++) {
            if(direction==4) continue;
            if (element.direction_value[direction] == 0) {
                po.x = element.x + direction % 3 - 1;
                po.y = element.y + direction / 3 - 1;
                if(po.x>15 || po.y>15) continue;
                if(g[po.y][po.x].member!=0) continue;
                g[po.y][po.x].member = 1;
                P.push_back(po);
            }
        }
    }
    for (piece element: white) {
        g[element.x][element.y].member = 1;
        if(global[element.y][element.x].member==0) break;
        for (int direction = 0; direction < 9; direction++) {
            if(direction==4) continue;
            if (element.direction_value[direction] == 0) {
                po.x = element.x + direction % 3 - 1;
                po.y = element.y + direction / 3 - 1;
                if(po.x>15 || po.y>15) continue;
                if(g[po.y][po.x].member==1) continue;
                g[po.y][po.x].member = 1;
                P.push_back(po);
            }
        }
    }
    return P;
}
piece Chess::dfs(int n,piece p){
    system("cls");
    AI = false; //dfs时不打印棋子
    First_cut = true;
    piece P = first_cut(n);
    if(First_cut){
        return P;
    } //先剪枝


    if(n==2){
       return initial;
    } //出口

    for(piece element : possible()){
        progress(element.x,element.y);
        //cout<<"("<<element.x<<","<<element.y<<"):";

        if(second_cut(n,element)){
            Recall();
            cout<<" cut"<<endl;
            continue;
        } //后剪枝

        initial = element;
        //cout<<" uncut"<<endl;
        Recall();
    }
    if(global[initial.y][initial.x].member==0){
        return initial;
    }

}
piece Chess::first_cut(int n){
    piece p{};
    if(n==1)
    {
        if(count%2==1){
            if(kill_white){
                p = k_white[k_white.size()-1];
                k_white.pop_back();
                kill_white = false;
                return p;
            } //如果自己再下一招就赢了
            if(kill_black){
                p = k_black[k_black.size()-1];
                k_black.pop_back();
                kill_black = false;
                return p;
            } //如果对方再下一招就赢了
            if(attack_white){
                p = a_white[a_white.size()-1];
                a_white.pop_back();
                attack_white = false;
                return p;
            } //对方不能一招制胜的时候自己下完这步稳赢
            if(attack_black){
                p = a_black[a_black.size()-1];
                a_black.pop_back();
                attack_black = false;
                return p;
            }
        }
        if(count%2==0){
            if(kill_black){
                p = k_black[k_black.size()-1];
                k_black.pop_back();
                kill_black = false;
                return p;
            }
            if(kill_white){
                p = k_white[k_white.size()-1];
                k_white.pop_back();
                kill_white = false;
                return p;
            }
            if(attack_black){
                p = a_black[a_black.size()-1];
                a_black.pop_back();
                attack_black = false;
                return p;
            }
            if(attack_white){
                p = a_white[a_white.size()-1];
                a_white.pop_back();
                attack_white = false;
                return p;
            }
        }
    }

    First_cut = false; return p;
}
bool Chess::second_cut(int n, piece &p){
    if(global[p.y][p.x].self->value<50 && count>2){
        return true;
    }
    return false;
}

#endif //CLIONPROJECTS_CHESS_H