# ScreenSaverCanceler2

デスクトップやウインドウアプリの無入力監視を阻害する。

# Features

- スクリーンセーバーが動き出すのを、マウスイベントを発生させて抑止する。
- Windows Appが無入力で回線切断するのを、マウスイベントを発生させて抑止する。

# Requirement

* Microsoft Windows 11（x64）
* たぶん [Microsoft Visual C++ Redistributable for Visual Studio 2022（x64）](https://visualstudio.microsoft.com/ja/downloads/) が必要

# Installation

1. ssc2.exeとssc2.iniをダウンロードしてください。
2. ssc2.exeを実行してください。

# Usage

## スクリーンセーバーを回避する

ssc2.exeを実行するとタスクトレイに常駐します。一定間隔でマウスを動かしたことにしてスクリーンセーバーが動くのを抑止します。

タスクトレイのアイコンを左クリックしてON、OFFを切り替えられます。

iniファイルの設定でインターバルを指定できます。単位は分です。

```
[ssc2]
interval=2
```

## Windows Appの無入力監視を回避する

Windows Appが無入力で回線切断するのを回避するためには、iniファイルの以下の項目を設定してssc2.exeを再起動してください。

```
[ssc2]
send_message=1
caption=【Windows Appのキャプション文字列】
```

Windows Appの接続先でもssc2.exeを実行してください。Windows Appの接続先側ではsend_message=0で問題ありません。

Windows Appを最小化している場合は機能しません。

# Author

はに〇 <hunny@tea.nifty.jp>

# License

MIT License
