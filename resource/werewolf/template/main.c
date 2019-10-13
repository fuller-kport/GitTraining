#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PLAYERS_NUM_MAX 20
#define ROLES_NUM 4

//役職全部を列挙
typedef enum{
	VILLAGER,
	WEREWOLF,
	FORETELLER,
	HUNTER
}Role;


//それぞれの役職はこの中の一つに入る
typedef enum{
	SIDE_VILLAGER,
	SIDE_WEREWOLF
}Side;

//Roleに紐付けられる情報の形を定義
typedef struct {
	char name[32];
	Side side;
}RoleInfo;

//Roleに紐付けられる情報をさっき作った型に入れる
RoleInfo ROLE_INFOS[] = {
	{"村人　",   SIDE_VILLAGER},
	{"人狼　",   SIDE_WEREWOLF},
	{"占い師", SIDE_VILLAGER},
	{"狩人　",   SIDE_VILLAGER}
};

//各プレイヤーが持つ情報
typedef struct{
	Role role;
	char name[32];
	int is_live;
}Player;

//プレイヤーの情報を表示 今はデバグ用に役職も表示
void show_players(Player p[], int p_num){
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~ << players! >> ~~~~~~~~~~~~~~~~~~~~~~~\n");
	for(int i=0;i<p_num;i++){
		if (p[i].is_live) printf("No.%d\t name: %s %s\n", i, p[i].name, ROLE_INFOS[p[i].role].name);
	}
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
}

//読みやすさ優先でこの実装
int try_exit(Player p[], int num){
	int werewolf_side_num = 0;
	int villager_side_num = 0;
	for (int i=0; i<num; i++){
		if (!p[i].is_live) continue;
		if (ROLE_INFOS[p[i].role].side == SIDE_WEREWOLF) {
			werewolf_side_num++;
		}else{
			villager_side_num++;
		}
	}
	if (werewolf_side_num >= villager_side_num) {
		printf("人狼 side win!!\n");
		return 1;
	}
	if (werewolf_side_num == 0) {
		printf("村人 side win!!\n");
		return 1;
	}
	return 0;
}

//int配列を完全にランダムに並び替える
void shuffle(int array[], int size)
{
	int i = size;
	while (i > 1) {
		int j = rand() % i;
		i--;
		int t = array[i];
		array[i] = array[j];
		array[j] = t;
	}
}

//配列の中から一つ選ぶ，同票はランダム
int ballot_winner(int ballot_box[], int size){
	int max = 0;
	for(int i=0; i<size; i++) {
		if (ballot_box[max] < ballot_box[i]) max = i; 
	}
	int j = 0;
	int arr[PLAYERS_NUM_MAX];
	for(int i=0; i<size; i++) {
		if (ballot_box[max] == ballot_box[i]){
			arr[j] = i;
			j++;
		}
	}
	shuffle(arr, j);
	return arr[0];
}

//標準入出力(黒?あるいは白い画面)から入力を全て読み捨てる
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

//読めるだけ読み込んで後は捨てる
void readLine(char buf[], int buf_size) {
	int i;
	char c;
	for(i=0; i<buf_size; i++) {
		buf[i] = getchar();
		if (buf[i] == '\n') {
			buf[i] = '\0';
			return;
		}
	}
	buf[buf_size-1] = '\0';
	clean_stdin();
}

//昼のターン
void daytime(Player players[], int players_num){
	printf("\n昼のターン\n");
	show_players(players, players_num);
	printf("怪しい誰かの噂話を いつものように\n");
	printf("時間カウントする予定\n");
	printf("嫌いな誰かは決まりましたか?\n");
	wait_key();
	puts("\033[2J");

	int ballot_box[PLAYERS_NUM_MAX];
	for(int i=0; i<players_num; i++) ballot_box[i] = 0;
	for(int i=0; i<players_num; i++) {
		if(!players[i].is_live) continue;
		show_players(players, players_num);
		printf("%s さんですね?\n", players[i].name);
		printf("誰を処刑しますか?\n");
		int n = read_living_other_than_myself_player_num(players, players_num, i);
		ballot_box[n]++;
		puts("\033[2J");
	}
	int dead = ballot_winner(ballot_box, players_num);
	players[dead].is_live = 0;
	printf("%s さんを処刑しました\n", players[dead].name);
}

//夜のターン
void midnight(Player players[], int players_num){
	int werewolf_ballot_box[PLAYERS_NUM_MAX];
	int is_protected[PLAYERS_NUM_MAX];
	for(int i=0; i<players_num; i++) werewolf_ballot_box[i] = 0;
	for(int i=0; i<players_num; i++) is_protected[i] = 0;

	for(int i=0; i<players_num; i++) {
		//死んでる人はパス
		if(!players[i].is_live) continue;

		printf("<< 真夜中 >>\n");
		show_players(players, players_num);

		printf("貴方は %s さんですね?\n", players[i].name);
		wait_key();

		int n;
		int val1;
		int val2;
		switch (players[i].role) {
			case VILLAGER:
				//村人だけ異常に決断が早いのは問題だから
				val1 = rand()%1000;
				val2 = val1 + rand()%10;
				printf("数理演習 問1  %d - %d = ?\n", val2, val1);
				for (;;) {
					printf("A: ");
					scanf("%d", &n);
					wait_key();
					if (n == val2 - val1) {
						break;
					}else{
						printf("留年\n\n");
					}
				}
				break;
			case WEREWOLF:
				printf("がぶぅ! するあいて だれ?\n"); //表現に問題あり
				n = read_living_other_than_myself_player_num(players, players_num, i);
				werewolf_ballot_box[n]++;
				break;
			case FORETELLER:
				printf("誰を占う?\n");
				n = read_living_other_than_myself_player_num(players, players_num, i);
				if (players[n].role == WEREWOLF) {
					printf("%s さんは人狼でした\n", players[n].name);
				}else{
					printf("%s さんは人狼ではありませんでした\n", players[n].name);
				}
				wait_key();
				break;
			case HUNTER:
				printf("今夜守るべきは…\n");
				n = read_living_other_than_myself_player_num(players, players_num, i);
				is_protected[n] = 1;
				break;
		}
		puts("\033[2J");
	}
	int kill = ballot_winner(werewolf_ballot_box, players_num);
	if (!is_protected[kill]) players[kill].is_live = 0;
}

int read_roles(int roles[]){
	int n;
	int each_num[ROLES_NUM];
	for (int i=0;i<ROLES_NUM;i++) each_num[i] = 0;

	for (int count = 0;count<PLAYERS_NUM_MAX;count++) {
		printf("人数は最大 合計%d 人です\n", PLAYERS_NUM_MAX);
		printf("現在の人数はそれぞれ以下の通りです\n");
		for (int j=0;j<ROLES_NUM;j++) {
			printf("%s: \tNo.%d \t%d人\n", ROLE_INFOS[j].name, j+1, each_num[j]);
		}
		printf("終了: \tNo.0");
		printf("\n追加する役職番号を入力してください\n");
		for (;;) {
			printf("No.");
			scanf("%d", &n);
			n--;
			each_num[n]++;
			if (-1<=n && n<ROLES_NUM) break;
		}
		if (n == -1) return count;
		roles[count] = n;
		puts("\033[2J");
	}
}

int main(void){
	srand((unsigned int)(time(NULL)));
	Player players[PLAYERS_NUM_MAX];

	//shuffleを使うためにRoleでは無くint配列にした… いい子はこんなコード書いちゃ駄目だよ
	int roles[PLAYERS_NUM_MAX];
	int players_num = read_roles(roles);
	shuffle(roles, players_num);
	puts("\033[2J");

	for(int i=0;i<players_num;i++) {
		players[i].is_live = 1;
		players[i].role = (Role)roles[i]; //roles[i]の手前の(Role)が何を示すのか分からなかった
	}

	printf("ある朝平和な村で死体が発見されました\n");
	printf("村の中に残忍な人狼が紛れ込んだようです(超理論)\n");
	wait_key();

	for (int i=0;i<players_num;i++) {
		printf("hi! player No.%d あなたの名前は?\n", i);
		readLine(players[i].name, sizeof(players[i].name)); 
		printf("%s さん，貴方は %s です\n", players[i].name, ROLE_INFOS[players[i].role].name);
		wait_key();
		puts("\033[2J");
	}

	//ゲームループ開始
	for(;;){
		//昼のターン
		daytime(players, players_num);
		if (try_exit(players, players_num)) return 0;
		printf("日が落ち，辺りは闇に包まれました\n");
		wait_key();

		//現状の生死の状態を記録
		int player_is_live[PLAYERS_NUM_MAX];
		for (int i=0;i<players_num;i++) player_is_live[i] = players[i].is_live;
		//夜のターン
		midnight(players, players_num);

		//夜の間に死んだ人を表示
		printf("深い夜が明け，日が辺りを照らしています\n");
		int count=0;
		for (int i=0;i<players_num;i++) {
			if(player_is_live[i] != players[i].is_live){
				printf("%s さんが無残な姿で発見されました\n", players[i].name);
				count++;
			}
		}
		if (count == 0) printf("昨晩の犠牲者はいませんでした\n");

		//終了判定，終了処理
		if (try_exit(players, players_num)) return 0;
	}
	return 0;
}

/*
#人狼ゲームを作ろう!
どうやって遊ぶか?
今回はゲームマスターを作る
自分の番が来たらPCの前に来て昼or夜の行動を選択するんだ
↑
後々通信出来るようにしたいね!

#人狼ゲームの仕様を決める
ユーザからの入力はどこから?
端末の標準入出力から

どんな役職がある?
村人
人狼
占い
猟師
霊媒 (後回し)

#ゲームの流れ
0. 役職を割り振る
0.0. 各役職の人数を入力してもらう
0.1. 各プレイヤーの名前を入力してもらう+プレイヤーに役職を教える
1. 昼のターン
1.0. 一定時間待つ
1.1. 処刑したい人を選んでもらう
1.2. かわいそうな誰かに泣いてもらう
1.3. 終了判定 (人狼の数が村人の人数以上なら人狼の，村人しかいないなら村人の勝利)
2. 夜のターン
2.0. 各プレイヤーの役職に応じてメッセージを出す
2.1. 能力を使いたい人を選んでもらう
2.2. 各プレイヤーの状態を決める
2.3. 終了判定 (人狼の数が村人の人数以上なら人狼の，村人しかいないなら村人の勝利)
3. 
3.0. 各プレイヤーに夜のターンの結果を知らせる
4. 1に戻る

人狼ゲームが何をしているかを分けていくとゲームの大まかな流れはこうなる

やることが多いと感じるかもしれない
ただ安心してほしい，最初に動くプログラムはずっと小さいものになるからだ

例えば
入力をもらう部分は後回しでも良い
数字を直接入れておけば代用出来るからだ

必要知識のリンク達
変数
配列
関数
*/
