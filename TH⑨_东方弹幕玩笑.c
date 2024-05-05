#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include<stdlib.h>
//包含的库————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
#define HIGH 20                                 //定义高度
#define WIDE 35                                 //定义宽度
#define UP 72                                   //方向键：上
#define DOWN 80                                 //方向键：下
#define LEFT 75                                 //方向键：左
#define RIGHT 77                                //方向键：右
#define SPACE 32                                //空格键
#define ESC 27                                  //Esc键
#define EnemyNum 5                              //小怪数
//定义的宏————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
struct Face                                     //定义一个窗口大小覆盖的二维数组，存储每一个点位的小方块状态
{
    int data[HIGH][WIDE];                       //有无方块      0无方块，1地图方块，2玩家方块，3Boss方块，4boss弹幕，5玩家弹幕, 6小怪~
    int color[HIGH][WIDE];                      //方块颜色
}face;     

struct BOSS{
    int x,y;
    int HP;
    int Form;
}Boss,enemy[EnemyNum];

struct PLAYER
{
    int x,y;
    int HP;
    int BOMB;
}Player;

struct GARDEN
{
    int x,y;
    int HP;
    int Shape[2][2];
};

//定义的结构—————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————— 
struct GARDEN Garden[4];                    //BOSS的4个使魔
int IsPrime;
char NameOfSpellCard[30];                   //当前符卡名称
int DanmuMove;                              //用于控制弹幕频率
int MOVE = 1;                               //频率变量
int Std_Speed = 5;
int Std_Time = 5;
int enemy_max = 100000000;
int enemy_num = 0;
int Form1_Time = 0;
int Score=0;
//全局变量————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————
void enemy_update()
{
	static int enemy_speed = 0;
	static int enemy_time = 0;
	int flag = 0;
	if (enemy_speed < Std_Speed){				//依靠循环来减速
		enemy_speed++;
    }
	if (enemy_time < Std_Time){
		enemy_time++;
    }
	if (enemy_num < enemy_max && enemy_time >= Std_Time){
		int i, j;
		do
		{
			i = rand() % (HIGH / 5) + 2;
			j = rand() % (WIDE - 4) + 2;		//j的范围:[2, WIDE - 3]
		} while (face.data[i][j] != 0);
		face.data[i][j] = 6;
		enemy_num++;
		enemy_time = 0;
	}

	if (enemy_speed >= Std_Speed){
	
		flag = 1;
		enemy_speed = 0;
	}

	for (int i = HIGH - 1; i >= 0; i--){
		for (int j = WIDE - 1; j >= 0; j--){
			if (face.data[i][j] == 6){			//若此处有小怪
				if (i == HIGH - 1){			//小怪飞到边界	
					enemy_num--;
					face.data[i][j] = 0;
				}
				else if (i < HIGH - 1 && face.data[i + 1][j] == 5){//检测是否被子弹击中
                    Form1_Time++;
                    Score++;
					printf("\a");
					enemy_num--;
					face.data[i][j] = 0;
				}else if (flag){					//flag为1更新小怪位置
					face.data[i + 1][j] = 6;
					face.data[i][j] = 0;
				}else if( i == Player.y && j == Player.x ){             //小怪撞到自机
                    printf("\a");                       //Biu~声音
                    Player.HP--;                        //自机残机-1
                    face.data[i][j] = 0;                //小怪消失
                    Player.x = WIDE/2;                  //自机归位
                    Player.y = HIGH - 5;
                    face.data[Player.y][Player.x] = 2;
                }
			}
		}
	}
}
void HideCursor()                               //隐藏光标，包含在Windos.h内
{
	CONSOLE_CURSOR_INFO curInfo;                //定义光标信息的结构体变量
	curInfo.dwSize = 1;                         //如果没赋值的话，隐藏光标无效
	curInfo.bVisible = FALSE;                   //将光标设置为不可见
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //获取控制台句柄
	SetConsoleCursorInfo(handle, &curInfo);     //设置光标信息
}                            
void CursorJump(int x, int y)                   //光标跳转，内含参数包含在Windos.h内
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, pos);
}                    
void color(int c)                               //颜色设置
{
	switch (c)
	{
	case 0:
		c = 13; //紫色
		break;
	case 1:
	case 2:
		c = 12; //红色
		break;
	case 3:
	case 4:
		c = 10; //绿色
		break;
	case 5:
		c = 14; //黄色
		break;
	case 6:
		c = 11; //浅蓝色
		break;
	default:
		c = 7; //白色
		break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); //颜色设置
	//注：SetConsoleTextAttribute是一个API（应用程序编程接口）
}
void makeMap()                                  //地图绘制
{
    system("cls");                                          //清空屏幕
    for( int i = 1 ; i<=HIGH ; i++ ){
        for( int j =1  ; j<=WIDE ; j++ ){
            if( i == 1 || j == 1 || j == WIDE || i == HIGH ){
                face.data[i][j] = 1;
                CursorJump(j,i);
                printf("#");
            }
        }
    }                                                                       //打印自机
}
void Menue()
{
    for( int i = 1 ; i<=HIGH ; i++ ){
        for( int j =1  ; j<=WIDE ; j++ ){
            if( i == 1 || j == 1 || j == WIDE || i == HIGH ){
                face.data[i][j] = 1;
                CursorJump(j,i);
                printf("#");
            }else if( i == 7 && j == 7 ){
                CursorJump(j,i);
                printf("東方弹幕玩笑");
            }else if( i == HIGH - 15 && j == 4 ){
                CursorJump(j,i);
                printf("★Start(S)");
            }else if( i == HIGH - 13 && j == 4 ){
                CursorJump(j,i);
                printf("★Quit(Q)");
            }
        }
    }
    while(!IsPrime){
        char isPrime;
        isPrime = getch();
        if( isPrime == 's'|| isPrime == 'S' ){
            IsPrime = 1;
            makeMap();
        }else if( isPrime == 'q' || isPrime == 'Q' ){
            exit(0);
        }
    }
}
void BossForm()                                  //Boss阶段
{
    if( Form1_Time == 5 ){
        //阶段2;
        //Boss.Form = 2;
    }/*else if( 使魔均被击破 ){
        //阶段3;
        Boss.Form = 3;
        
    }*/
}
void DataFirst()                                //数据初始化
{
    DanmuMove = 20;                            //越大使魔弹幕越慢
    

    Player.x = WIDE/2;
    Player.y = HIGH - 5;
    Player.HP = 9;
    Player.BOMB = 3;
    face.data[Player.y][Player.x] = 2;

    Boss.x = NULL;
    Boss.y = NULL;
    Boss.HP = 114;
    Boss.Form = 1;
    //face.data[Boss.y][Boss.x] = 3;

    for( int i = 0 ; i<4 ; i++ ){
        for( int j = 0 ; j<2 ; j++ ){
            for( int k = 0 ; k<2 ; k++ ){
                Garden[i].Shape[j][k] = 1;
            }
        }
    }
}
void DrawEnemy(int k,int x,int y)
{
    CursorJump(x,y);
    for ( int i = 0 ; i < 2 ; i++ ){
        for ( int j = 0 ; j < 2 ; j++ ){
            if ( Garden[k].Shape[i][j] == 1 ){ //如果该位置有方块
                CursorJump(x + j, y + i); //光标跳转到指定位置
                printf("■"); //输出方块
            }
        }
    }
}
void BossAttack(int n)
{
    
    switch (n)
    {
    
    case 1:    
                                       //阶段1，道中小怪，BOSS不出现
        enemy_update();
        break;
    case 2:                                              //BOSS第二阶段： 召唤4个使魔，并发射弹幕，

        Garden[1].x = 5;                                                     //设置使魔位置
        Garden[2].x = 8;
        Garden[3].x = 15;
        Garden[4].x = 23;
        for( int i = 0 ; i<4 ; i++ ){
            Garden[i].y = 4;
        }
        for( int k = 0 ; k<4 ; k++ ){
            face.data[Garden[k].y][Garden[k].x] = 7;
            DrawEnemy(k,Garden[k].x,Garden[k].y);                                   //画出使魔
        }
        if( MOVE % DanmuMove == 0 ){
            for( int i = 0 ; i<4 ; i++ ){
                face.data[Garden[i].y+3][Garden[i].x] = 4;                          //使魔开火
            }
        }
        break;
    case 3:

        break;
    default:

        break;
    }
}
void NOinteractUpdate()
{
    if( Score == 1000 ) {
    	system("cls");
        CursorJump(WIDE/2-7,HIGH/2);
        printf("STAGE CLEAR");
        getchar();
        exit(0);
	}

    if( MOVE < 10000000){                       //MOVE循环递增
        MOVE++;
    }else{
        MOVE = 1;
    }

    BossForm();    
    
    if( Boss.HP == 0 ){
        face.data[Boss.y][Boss.x] = 0;
        Boss.x = NULL;
        Boss.y = NULL;
        system("cls");
        CursorJump(15,15);
        printf("STAGE CLEAR");
        getchar();
        exit(0);
    }
    if( Player.HP <= 0 ){
        system("cls");
        CursorJump(WIDE/2-7,HIGH/2);
        printf("满 身 疮 痍");
        CursorJump(WIDE/2-9,HIGH/2+3);
        printf("你的最终得分为:%d",Score);
        getchar();
        exit(0);
    }
    //更新弹幕
    for( int i = 0 ; i < HIGH ; i++ ){
        for( int j = 0 ; j < WIDE ; j++ ){              
            if( face.data[i][j] == 4 ){                 //若此处有使魔弹幕
                if( i == Player.y && j == Player.x ){   
                    printf("\a");                       //Biu~声音
                    Player.HP--;                        //自机残机-1
                    face.data[i][j] = 2;                //弹幕消失
                    Player.x = WIDE/2;                  //自机归位
                    Player.y = HIGH - 5;
                }else if( i < HIGH  && MOVE % DanmuMove == 0 ){  //使魔弹幕下移
                    face.data[i][j] = 0;
                    face.data[i+1][j] = 4;
                }else{                                  //使魔弹幕触壁消失
                    face.data[i][j] = 0;
                }
            }else if( face.data[i][j] == 5 ){           //若此处有自机弹幕
                if( face.data[i-1][j] == 6 ){           //若击中小怪
                    face.data[i-1][j] = 0;              //小怪消失
                    face.data[i][j] = 0;                //子弹消失
                }
                if( i == Boss.y && j == Boss.x ){       //若击中Boss
                    Boss.HP--;                          //Boss血量-1
                    face.data[i][j] = 3;                //弹幕消失
                }else if( i>2 ){
                    face.data[i][j] = 0;
                    face.data[i-1][j] = 5;
                }else{
                    face.data[i][j] = 0;
                }
            }
        }
    }
}
void YESinteractUpdate()
{
    char input;
	if (_kbhit()) // 判断是否有输入
	{
		input = _getch(); // 根据用户的不同输入来移动，不必输入回车
		if (input == LEFT && Player.x > 2 )
		{
			face.data[Player.y][Player.x] = 0;
			Player.x--; // 位置左移
			face.data[Player.y][Player.x] = 1;
		}
		else if (input == RIGHT && Player.x < WIDE - 2 )
		{
			face.data[Player.y][Player.x] = 0;
			Player.x++; // 位置右移
			face.data[Player.y][Player.x] = 1;
		}
		else if (input == UP && Player.y > 2 )
		{
			face.data[Player.y][Player.x] = 0;
			Player.y--; // 位置上移
			face.data[Player.y][Player.x] = 1;
		}
		else if (input == DOWN )
		{
            if( Player.y != HIGH - 1){
			face.data[Player.y][Player.x] = 0;
			Player.y++; // 位置下移
			face.data[Player.y][Player.x] = 1;
            }
		}
		else if (input == 'z' || input == 'Z') // 发射子弹
		{
			face.data[Player.y-1][Player.x] = 5;

		}else if (input == 'x' || input == 'X'){
            if( Player.BOMB > 0 ){
                Player.BOMB--;
                for( int i = 0 ; i<HIGH ; i++ ){
                    for( int j = 0 ; j<WIDE ; j++ ){
                        if( face.data[i][j] == 6 ){
                            face.data[i][j] = 0;
                        }
                    }
                }
            }
        }
	}
}
void Show()
{
    for( int i = 0 ; i<HIGH ; i++ ){
        for( int j = 0 ; j<WIDE ; j++ ){
            for( int k = 0 ; k<4 ; k++ ){
                if( i == Garden[k].y && j == Garden[k].x ){
                    CursorJump(j,i);
                    printf("%%");
                }
            }
            if( face.data[i][j] == 0 ){             //放置空格
            	CursorJump(j,i); 
                printf(" ");
            }else if(  i == Player.y && j == Player.x ){      //刷新自机位置
            	CursorJump(j,i);
                printf("⑨");
            }else if( i == Boss.y && j == Boss.x ){       //刷新BOSS位置
            	CursorJump(j,i);
                
                color(Boss.Form);
                printf("$");
            }else if( face.data[i][j] == 4 && DanmuMove % 100 == 0 ){       //刷新使魔弹幕
            	CursorJump(j,i);
                printf("↓");
            }else if( face.data[i][j] == 5 ){       //刷新自机弹幕
            	CursorJump(j,i);
                printf("|");
            }else if( face.data[i][j] == 6 ){           //显示小怪
                CursorJump(j,i);
                printf("*");
            }
            if( j == WIDE - 7 && i == 2 ){         //显示UI
            for( int k = 0 ; k<6 ; k++ ){
                face.data[2][WIDE-7+k] = 7;
            }
            CursorJump(j,i);
            printf("残机:%d",Player.HP);
            //后面接残机数
            }else if( j == WIDE - 7 && i == 3 ){   //显示UI
                for( int k = 0 ; k<6 ; k++ ){
                    face.data[3][WIDE-7+k] = 7;
                }
                CursorJump(j,i);
                printf("BOMB:%d",Player.BOMB);
                //后面接BOMB数
            }else if( j == 3 && i == 3 ){
                for( int k = 0 ; k<15 ; k++ ){
                    face.data[4][3+k] = 7;
                }
                for( int k = 0 ; k<11 ; k++ ){
                    face.data[3][3+k] = 7;
                }
                CursorJump(j,i);
                printf("当前符卡:\n # %s",NameOfSpellCard);
                //后面接符卡名字
            }
        }
    }
}

int main()
{
    #pragma warning (disable:4996);             //消除警告
    system("mode con lines=22 cols=37");        //设置cmd窗口的大小
    HideCursor();                               //隐藏光标
    Menue();                                    //打印菜单
    DataFirst();                                //初始化数据
    while(IsPrime){
        Show();
        BossAttack(Boss.Form);
        NOinteractUpdate();
        YESinteractUpdate();
    }
    return 0;
}

    // Boss.x = WIDE/2;
    // Boss.y = 10;
    // Boss.HP = 114;
    // Boss.Form = 1;
    // face.data[Boss.y][Boss.x] = 3;
    


















