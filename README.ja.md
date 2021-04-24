DrawShape
===

![Software Version](http://img.shields.io/badge/Version-v1.1.1-green.svg?style=flat)
![VS Version](http://img.shields.io/badge/VisualStudio-2019-blue.svg?style=flat)
[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)

[English Page](./README.md)

## 概要
簡単にレイヤーや形状を追加し、描画内容を拡大縮小/移動することができる、簡易CADソフトのような ActiveX コントロール。  

- 使用例1
    - ![use1](https://user-images.githubusercontent.com/18702413/71414988-ecee1100-269c-11ea-895c-096abe43e30e.png)

- 使用例2
    - ![use2](https://user-images.githubusercontent.com/18702413/71182037-ac674f80-22b8-11ea-8676-d9a71077a38e.gif)

## バージョン
v1.1.1

## 開発環境
Visual Studio 2019

## ライセンス
MIT License

## 使用したOSS
- [PicoJSON](https://github.com/kazuho/picojson)

## インストール／アンインストール
### インストール方法
- Windows (x86) の場合
    1. DrawShape.ocxを任意のフォルダを配置する。
    2. 管理者権限でコマンドプロンプトを起動する。
    3. `regsvr32 DrawShape.ocx` を実行。
- Windows (x64) の場合
    1. DrawShape.ocxを任意のフォルダを配置する。
    2. 管理者権限でコマンドプロンプトを起動する。
    3. `C:\Windows\SysWow64\regsvr32 DrawShape.ocx` を実行。

### アンインストール方法
- Windows (x86) の場合
    1. DrawShape.ocxを任意のフォルダを配置する。
    2. 管理者権限でコマンドプロンプトを起動する。
    3. `regsvr32 /u DrawShape.ocx` を実行。
- Windows (x64) の場合
    1. DrawShape.ocxを任意のフォルダを配置する。
    2. 管理者権限でコマンドプロンプトを起動する。
    3. `C:\Windows\SysWow64\regsvr32 /u DrawShape.ocx` を実行。

## 使用方法
### C++ / MFCアプリケーション (on Visual Studio 2019)
1. ダイアログへのコントールの挿入方法
    1. リソースビューからダイアログエディタを開く。
    2. ダイアログ上で右クリックして「ActiveXコントロールの挿入...」を選択。
    3. 「ActiveXコントロールの挿入」画面から「DrawShape Control」を選択して「OK」ボタンを押下。
2. プロパティ／メソッドの追加方法
    1. 挿入した DrawShape コントロールを右クリックして「変数の追加...」を選択してコントロール変数を追加する。
    2. プロジェクトに自動的に IDispatch ラッパークラスが挿入され、そのクラスの型のコントロール変数が追加される。
       ラッパークラスのメンバ関数にプロパティの Getter/Setter とメソッドが用意された状態となる。
        - ・・・はずだが、VisualStudio 2017 / 2019 ではうまくいかない。2013では問題なし。
3. イベントハンドラの追加方法
    1. 挿入した DrawShape コントロールを右クリックして「イベントハンドラーの追加...」を選択する。
    2. 表示されたウィザードで追加したいイベントとハンドラのメンバ関数を指定して挿入する。

### C# / Windowsフォームアプリケーション (on Visual Studio 2019)
1. フォームへのコントロールの挿入方法
    1. ソリューションエクスプローラーからフォームのデザイナを開く。
    2. ツールボックスの何もない領域で右クリックして「アイテムの選択...」を選択する。
    3. 「ツールボックスアイテムの選択」画面の「COMコンポーネント」タブを開き、「DrawShape Control」にチェックを付けて「OK」ボタンを押下。
    4. ツールボックス内に「DrawShape Control」が追加されるのでフォームへ挿入する。
    5. 挿入に失敗する場合は、「プロジェクト」メニューの「参照の追加...」の「COM」ページから「DrawShapeLib」にチェックを付けて「OK」ボタンを押下してから試す。
2. プロパティ／メソッドの追加方法
    1. コントロールの挿入を行うことでプロジェクトの参照に AxDrawShapeクラス（AxDrawShapeLib名前空間 - AxDrawShapeクラス）が追加されており、
       コントロール変数はそのクラスの型となっている。プロパティとメソッドも追加されているため使用可能な状態となる。
3. イベントハンドラの追加方法
    1. AxDrawShapeクラス（AxDrawShapeLib名前空間 - AxDrawShapeクラス）に追加されているイベント変数とデリゲート型を利用することでイベントハンドラ関数を追加可能。

[C#サンプルアプリケーション](/DrawShapeTest)

## リファレンス
[リファレンス](REFERENCE.ja.md)
