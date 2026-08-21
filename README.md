# FluidSimulation 
![png](FluidSimulation/icon.png)

OpenGLで動作する、2Dリアルタイム流体シミュレーションです。

速度場、圧力場、密度・色の計算をフラグメントシェーダーとRenderTargetを使ってGPU上で実行します。

## Features

### Fluid simulation

- 速度場の移流
- 密度・色の移流
- 発散の計算
- Jacobi法による圧力計算
- Pressure projectionによる速度場の補正
- 渦度の計算
- Vorticity confinement
- 速度拡散
- 密度拡散
- 速度・密度の減衰
- セルサイズの変更
- Jacobi反復回数の変更
- 周期境界（Repeat）

### Resolution

- 流体計算用RenderTargetの解像度変更
- 密度・色用RenderTargetの解像度変更
- 流体計算と密度場を異なる解像度で実行
- 解像度差に応じた密度移流時の速度サンプリング

### Mouse interaction

- 左ドラッグで速度と色を追加
- 右ドラッグで速度のみ追加
- 入力範囲と入力強度の変更

### Color

- 時間によって変化するRainbowモード
- 任意色を指定するOwn Colorモード
- 内側と外側の色を個別に設定
- 内側と外側の色を同期
- 色の強度、彩度、明度、変化速度、位相差の変更
- 加算混色
- 減算混色

### Rendering

以下の流体データを表示できます。

- Color and Density
- Velocity
- Pressure
- Divergence
- Curl

表示結果には以下の効果を組み合わせられます。

- Base color
- Gradient
- Base colorとGradientの積
- 各表示成分の強度調整

### Reset

- シミュレーションのリセット
- 同梱画像を使った密度・色の初期化
- F1キーによるStageの再生成

## Controls

| Input | Action |
|---|---|
| Left Mouse Drag | 速度と色を追加 |
| Right Mouse Drag | 速度を追加 |

その他のパラメーターはImGuiの`Fluid Simulator`ウィンドウから変更できます。

## Requirements

- Windows
- OpenGL 4.6対応GPU・ドライバー
- Visual Studio / MSVC v145
- C++20

使用している主なライブラリ：

- GLFW
- GLAD
- GLM
- Dear ImGui
- spdlog
- stb_image

## Build

1. リポジトリをクローンします。

```powershell
git clone https://github.com/Soichiro-Ishii/FluidSimulation.git
```

2. `FluidSimulation.slnx`をVisual Studioで開きます。

3. `x64`構成でビルドします。

現在の`FluidSimulation.vcxproj`には、一部ライブラリのインクルードディレクトリとライブラリディレクトリとして`C:\prog`以下の絶対パスが設定されています。環境が異なる場合は、各依存ライブラリのパスを変更してください。

シェーダーとテクスチャは相対パスで読み込まれるため、実行時の作業ディレクトリには`FluidSimulation`ディレクトリを指定してください。