#include <stdio.h>
#include <stdlib.h>

//名前の最大文字数
#define NAME_MAX_NUM 32
#define ROLE_STR_MAX_NUM 32
#define ROLES_NUM 2
#define PLAYERS_MAX_NUM 30

//役職を表すRole型を定義
typedef enum{
	VILLAGER, //実際は0
	WEREWOLF  //実際は1
}Role;

char ROLE_STRINGS[][ROLE_STR_MAX_NUM] = {"村人", "狼"};

//今はこの２つだけだね
typedef enum{
	NONE,
	VILLAGER_SIDE,
	WEREWOLF_SIDE
}WinSide;

//プレイヤーを表すPlayer型を定義
typedef struct{
	Role role;               //役職番号
	char name[NAME_MAX_NUM]; //名前
	int is_live;             //生きているなら1, 死んでいるなら0
}Player;

//プレイヤーの情報を表示
void show_players(Player p[], int p_num){
	printf("\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~ << players! >> ~~~~~~~~~~~~~~~~~~~~~~~\n");
	for(int i=0;i<p_num;i++){
//		if (p[i].is_live) printf("No.%d\t name: %s role:%s\n", i, p[i].name, ROLE_STRINGS[p[i].role]);
		if (p[i].is_live) printf("No.%d\t name: %s\n", i, p[i].name);
	}
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

//入力を全部読み捨てる
void clean_stdin(void){
	char c;
	for(;;){
		c = getchar();
		if (c == '\n' || c == EOF) break;
	}
}

//上の関数を待機用として用いる
void wait_key(void) {
	printf("<ENTER>\n");
	clean_stdin();
}

//自分以外の生きているプレイヤーNo.を読み込む
int read_living_other_than_myself_player_num(Player players[], int players_num, int cur_player_num){
	int n;
	for (;;){
		printf("No.");
		scanf("%d", &n);
		if (n<0 || n>=players_num){
			printf("player No.%d は村にいません\n", n);
			continue;
		}
		if (n == cur_player_num){
			printf("自分自身は選択出来ません\n");
			continue;
		}
		if (!players[n].is_live){
			printf("player No.%d は既に死体です\n", n);
			continue;
		}
		wait_key();
		return n;
	}
}

//投票を集計して一人選ぶ
int ballot_winner(int ballot_box[], int size){
	int max = 0;//最大の得票数
	for(int i=0; i<size; i++) {
		if (max < ballot_box[i]) max = ballot_box[i]; 
	}
	int j = 0;//最大の投票数を得た人は何人いるか
	int arr[PLAYERS_MAX_NUM];//最大の投票数を得た人達(j人)
	for(int i=0; i<size; i++) {
		if (ballot_box[i] == max){
			arr[j] = i;
			j++;
		}
	}
	return arr[rand()%j];
}

//画面を綺麗にする
void clear(){
	puts("\033[2J");
};

void mathematical_exercises(){
	int sol;
	int val1 = rand()%1000;
	int val2 = val1 + rand()%10;
	printf("数理演習 問1  %d - %d = ?\n", val2, val1);
	for (;;) {
		printf("A: ");
		scanf("%d", &sol);
		wait_key();
		if (sol == val2 - val1) {
			break;
		}else{
			printf("留年\n\n");
		}
	}
};

WinSide win_side(Player p[], int p_num){
	int werewolf_num = 0;
	int villager_num = 0;
	for(int i=0;i<p_num;i++){
		if(!p[i].is_live)continue;
		switch(p[i].role){
			case VILLAGER: villager_num++;break;
			case WEREWOLF: werewolf_num++;break;
		}
	}
	if(villager_num <= werewolf_num)return WEREWOLF_SIDE;
	if(werewolf_num <= 0)return VILLAGER_SIDE;
	return NONE;
}

int read_roles(Role roles[]){
	int n;
	int each_num[ROLES_NUM];
	for (int i=0;i<ROLES_NUM;i++) each_num[i] = 0;

	for (int count = 0;count<PLAYERS_MAX_NUM;count++) {
		printf("人数は最大 合計%d 人です\n", PLAYERS_MAX_NUM);
		printf("現在の人数はそれぞれ以下の通りです\n");
		for (int j=0;j<ROLES_NUM;j++) {
			printf("%s: \tNo.%d \t%d人\n", ROLE_STRINGS[j], j+1, each_num[j]);
		}
		printf("決定: \tNo.0\n");
		printf("追加する役職番号を入力してください\n");
		for (;;) {
			printf("No.");
			scanf("%d", &n);
			each_num[--n]++;
			if (-1<=n && n<ROLES_NUM) break;
		}
		if (n == -1) return count;
		roles[count] = n;
		clear();
	}
}

void shuffle(Role roles[], int size)
{
	int i = size;
	while (i > 1) {
		int j = rand() % i;
		i--;
		int tmp = roles[i];
		roles[i] = roles[j];
		roles[j] = tmp;
	}
}

int main(void){
	clear();
	Player players[PLAYERS_MAX_NUM];
	Role roles[PLAYERS_MAX_NUM];
	//	役職を割り振る
	//		各役職の人数を入力してもらう
	int players_num = read_roles(roles);
	shuffle(roles, players_num);
	
	//		各プレイヤーの名前を入力してもらう+プレイヤーに役職を教える
	for (int i=0;i<players_num;i++){
		players[i].role = roles[i]; //就職
		sprintf(players[i].name, "hoge");  //名前は適当に
		players[i].is_live = 1; //生きてることにする
	}

	for(;;){
		//昼のターン
		show_players(players, players_num);
		printf("話し合ってください\n");
		wait_key();//入力をまつ
		//処刑したい人を選んでもらう
		clear();
		int ballot_box[PLAYERS_MAX_NUM];//投票箱
		for(int i=0;i<players_num;i++)ballot_box[i] = 0;
		for(int i=0;i<players_num;i++){
			if(!players[i].is_live)continue;//死人は飛ばす
			show_players(players, players_num);
			printf("No.%d %sさんですね? 投票したい人の番号を入力してください\n", i, players[i].name);
			int n = read_living_other_than_myself_player_num(players, players_num, i);
			ballot_box[n]++;
			clear();
		}
		//誰かが火あぶりにされる
		int dead = ballot_winner(ballot_box, players_num);
		players[dead].is_live = 0;//死んだことにする
		printf("No.%d %s さんを処刑しました\n", dead, players[dead].name);

		//		終了判定 (人狼の数が村人の人数以上なら人狼の，村人しかいないなら村人の勝利)
		switch(win_side(players, players_num)){
			case VILLAGER_SIDE: goto VILLAGER_WIN;
			case WEREWOLF_SIDE: goto WEREWOLF_WIN;
		}
		//夜のターン
		printf("日が沈み，恐ろしい夜が訪れました\n");
		wait_key();
		clear();

		int ballot_box_werewolf[PLAYERS_MAX_NUM];//人狼が複数居る時にターゲットを投票する
		for(int i=0;i<players_num;i++)ballot_box_werewolf[i] = 0;

		for(int i=0;i<players_num;i++){
			if (!players[i].is_live) continue;//死人は飛ばす
			printf("貴方はNo.%d, %s さんですね?\n", i, players[i].name);
			wait_key();
			switch(players[i].role){//各プレイヤーの役職に応じて行動してもらう．
				case VILLAGER://村人だけ異常に早いの嫌だから長岡高専名物数理演習!!をやってもらう
					mathematical_exercises();
					break;
				case WEREWOLF://殺しの対象を投票
					show_players(players, players_num);
					printf("今夜の獲物は?\n");
					int n = read_living_other_than_myself_player_num(players, players_num, i);
					ballot_box_werewolf[n]++;
					break;
			}
			clear();
		}
		//各プレイヤーの状態を決める
		int werewolf_target = ballot_winner(ballot_box_werewolf, players_num);
		players[werewolf_target].is_live = 0;//死んだことにする

		show_players(players, players_num);
		wait_key();
		//		終了判定 (人狼の数が村人の人数以上なら人狼の，村人しかいないなら村人の勝利)
		switch(win_side(players, players_num)){
			case VILLAGER_SIDE: goto VILLAGER_WIN;
			case WEREWOLF_SIDE: goto WEREWOLF_WIN;
		}
		//	各プレイヤーに夜のターンの結果を知らせる
	}
	//	1に戻る

	//人間の勝利
VILLAGER_WIN:
	printf("人間の勝利\n");
	wait_key();
	return 0;
	//人狼の勝利
WEREWOLF_WIN:
	printf("人狼の勝利\n");
	wait_key();
	return 0;
}
