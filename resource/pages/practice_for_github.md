# 人狼完成させよっか…
実践Gitでは人狼を作った…途中まではね．
~~サボったわけじゃないんだよ，チュートリアル作成って大変なんすよ~~
流石にここらへんで完成させないとまずい．

# 作業の続きの前に

TODO: 公開鍵とかの初期設定

## GitHubアカウントを作る
ここはまぁ[GitHub公式](https://github.com)から何となく作れると思うので適宜ぐぐりながらやって．

## リモートリポジトリを作る
GitHubのアカウントが出来たらログインしよう．
左側のRepositoriesって欄に`New`って書いてあるボタンがあるはず… 押してくれ

次の画面で作るリポジトリの名前や，privateかpublic…公開するかどうかを聞かれる．
名前は好きに決めていい，`my_werewolf_gamemaster`とかにしとく?

下部の`Create repository`を押した時点でリモートリポジトリが作られる．
これで人狼のソースコードとその履歴を入れておくための箱(リモートリポジトリ)が出来る．
やってくれ!

## ローカルリポジトリをリモートリポジトリに紐付ける
`Create repository`ボタンを押すと出てくる画面に色々コマンドが書いてあると思う．
英文を読めば分かるが，新しく作る場合と既存のリポジトリをGitHubにあげる場合が書かれている．
~~まぁどっちもやってることは一緒で，
ローカルリポジトリをリモートリポジトリに紐付けてるんだよね… 無かったら作るってだけ~~

今回は後者だ，そしてそのコマンドというのが次の２つだ．
```
git remote add origin [リモートリポジトリのurl]
git push -u origin master
```
`git remote add [別名] [リモートリポジトリのurl]`はリモートリポジトリのurlの別名を登録している．
これをすることでわざわざurlを入力しなくていいし，わかりやすくなる．

`origin`は元のリポジトリのurlに習慣的につける別名だ．

`git push -u [リモートリポジトリのurl] [プランチ名]`は
リモートリポジトリのコミット履歴を指定したブランチの履歴で更新するコマンドだ．
`-u`オプションは自分で調べて… 実は無くてもいいんすよ．~~2回目以降の引数を省略できるんだよね…~~

これらコマンドを実践Gitで作った人狼のディレクトリで実行して，人狼のローカルリポジトリを
新しく作ったリポジトリに紐付けてくれ．
`git push`が成功すればGitHubのサイトからリモートリポジトリに中身が入った事を確認出来るはずだ

## A君が現れた!!
ところで，実はここはGitHubのチュートリアルだったんだ!
だからGitHubの利点~~+注意点~~をわかってもらいやすくするために，
君のイマジナリーフレンドのA君を捕まえて人狼作成の手伝いをしてもらった事にする．

~~ちなみにイマジナリーフレンドのAくんは
国道404沿いの草むらを歩いていれば飛び出して来る．
低レベルなのでクイックボールを投げれば多分捕まえられる．~~

### A君のローカルリポジトリを作成
A君はネットの向こう側に居る誰かという設定だ．
だけど今回は一人二役で演習をする．
君のPCに元のリポジトリの他にA君用のリポジトリを作り，それぞれで違う部分を編集する．

まずはさっきGitHub作ったリモートリポジトリからA君用のリポジトリを作る．
```
git clone [リモートリポジトリのurl]
```

`git clone`はリポジトリをコピーするコマンドだ．特に指定無しだと`master`ブランチのみがコピーされる．

今回はリモートリポジトリをカレントディレクトリ(コピー先を省略した場合)にコピーしている．
コマンドを打つ場所に注意して既存の人狼のローカルリポジトリの中以外の好きな場所に
リモートリポジトリをコピーしよう．

これらの2つのリポジトリをGitHubで上手いことまとめることで人狼を完成させる!

# step7 共同作業
残ったタスクは残りこんくらい…
- 名前を入力してもらう
- 各役職の人数を入力してもらう
- 乱数を本当にランダムにする
- 表示関数から役職表示を消す
- 昼夜の間に死んだ人を表示

これを二人で分担してやる．

## 君の作業
君は元々の予定通り，`finish`ブランチに直接コミットすることにした．
君のローカルリポジトリのfinishブランチに移動し作業を始めよう．

### 名前を入力してもらう
```
//読めるだけ読み込んで後は捨てる
void read_line(char buf[], int buf_size) {
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
```
これが文字列を読み込む関数だ，引数で指定した配列に一行文字列を読み込む．
最大まで読み込んだ時文字が余っていたら捨てている．捨てないと次の入力が誤作動するからだ．

```
for (int i=0;i<players_num;i++){
	players[i].role = roles[i]; //就職
	//ここを消した
	players[i].is_live = 1; //生きてることにする
}

//名前の入力と役職提示
for (int i=0;i<players_num;i++) {
	printf("Player No.%d あなたの名前は?\n", i);
	read_line(players[i].name, sizeof(players[i].name)); 
	printf("%s さん，貴方は %s です\n", players[i].name, ROLE_STRINGS[players[i].role]);
	wait_key();
	clear();
}
```
使う時は今までhogeを適当に代入していた箇所を消してこんな感じで使う．
ここでは名前を入力したら役職を教える様にしている．

### 乱数を本当にランダムにする
本当にランダムにするためには乱数調整でおなじみの…初期シードを決めてやる必要がある．
```
//乱数のシード値を設定
srand((unsigned int)(time(NULL)));
```
今回はシードの初期化に現在の時間を使った．

そうそう…ヘッダのインクルードも忘れずに!
```
#include <time.h>
```

### 昼夜の間に死んだ人を表示
この作業はちょっと変更が大きくなる．
というのも昼のターンと夜のターンをそれぞれ関数にまとめてしまうからだ…~~わかりやすくするためだけの為に~~

新しく関数`daytime()`と`midnight()`を作り，そこにmain関数にベタが書きしていた処理をまとめよう．

死んだ人を表示するために，こんな風に昼夜それぞれのターンの前の状態を記録しておく．
さっき昼夜それぞれのターンをまとめたおかげでわかりやすくなっている~~のかな?~~．
```
//現状の生死の状態を記録
for (int i=0;i<players_num;i++) players_ld[i] = players[i].is_live;
//夜のターン
midnight(players, players_num);
print_cadavers(players_ld, players, players_num);//死んだ人を表示
```

そして各ターンの前後の違いから，その間に死んだ人を表示する関数はこうだ．
```
void print_cadavers(int before[], Player after[], int players_num){
	int count=0;
	for (int i=0;i<players_num;i++) {
		if(before[i] != after[i].is_live){
			printf("No.%d %s さんは死亡しました\n", i, after[i].name);
			count++;
		}
	}
	if (count == 0) printf("誰も死にませんでした\n");
}
```

複数人死者が出てもいいから，サイコキラーとか猫又とかが実装しやすくなるね!
最終的には[こんな感じ](https://github.com/fuller-kport/GitTraining/blob/master/resource/werewolf/step7/you)だ!

コミットも忘れずに!

### GitHubに反映させる
ローカルリポジトリの変更が終わった!
これをGitHubに反映させれば後はA君の作業を待つばかりだ．

次のコマンドでローカルリポジトリの履歴をリモートリポジトリに反映出来る．
```
git push origin [更新を反映させたいブランチ]
```
更新を反映させたいブランチに指定した名前と同じ名前の`origin`上のブランチに変更を反映する．
今回の場合は
`git push origin finish`とすれば良いわけだ．

これで君の仕事は一区切りついた．

## A君の作業:  夜の間に死んだ人を表示
A君はホストに変更内容を取り込んでもらう方針で編集することにした．
A君はこのプロジェクトの主人ではないから，
主人に一度チェックしてから変更を取り込んでもらう方がいいと考えたんだね．

そのためにA君用のローカルリポジトリに移動し，
`finish`から新しいブランチ`add_read_roles`を生やしてそこで作業を始めよう．

### 各役職の人数を入力してもらう
これがその関数だ，入力した番号に応じて各役職を順に配列に入れていく．
```
#define ROLES_NUM 2 

int read_roles(Role roles[]){
	int n;
	int each_num[ROLES_NUM];
	for (int i=0;i<ROLES_NUM;i++) each_num[i] = 0;

	for (int count=0;;) {
		printf ("人数は最大合計%d人です\n", PLAYERS_MAX_NUM);
		printf("現在の各役職の人数はそれぞれ以下の通りです\n");
		for (int j=0;j<ROLES_NUM;j++) {
			printf("%s: \tNo.%d \t%d人\n", ROLE_STRINGS[j], j+1, each_num[j]);
		}
		printf("決定: \tNo.0\n");
		printf("追加する役職番号を入力してください(負号付きで減少)\n");
		for (;;) {
			printf("No.");
			scanf("%d", &n);
			if (-ROLES_NUM<=n && n<=ROLES_NUM) break;
		}
		if (n == 0) { 
			if (count < PLAYERS_MAX_NUM) return count;
		}else if (n < 0) {
			if (each_num[++n] > 0) {
				each_num[n]--;
				count--;
			}
		}else{
			roles[count] = --n;
			each_num[n]++;
			count++;
		}
		clear();
	}
}
```

上の関数が返す役職の配列は入力された順になっているので，役職がバレてしまう．
そこで役職の並びをシャッフルし，ランダムに並べ替える．
```
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
```

これらの関数を使って役職を入力出来るようにしたものが
[こちら!](https://github.com/fuller-kport/GitTraining/blob/master/resource/werewolf/step7/A)

### 表示関数から役職を消す(これはついで)
これはマジでついで…分かるっしょ~~わざわざタイトルにした理由がわからない~~

終わったらコミットを忘れずに!

A君の変更も`origin`(リモートリポジトリ)に反映させよう…
こちらも`git push origin [ブランチ名]`でOKだ．
この場合`git push origin add_print_on_night`っすね!

## 合体!
### プルリクエストを送る
君とA君はどちらも`git push`コマンドで変更をリモートリポジトリに送った．

君の送り先には`finish`ブランチが既にあった，
この場合はGitでコミットした時の様に`origin/finish`ブランチの履歴に追加される．

あっ `origin/○○`って書くとリモートリポジトリの〇〇ブランチってことね．

ではA君の送った`add_read_roles`はどう成るだろう．


### コンフリクトォ!!



